#!/bin/bash

# Wrapper script to prevent fontconfig/freetype loading
# This provides additional protection beyond the code changes

# Unset any font-related environment variables that might trigger fontconfig
unset FONTCONFIG_FILE
unset FONTCONFIG_PATH

# Tell programs to prefer X11 rendering
export GDK_USE_XFT=0
export QT_XFT=false

# Ensure X11 font path includes our minimal fonts
xset +fp /usr/share/fonts/X11/misc/
xset fp rehash

# Launch the application
exec ./src/leafpad "$@"
