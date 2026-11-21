#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
: "${ANDROID_NDK_ROOT:?Set ANDROID_NDK_ROOT to the installed Android NDK}"
: "${ANDROID_SDK_ROOT:?Set ANDROID_SDK_ROOT to the installed Android SDK}"
qmake6 kindlewall.pro -spec android-clang CONFIG+=release
make -j"$(nproc)"
make apk
