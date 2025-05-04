#!/bin/bash

set -e  # Exit on error

PROJECT_NAME="JamsEscape"
BUILD_DIR="build"
BUNDLE_DIR="game_bundle"
ZIP_FILE="${PROJECT_NAME}_mac.zip"

# === Clean command ===
if [ "$1" == "clean" ]; then
    echo "🧹 Cleaning build, bundle, and zip files..."
    rm -rf "$BUILD_DIR" "$BUNDLE_DIR" "$ZIP_FILE"
    echo "✅ Clean complete!"
    exit 0
fi

# === Build process ===
echo "📦 Building project..."

# Clean previous builds
rm -rf "$BUILD_DIR" "$BUNDLE_DIR" "$ZIP_FILE"

# Create build folder
mkdir "$BUILD_DIR"
cd "$BUILD_DIR"

# Run CMake
echo "⚙️ Running CMake..."
cmake ..

# Build the project
echo "🛠️ Building project..."
make -j

cd ..

# === Bundle creation ===
echo "🎁 Preparing game bundle..."
mkdir "$BUNDLE_DIR"
cp "$BUILD_DIR/$PROJECT_NAME" "$BUNDLE_DIR/"
cp -r assets "$BUNDLE_DIR/"

# === Copy SFML dynamic libraries ===
echo "📦 Copying SFML .dylib files..."
SFML_LIB_DIR="$BUILD_DIR/SFML/lib"
SFML_DYLIBS=$(find "$SFML_LIB_DIR" -name "libsfml-*.dylib")

for lib in $SFML_DYLIBS; do
    cp "$lib" "$BUNDLE_DIR/"
done

# === Remove quarantine flags ===
echo "🚫 Removing quarantine flags from binary and libraries..."

xattr -rd com.apple.quarantine "$BUNDLE_DIR/$PROJECT_NAME" || echo "⚠️ Could not remove from game binary"

for file in "$BUNDLE_DIR"/*.{dylib}; do
    [ -e "$file" ] && xattr -rd com.apple.quarantine "$file" || true
done

# Optional: also clean quarantine from assets (fonts, music, etc.)
find "$BUNDLE_DIR/assets" -exec xattr -rd com.apple.quarantine {} \; 2>/dev/null

# === Zip the bundle ===
echo "🗜️ Zipping..."
zip -r "$ZIP_FILE" "$BUNDLE_DIR"

echo "✅ Done! Game is ready to share: $ZIP_FILE"
