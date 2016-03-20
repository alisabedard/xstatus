# xstatus
simple X11 status bar and launcher for lightweight window managers

# How to use custom status
The ruby utility rbstatus is included as an example of how to provide
a custom updating status string to xstatus.  xstatus reads /tmp/xstatus
periodically, displaying the text therein.  You may have to modify the
files polled in rbstatus to reflect your own system.  

## TODO
* Compute geometries, rather than using so many fixed values.
* Add menus
* Add Xft support

