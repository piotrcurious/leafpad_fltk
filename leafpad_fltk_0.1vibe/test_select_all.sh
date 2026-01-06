#!/bin/bash
set -e

# Start fluxbox in the background
fluxbox &
FLUXBOX_PID=$!
sleep 1

# Start the application
./src/leafpad &
LEAFPAD_PID=$!
sleep 2

# Find the window ID and activate it
WINDOW_ID=$(xdotool search --pid $LEAFPAD_PID | head -1)
xdotool windowactivate $WINDOW_ID

# Perform the test actions
xdotool type --delay 100 "Hello, world!"
sleep 1
xdotool key --clearmodifiers "alt+e"
sleep 0.5
xdotool key "a"
sleep 1
xdotool key --clearmodifiers "ctrl+x"
sleep 1
import -window root screenshot_after_select_all_cut.png

# Kill the background processes
kill $LEAFPAD_PID
kill $FLUXBOX_PID
