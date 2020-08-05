/* GTK-based application grid
 * Copyright (c) 2020 Piotr Miller
 * e-mail: nwg.piotr@gmail.com
 * Website: http://nwg.pl
 * Project: https://github.com/nwg-piotr/nwg-launchers
 * License: GPL3
 * *
 * Credits for window transparency go to AthanasiusOfAlex at https://stackoverflow.com/a/21460337
 * transparent.cpp
 * Code adapted from 'alphademo.c' by Mike
 * (http://plan99.net/~mike/blog--now a dead link--unable to find it.)
 * as modified by karlphillip for StackExchange:
 * (https://stackoverflow.com/questions/3908565/how-to-make-gtk-window-background-transparent)
 * Re-worked for Gtkmm 3.0 by Louis Melahn, L.C. January 31, 2014.
 * */

#include "nwg_tools.h"
#include "grid.h"

MainWindow::MainWindow(): CommonWindow("~nwggrid", "~nwggrid") {
    searchbox.set_text("Type to search");
    searchbox.set_sensitive(false);
    searchbox.set_name("searchbox");
    search_phrase = "";
    apps_grid.set_column_spacing(5);
    apps_grid.set_row_spacing(5);
    apps_grid.set_column_homogeneous(true);
    favs_grid.set_column_spacing(5);
    favs_grid.set_row_spacing(5);
    favs_grid.set_column_homogeneous(true);
    pinned_grid.set_column_spacing(5);
    pinned_grid.set_row_spacing(5);
    pinned_grid.set_column_homogeneous(true);
    label_desc.set_text("");
    label_desc.set_name("description");
    description = &label_desc;
    separator.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    separator.set_name("separator");
    separator1.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    separator1.set_name("separator");
    add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);
    // We can not go fullscreen() here:
    // On sway the window would become opaque - we don't want it
    // On i3 all windows below will be hidden - we don't want it as well
    if (wm != "sway" && wm != "i3") {
        fullscreen();
    } else {
        set_type_hint(Gdk::WINDOW_TYPE_HINT_SPLASHSCREEN);
        set_decorated(false);
    }
}

bool MainWindow::on_key_press_event(GdkEventKey* key_event) {
    if (key_event -> keyval == GDK_KEY_Escape) {
        Gtk::Main::quit();
        return Gtk::Window::on_key_press_event(key_event);
    } else if (((key_event -> keyval >= GDK_KEY_A && key_event -> keyval <= GDK_KEY_Z)
        || (key_event -> keyval >= GDK_KEY_a && key_event -> keyval <= GDK_KEY_z)
        || (key_event -> keyval >= GDK_KEY_0 && key_event -> keyval <= GDK_KEY_9)
        || (key_event -> keyval == GDK_KEY_space))
        && key_event->type == GDK_KEY_PRESS) {

        char character = key_event -> keyval;
        this -> search_phrase += character;
        this -> searchbox.set_text(this -> search_phrase);
        this -> filter_view();
        return true;
    } else if (key_event -> keyval == GDK_KEY_BackSpace && this -> search_phrase.size() > 0) {
        this -> search_phrase = this -> search_phrase.substr(0, this -> search_phrase.size() - 1);
        this -> searchbox.set_text(this -> search_phrase);
        this -> filter_view();
        return true;
    } else if (key_event -> keyval == GDK_KEY_Delete) {
        this -> search_phrase = "";
        this -> searchbox.set_text(this -> search_phrase);
        this -> filter_view();
        return true;
    }
    //if the event has not been handled, call the base class
    return Gtk::Window::on_key_press_event(key_event);
}

void MainWindow::filter_view() {
    if (this -> search_phrase.size() > 0) {
        this -> filtered_boxes.clear();

        for (auto& box : this -> all_boxes) {
            if (box.name.uppercase().find(this -> search_phrase.uppercase()) != std::string::npos
                || box.exec.uppercase().find(this -> search_phrase.uppercase()) != std::string::npos
                || box.comment.uppercase().find(this -> search_phrase.uppercase()) != std::string::npos) {

                this -> filtered_boxes.emplace_back(&box);
            }
        }
        this -> favs_grid.hide();
        this -> separator.hide();
        this -> rebuild_grid(true);
    } else {
        this -> favs_grid.show();
        this -> separator.show();
        this -> rebuild_grid(false);
    }
}

void MainWindow::rebuild_grid(bool filtered) {
    int column = 0;
    int row = 0;

    for (Gtk::Widget *widget : this -> apps_grid.get_children()) {
        this -> apps_grid.remove(*widget);
        widget -> unset_state_flags(Gtk::STATE_FLAG_PRELIGHT);
    }
    int cnt = 0;
    this -> apps_grid.freeze_child_notify();
    if (filtered) {
        for (auto& box : this -> filtered_boxes) {
            this -> apps_grid.attach(*box, column, row, 1, 1);
            if (column < num_col - 1) {
                column++;
            } else {
                column = 0;
                row++;
            }
            cnt++;
        }
    } else {
        for (auto& box : this -> all_boxes) {
            this -> apps_grid.attach(box, column, row, 1, 1);
            if (column < num_col - 1) {
                column++;
            } else {
                column = 0;
                row++;
            }
            cnt++;
        }
    }
    this -> apps_grid.thaw_child_notify();
    // Set keyboard focus to the first visible button
    if (this -> favs_grid.is_visible()) {
        auto* first = favs_grid.get_child_at(0, 0);
        if (first) {
            first -> grab_focus();
        }
    } else {
        auto* first = apps_grid.get_child_at(0, 0);
        if (first) {
            first -> grab_focus();
        }
    }
}

GridBox::GridBox(Glib::ustring name, Glib::ustring exec, Glib::ustring comment, bool pinned)
 : AppBox(std::move(name), std::move(exec), std::move(comment)), pinned(pinned) {}

bool GridBox::on_button_press_event(GdkEventButton* event) {
    int clicks = 0;
    try {
        clicks = cache[exec];
        clicks++;
    } catch(...) {
        clicks = 1;
    }
    cache[exec] = clicks;
    save_json(cache, cache_file);    

    exec.append(" &");
    std::system(exec.data());

    if (pins && event->button == 3) {
        if (pinned) {
            remove_and_save_pinned(exec);
        } else {
            add_and_save_pinned(exec);
        }
    }

    Gtk::Main::quit();
    return true;
}

bool GridBox::on_focus_in_event(GdkEventFocus* event) {
    (void) event; // suppress warning

    description -> set_text(comment);
    return true;
}

void GridBox::on_enter() {
    description -> set_text(comment);
    return AppBox::on_enter();
}

