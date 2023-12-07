# PLY

"PLY" is a configuration drive shortcut manager.

What dose it do:
- Manage all shortcuts, command in one place.
- Start program with user input params or pre-defined params.
- Filter result use 5 match mode.
- Customizable color, font.

# Usage

## Keyboard Operation
|key|function|
|:---  |:---  |
|<kbd>Enter</kbd>| run selected item |
|<kbd>Ctrl</kbd> + <kbd>Enter</kbd>| run selected item and exit |
|<kbd>Esc</kbd>| exit |
|<kbd>Tab</kbd>| show item info |
|<kbd>Ctrl</kbd> + <kbd>r</kbd>| restart |
|<kbd>Ctrl</kbd> + <kbd>1~9,0</kbd>| select 1~9,10 line |
|<kbd>Ctrl</kbd> + <kbd>g</kbd>| jump mode |
|<kbd>Alt</kbd> + <kbd>1</kbd>| match prefix |
|<kbd>Alt</kbd> + <kbd>2</kbd>| match in middle string |
|<kbd>Alt</kbd> + <kbd>3</kbd>| match suffix |
|<kbd>Alt</kbd> + <kbd>4</kbd>| match any substring |
|<kbd>Alt</kbd> + <kbd>5</kbd>| match abbrev, for example: `pif` will match `ply is fun` |
|<kbd>Alt</kbd> + <kbd>0</kbd>| toggle match case |
|<kbd>Ctrl</kbd> + <kbd>n</kbd>| next page |
|<kbd>Ctrl</kbd> + <kbd>p</kbd>| previous page |
|<kbd>Ctrl</kbd> + <kbd>j</kbd>| next line |
|<kbd>Ctrl</kbd> + <kbd>k</kbd>| previous line |
|<kbd>Ctrl</kbd> + <kbd>\\</kbd>| change work dir |

## Mouse Operation
|key|function|
|:---  |:---  |
|left mouse button| select an item |
|right mouse button| run selected item |
|middle mouse button| show item info |
|right mouse button drag to left| next page |
|right mouse button drag to right| previous page |

## Conf File

See example `ply.conf`, you can start command `ply my-own-ply.conf`
to run a specific config file.


## Item File

Description of an item:

```
  ;; the name can contain any char except TAB
name
  ;; directory used when run item
  dir:
  ;; the path to executable file
  exe:
  ;; arguments
  arg:
  ;; replace string in arguments, replace once
  ;; can be list (comma separated)
  var:
  ;; comment for this item, show in bottom line
  ;; if you selected it
  cmm:
  ;; include other item
  ;; can be list (tab separated)
  inc:
  - this line ignored
  " you can write any comment style
  # that start with two space
```

How does the item run:

```
(name)--[only]-->{RUN name}
  |
  |--(dir || cmm)-[only]-->{CHDIR dir}
  |--(dir && exe)--->{CHDIR dir + RUN exe}
  |--(exe && arg)--->{RUN exe arg}
  |--(exe && arg && var)--->{INPUT var RUN exe arg}
  |--(inc)--->{RUN name1 name2 name3 ...}
```

Notice the **file encoding**.

Example:

```
chrome'(incognito)
  exe:chrome
  arg:--incognito

cmd output to vim
  exe:cmd
  arg:/c %c | vim -
  var:%c

mpv
  dir:D:\APP\mpv-x86_64-20230108-git-c8a9000
  exe:cmd
  arg:/c start D:\APP\mpv-x86_64-20230108-git-c8a9000\mpv.exe %a
  var:%a
  cmm:run mpv with arguments

wifi connect
  exe:netsh
  arg:wlan connect %n
  var:%n

wifi disconnect
  exe:netsh
  arg:wlan disconnect

magnify.exe

edge'(clean)
  exe:taskkill
  arg:/f /im msedge.exe /t

```

