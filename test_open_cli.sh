#!/bin/bash
./src/leafpad test.txt &
LEAFPAD_PID=$!
sleep 1
import -window root screenshot.png
kill $LEAFPAD_PID
