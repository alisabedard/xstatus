# xstatus
simple X11 status bar and launcher for lightweight window managers

# Note on Submodule
xstatus uses a submodule, libjb.  Please either clone this project recursively,
or make sure you run the following:

    git submodule init
    git submodule update

# Custom Status Text
The ruby utility rbstatus.rb is included as an example of how to provide
a custom updating status string to xstatus.  xstatus reads /tmp/xstatus
periodically, displaying the text therein.  You may have to modify the
files polled in rbstatus.rb to reflect your own system.  

# TODO
* Compute geometries, rather than using so many fixed values.
* Add menus
* Add Xft support

