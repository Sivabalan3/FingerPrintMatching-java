#!/bin/bash
set -e

echo "🔧 Building R307 capture binary..."

# Go into r307 folder
cd r307

g++ -std=c++17 \
    r307_capture.cpp \
    r307_driver.cpp \
    base64.cpp \
    -o r307_capture

chmod +x r307_capture

echo "✅ R307 capture binary built successfully"
