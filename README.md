# nwg-launchers
As it's never too late to learn something new, I decided to try and code my
[sgtk-menu](https://github.com/nwg-piotr/sgtk-menu) set of launchers, written in python, from scratch in C++.
By the way I'm trying to simplify their usage, by reducing the number of arguments. Whatever possible, is being moved
to css style sheets.

![demo-gif](http://nwg.pl/Lychee/uploads/big/0b284762e075ee04df1d0a378ce5e20a.gif)

It's damned difficult to make all the stuff behave properly on all window managers. My priorities are:

1. it **must work well** on sway;
2. it **should work as well as possible** on i3, dwm and Openbox.

Feel free to report issues you encounter on other window managers / desktop environments, but they may or may not be resolved.

# Packages

The latest released version is [available](https://aur.archlinux.org/packages/nwg-launchers) in Arch User Repository.
Current development version (`master` branch) may be installed as the `nwg-launchers-git` AUR package.
For other Linux distributions see the table below.

[![Packaging status](https://repology.org/badge/vertical-allrepos/nwg-launchers.svg)](https://repology.org/project/nwg-launchers/versions)

# Building and installing

To build nwg-launchers from source, you need a copy of the source code, which
can be obtained by cloning the repository or by downloading and unpacking [the
latest release](https://github.com/nwg-piotr/nwg-launchers/releases/latest).

## Dependencies

- `gtkmm3` (`libgtkmm-3.0-dev`)
- `nlohmann-json` - optional, can be downloaded as a subproject
- `meson` and `ninja` - build dependencies

## Building

This project uses the Meson build system for building and installing the
executables and the necessary data. The options that can be passed to the
`meson` command can be found in the `meson_options.txt` file, and can be used to
disable building some of the available programs.

```
$ git clone https://github.com/nwg-piotr/nwg-launchers.git
$ cd nwg-launchers
$ meson builddir -Dbuildtype=release
$ ninja -C builddir
```

## Installation

To install:

```
$ sudo ninja -C builddir install
```

To uninstall:

```
$ sudo ninja -C builddir uninstall
```

**Note: the descriptions below apply to the `master` branch. Certain features may or may not be available in the latest
release, as well as in the current package for your Linux distribution.**

# nwggrid

This command creates a GNOME-like application grid, with the search box, optionally prepended with a row of `-f` favourites
(most frequently used apps) or `-p` pinned program icons.

This only works with the `-p` argument:

- to pin up a program icon, right click its icon in the applications grid;
- to unpin a program, right click its icon in the pinned programs grid.

*Hit "Delete" key to clear the search box.*

[![Swappshot-Mon-Mar-23-205030-2020.th.png](https://scrot.cloud/images/2020/03/23/Swappshot-Mon-Mar-23-205030-2020.th.png)](https://scrot.cloud/image/jb3k) [![Swappshot-Mon-Mar-23-205157-2020.th.png](https://scrot.cloud/images/2020/03/23/Swappshot-Mon-Mar-23-205157-2020.th.png)](https://scrot.cloud/image/jOWg) [![Swappshot-Mon-Mar-23-205248-2020.th.png](https://scrot.cloud/images/2020/03/23/Swappshot-Mon-Mar-23-205248-2020.th.png)](https://scrot.cloud/image/joh5)

### Usage

```
$ nwggrid -h
GTK application grid: nwggrid 0.3.1 (c) Piotr Miller 2020 & Contributors

Options:
-h               show this help message and exit
-f               display favourites (most used entries)
-p               display pinned entries
-o <opacity>     default (black) background opacity (0.0 - 1.0, default 0.9)
-b <background>  background colour in RRGGBB or RRGGBBAA format (RRGGBBAA alpha overrides <opacity>)
-n <col>         number of grid columns (default: 6)
-s <size>        button image size (default: 72)
-c <name>        css file name (default: style.css)
-l <ln>          force use of <ln> language
-wm <wmname>     window manager name (if can not be detected)
```

### Custom background

Use -b <RRGGBB> | <RRGGBBAA> argument (w/o #) to define custom background colour. If alpha value given, it overrides
the opacity, as well default, as defined with the -o <opacity> argument.

### Custom styling

On first run the program creates the `nwg-launchers/nwggrid` folder in your .config directory. You'll find the `style.css` files inside.
You may edit the style sheet to your liking.

# nwgbar

This command creates a horizontal or vertical button bar, out of a template file.

[![Swappshot-Mon-Mar-23-210713-2020.th.png](https://scrot.cloud/images/2020/03/23/Swappshot-Mon-Mar-23-210713-2020.th.png)](https://scrot.cloud/image/jRPQ) [![Swappshot-Mon-Mar-23-210652-2020.th.png](https://scrot.cloud/images/2020/03/23/Swappshot-Mon-Mar-23-210652-2020.th.png)](https://scrot.cloud/image/j8LU)

### Usage

```
$ nwgbar -h
GTK button bar: nwgbar 0.3.1 (c) Piotr Miller & Contributors 2020

Options:
-h               show this help message and exit
-v               arrange buttons vertically
-ha <l>|<r>      horizontal alignment left/right (default: center)
-va <t>|<b>      vertical alignment top/bottom (default: middle)
-t <name>        template file name (default: bar.json)
-c <name>        css file name (default: style.css)
-o <opacity>     background opacity (0.0 - 1.0, default 0.9)
-b <background>  background colour in RRGGBB or RRGGBBAA format (RRGGBBAA alpha overrides <opacity>)
-s <size>        button image size (default: 72)
-wm <wmname>     window manager name (if can not be detected)
```

### Custom background

Use -b <RRGGBB> | <RRGGBBAA> argument (w/o #) to define custom background colour. If alpha value given, it overrides
the opacity, as well default, as defined with the -o <opacity> argument.

### Customization

On first run the program creates the `nwg-launchers/nwgbar` folder in your .config directory. You'll find a sample template `bar.json`
and the `style.css` files inside.

Templates use json format. The default one defines an example Exit menu for sway window manager on Arch Linux:

```json
[
  {
    "name": "Lock screen",
    "exec": "swaylock -f -c 000000",
    "icon": "system-lock-screen"
  },
  {
    "name": "Logout",
    "exec": "swaymsg exit",
    "icon": "system-log-out"
  },
  {
    "name": "Reboot",
    "exec": "systemctl reboot",
    "icon": "system-reboot"
  },
  {
    "name": "Shutdown",
    "exec": "systemctl -i poweroff",
    "icon": "system-shutdown"
  }
]
```

You may use as many templates as you need, with the `-t` argument. All of them must be placed in the config directory.
You may use own icon files instead of icon names, like `/path/to/the/file/my_icon.svg`.

The style sheet makes the buttons look similar to `nwggrid`. You can customize them as well.

# nwgdmenu

This program provides 2 commands:

- `<input> | nwgdmenu` - displays newline-separated stdin input as a GTK menu
- `nwgdmenu` - creates a GTK menu out of commands found in $PATH

*Hit "Delete" to clear the search box.*
*Hit "Insert" to switch case sensitivity.*

[![Swappshot-Mon-Mar-23-211702-2020.th.png](https://scrot.cloud/images/2020/03/23/Swappshot-Mon-Mar-23-211702-2020.th.png)](https://scrot.cloud/image/jfHK) [![Swappshot-Mon-Mar-23-211911-2020.th.png](https://scrot.cloud/images/2020/03/23/Swappshot-Mon-Mar-23-211911-2020.th.png)](https://scrot.cloud/image/j3MG) [![Swappshot-Mon-Mar-23-211736-2020.th.png](https://scrot.cloud/images/2020/03/23/Swappshot-Mon-Mar-23-211736-2020.th.png)](https://scrot.cloud/image/jvOi)

### Usage

```
$ nwgdmenu -h
GTK dynamic menu: nwgdmenu 0.3.1 (c) Piotr Miller & Contributors 2020

<input> | nwgdmenu - displays newline-separated stdin input as a GTK menu
nwgdmenu - creates a GTK menu out of commands found in $PATH

Options:
-h               show this help message and exit
-n               no search box
-ha <l>|<r>      horizontal alignment left/right (default: center)
-va <t>|<b>      vertical alignment top/bottom (default: middle)
-r <rows>        number of rows (default: 20)
-c <name>        css file name (default: style.css)
-o <opacity>     background opacity (0.0 - 1.0, default 0.3)
-b <background>  background colour in RRGGBB or RRGGBBAA format (RRGGBBAA alpha overrides <opacity>)
-wm <wmname>     window manager name (if can not be detected)
-run             ignore stdin, always build from commands in $PATH(1)

Hotkeys:
Delete        clear search box
Insert        switch case sensitivity
```

(1) _The program should auto-detect if something has been passed in `stdin`, and build the menu out of the `stdin` content
or from commands found in `$PATH` accordingly. However, in some specific cases (e.g. if you use gdm and start nwgdmenu
from a key binding) the `stdin` content detection may be false-positive, which results in displaying an empty menu.
In such case use the `nwgdmenu -run` instead, to force building the menu out of commands in `$PATH`._

Notice: if you start your WM from a script (w/o DM), only sway and i3 will be auto-detected. You may need to pass the WM name as the argument:

`nwgdmenu -wm dwm`

The generic name `tiling` will be accepted as well.

### Custom background

Use -b <RRGGBB> | <RRGGBBAA> argument (w/o #) to define custom background colour. If alpha value given, it overrides
the opacity, as well default, as defined with the -o <opacity> argument.

### Custom styling

On first run the program creates the `nwg-launchers/nwgdmenu` folder in your .config directory. You'll find the
default `style.css` files inside. Use it to adjust styling and a vertical margin to the menu, if needed.

## i3 note

In case you use default window borders, an exclusion like this may be necessary:

```
for_window [title="~nwg"] border none
```

## Openbox Note

To start nwgdmenu from a key binding, use the `-run` argument, e.g.:

```xml
<keybind key="W-D">
  <action name="Execute">
    <command>nwgdmenu -run</command>
  </action>
</keybind>
```

## Tips & tricks

### Hide unwanted icons in nwggrid

See: [https://wiki.archlinux.org/index.php/desktop_entries#Hide_desktop_entries](https://wiki.archlinux.org/index.php/desktop_entries#Hide_desktop_entries)
