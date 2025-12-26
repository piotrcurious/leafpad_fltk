#!/bin/bash
./src/leafpad &
LEAFPAD_PID=$!
sleep 1
xdotool search --name "Leafpad" windowactivate
xdotool key "alt+f"
xdotool key "o"
sleep 1
xdotool type "test.txt"
xdotool key "Return"
sleep 1
TITLE=$(xdotool search --name "Leafpad" getwindowname)
if [ "$TITLE" == "test.txt" ]; then
  echo "Test passed: File opened successfully."
else
  echo "Test failed: File not opened."
fi
kill $LEAFPAD_PID
