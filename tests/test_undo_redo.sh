#!/bin/bash

# Exit on error
set -e

# Clean up previous test files
rm -f test_*.txt

# Start leafpad in the background
../leafpad_fltk-fltk-port-undo4/src/leafpad > /dev/null 2>&1 &
LEAFPAD_PID=$!

# Wait for the main window to be ready
WINDOW_ID=""
for i in {1..10}; do
  WINDOW_ID=$(xdotool search --sync --onlyvisible --pid ${LEAFPAD_PID} | head -1)
  if [ -n "$WINDOW_ID" ]; then
    break
  fi
  sleep 0.5
done

if [ -z "$WINDOW_ID" ]; then
  echo "Error: Leafpad window not found."
  # Check if process exists before killing
  ps -p $LEAFPAD_PID > /dev/null && kill $LEAFPAD_PID
  exit 1
fi

sleep 1

# --- Reusable SaveAs Function ---
save_as() {
    local filename=$1
    xdotool key --window $WINDOW_ID "ctrl+shift+s"
    sleep 1
    local dialog_id=$(xdotool search --sync --pid ${LEAFPAD_PID} --onlyvisible --name "Save File As" | head -1)
    if [ -z "$dialog_id" ]; then
      echo "Error: 'Save File As' dialog not found."
      kill $LEAFPAD_PID
      exit 1
    fi
    xdotool key --window $dialog_id "ctrl+a" "BackSpace"
    xdotool type --window $dialog_id "$filename"
    xdotool key --window $dialog_id "Return"
    sleep 1
}

# --- Reusable Paste Function ---
paste_text() {
    local text=$1
    echo -n "$text" | xclip -selection clipboard
    xdotool key --window $WINDOW_ID "ctrl+v"
    sleep 1
}

# --- Test Execution ---

# 1. Paste "original text" and save
paste_text "original text"
save_as "test_initial.txt"

# 2. Paste " extra text" and save
paste_text " extra text"
save_as "test_extra.txt"

# 3. Undo and save
echo "Testing Undo..."
xdotool key --window $WINDOW_ID "ctrl+z"
sleep 1
save_as "test_undo.txt"

# 4. Redo and save
echo "Testing Redo..."
xdotool key --window $WINDOW_ID "ctrl+shift+z"
sleep 1
save_as "test_redo.txt"

# --- Cleanup ---
kill $LEAFPAD_PID

# --- Verification ---
echo "Verifying results..."

# Verify initial save
EXPECTED_INITIAL="original text"
if [ "$(cat test_initial.txt)" != "$EXPECTED_INITIAL" ]; then
  echo "Initial save test FAILED!"
  exit 1
fi
echo "Initial save test PASSED."

# Verify extra text save
EXPECTED_EXTRA="original text extra text"
if [ "$(cat test_extra.txt)" != "$EXPECTED_EXTRA" ]; then
  echo "Extra text save test FAILED!"
  exit 1
fi
echo "Extra text save test PASSED."

# Verify undo
EXPECTED_UNDO="original text"
if [ "$(cat test_undo.txt)" != "$EXPECTED_UNDO" ]; then
  echo "Undo test FAILED!"
  exit 1
fi
echo "Undo test PASSED."

# Verify redo
EXPECTED_REDO="original text extra text"
if [ "$(cat test_redo.txt)" != "$EXPECTED_REDO" ]; then
  echo "Redo test FAILED!"
  exit 1
fi
echo "Redo test PASSED."

echo "All tests passed."
exit 0
