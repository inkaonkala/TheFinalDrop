#!/bin/bash

set -e

PROJECT_NAME="TheFinalDrops"
BUILD_DIR="build"
BUNDLE_DIR="bundle"
APP_NAME="${PROJECT_NAME}.app"
APP_DIR="${BUNDLE_DIR}/${APP_NAME}"
ZIP_FILE="${PROJECT_NAME}_mac.zip"

# 🔧 Path to your local SFML 2.6.2 Frameworks (just built)
SFML_FRAMEWORKS_DIR="../SFML-2.6.2/framework-install/Frameworks"

# === Clean ===
if [ "$1" == "clean" ]; then
    echo "🧹 Cleaning build, bundle, and zip..."
    rm -rf "$BUILD_DIR" "$BUNDLE_DIR" "$ZIP_FILE"
    echo "✅ Clean complete!"
    exit 0
fi

echo "📦 Building project..."

rm -rf "$BUILD_DIR" "$BUNDLE_DIR" "$ZIP_FILE"
mkdir "$BUILD_DIR"
cd "$BUILD_DIR"
cmake ..
make -j
cd ..

# === Create .app bundle structure ===
echo "🎁 Creating .app bundle..."
mkdir -p "$APP_DIR/Contents/MacOS"
mkdir -p "$APP_DIR/Contents/Resources/assets"
mkdir -p "$APP_DIR/Contents/Frameworks"

# Copy game binary
cp "$BUILD_DIR/$PROJECT_NAME" "$APP_DIR/Contents/MacOS/"

# Copy assets
cp -r assets/* "$APP_DIR/Contents/Resources/assets/"

# Copy SFML Frameworks
echo "📦 Copying SFML 2.6.2 frameworks..."
cp -R "$SFML_FRAMEWORKS_DIR"/SFML*.framework "$APP_DIR/Contents/Frameworks/"

# Create Info.plist
cat > "$APP_DIR/Contents/Info.plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
 "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleName</key>
    <string>${PROJECT_NAME}</string>
    <key>CFBundleExecutable</key>
    <string>${PROJECT_NAME}</string>
    <key>CFBundleIdentifier</key>
    <string>com.yourname.${PROJECT_NAME}</string>
    <key>CFBundleVersion</key>
    <string>1.0</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
</dict>
</plist>
EOF

# === Fix SFML Framework Paths ===
echo "🔧 Patching binary framework paths..."
BIN="$APP_DIR/Contents/MacOS/$PROJECT_NAME"

for framework in SFML SFMLGraphics SFMLWindow SFMLSystem SFMLAudio SFMLNetwork; do
    install_name_tool -change \
        "/Library/Frameworks/${framework}.framework/Versions/A/${framework}" \
        "@executable_path/../Frameworks/${framework}.framework/Versions/A/${framework}" \
        "$BIN" || echo "⚠️ Could not patch $framework (maybe not used)"
done

# === Remove Quarantine ===
echo "🚫 Removing quarantine flags..."
xattr -rd com.apple.quarantine "$APP_DIR" || true

# === Zip it up ===
echo "🗜️ Creating zip for itch.io..."
cd "$BUNDLE_DIR"
zip -r "../$ZIP_FILE" "$APP_NAME"
cd ..

echo "✅ Done! Game ready to upload: $ZIP_FILE"
