WINNER
------

An exploration of what it takes to write a window server that runs on top of (Ubuntu) Linux.

Running
-------

`winner` *must* be run as root from inside a real Terminal to work (not a Terminal window under X11. A virtual Terminal like Ctrl-Alt-F1 gives you is fine). If it is not run in a real Terminal, X11 will clobber its drawings and you won't see anything. If it is not run as root, it will be unable to acquire the framebuffer device and terminate with an error.

Test application
----------------

Currently, the test application will erase the frame buffer to white, then draw a few shapes in it, then immediately return.