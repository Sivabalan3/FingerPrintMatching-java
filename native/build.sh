#!/bin/bash
set -e

cd native/r307
g++ r307_capture.cpp -o r307_capture
chmod +x r307_capture
echo "R307 capture binary built successfully"
