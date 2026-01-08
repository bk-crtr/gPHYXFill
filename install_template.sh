#!/bin/bash

TEMPLATE_SRC="templates/gPHYX Tools"
DEST_ROOT="$HOME/Movies/Motion Templates.localized/Effects.localized"

if [ ! -d "$DEST_ROOT" ]; then
    echo "Creating destination directory..."
    mkdir -p "$DEST_ROOT"
fi

echo "Installing gPHYX Fill Motion Template..."
cp -R "$TEMPLATE_SRC" "$DEST_ROOT/"

echo "Registering plugin with PlugInKit..."
# Path to the build product (default location)
BUILD_PATH="$HOME/Library/Developer/Xcode/DerivedData/gPHYXFill-aowoygxohojjbpdysrhrzsfbfzxy/Build/Products/Debug/gPHYXFill.app/Contents/XPCServices/gPHYXFillXPC.pluginkit"

if [ -d "$BUILD_PATH" ]; then
    pluginkit -a "$BUILD_PATH"
    echo "Plugin registered successfully."
else
    echo "Warning: Build product not found at $BUILD_PATH."
    echo "Please build the project in Xcode first."
fi

echo "Done! Please restart Final Cut Pro and look for 'gPHYX Tools' in Effects."
