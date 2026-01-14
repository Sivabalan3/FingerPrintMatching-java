#!/bin/bash
set -e

# Go into the r307 folder relative to this script
cd r307

# Compile the C++ program
g++ r307_capture.cpp -o r307_capture

# Make binary executable
chmod +x r307_capture

echo "R307 capture binary built successfully"
