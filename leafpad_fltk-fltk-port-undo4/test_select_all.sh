#!/bin/bash
./src/leafpad &
LEAFPAD_PID=$!
sleep 2
xdotool search --name "Leafpad" windowactivate --sync
xdotool type --delay 100 "Hello, world!"
sleep 1
xdotool key --clearmodifiers "alt+e"
xdotool key "a"
sleep 1
xdotool key --clearmodifiers "ctrl+x"
sleep 1
import -window root screenshot_after_select_all_cut.png
kill $LEAFPAD_PID
