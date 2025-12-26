#!/bin/bash
./src/leafpad &
LEAFPAD_PID=$!
sleep 1
xdotool search --name "Leafpad" windowactivate
xdotool type "Hello, world!"
xdotool key "alt+f"
xdotool key "n"
sleep 1
TEXT=$(xdotool search --name "Leafpad" getwindowname)
if [ "$TEXT" == "Leafpad" ]; then
  echo "Test passed: Editor is empty."
else
  echo "Test failed: Editor is not empty."
fi
kill $LEAFPAD_PID
