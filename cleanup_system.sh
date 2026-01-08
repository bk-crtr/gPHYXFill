#!/bin/bash
set -e

echo "🧹 Starting AGGRESSIVE Cleanup..."

# 1. Kill duplicate processes
echo "🔪 Killing running processes..."
killall "gPHYXFill" 2>/dev/null || true
killall "gPHYXFillXPC" 2>/dev/null || true
killall "gPHYXEditor" 2>/dev/null || true
killall "Motion" 2>/dev/null || true
killall "Final Cut Pro" 2>/dev/null || true

# 2. Remove from DerivedData (Huge source of duplicates)
echo "🗑️  Removing DerivedData..."
rm -rf ~/Library/Developer/Xcode/DerivedData/gPHYXFill-*

# 3. Remove local build artifacts
echo "🗑️  Removing local builds..."
rm -rf "$HOME/Projects/gPHYXFill/build"
rm -rf "$HOME/Projects/gPHYXFill/gPHYXFill.app" 2>/dev/null || true

# 4. Remove from user Applications
echo "🗑️  Removing from ~/Applications..."
rm -rf "$HOME/Applications/gPHYXFill.app"

# 5. Remove system installation (to allow clean reinstall)
echo "🗑️  Removing system installation..."
rm -rf "/Applications/gPHYXFill.app"

# 6. Unregister with lsregister (cleanup database)
echo "unregistering..."
/System/Library/Frameworks/CoreServices.framework/Frameworks/LaunchServices.framework/Support/lsregister -u "/Applications/gPHYXFill.app" 2>/dev/null || true
/System/Library/Frameworks/CoreServices.framework/Frameworks/LaunchServices.framework/Support/lsregister -u "$HOME/Applications/gPHYXFill.app" 2>/dev/null || true

echo "✅ Cleanup Complete. System is ready for clean build."
