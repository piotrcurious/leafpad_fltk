#!/bin/bash
./src/leafpad &
LEAFPAD_PID=$!
sleep 1
xdotool search --name "Leafpad" windowactivate
xdotool key "alt+f"
xdotool key "q"
wait $LEAFPAD_PID
if [ $? -eq 0 ]; then
  echo "Test passed: Application exited successfully."
else
  echo "Test failed: Application did not exit as expected."
fi
