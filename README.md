# Kindlewall

**Frame a folder of photos into Kindle-sized wallpapers, one image at a time.** Kindlewall is a small, keyboard-friendly desktop app built with Qt Quick. It keeps your originals untouched and saves finished crops to a folder you choose.

> Linux is the current tested desktop target. Windows build instructions are included, but a Windows release still needs to be built and verified on Windows. Android is experimental and is not ready for release; see [Platform status](#platform-status).

## What it does

- Loads supported images from a folder and presents them in a review queue.
- Frames each image to a custom output resolution.
- Moves the crop with the arrow keys; zooms from 50% to 200%; rotates from -180° to 180°.
- Fills the canvas with a cover-scaled version of the image when zoomed out.
- Saves a high-quality JPEG, advances to the next image, and leaves the source unchanged.
- Skips images, undoes the last save or skip, and remembers output and Kindle folders.
- Copies completed wallpapers to a chosen folder, such as a mounted Kindle directory.

Supported inputs: JPEG, PNG, WebP, BMP, and TIFF (depending on the Qt image plugins installed).

## Keyboard controls

| Key | Action |
| --- | --- |
| Arrow keys | Nudge the crop |
| Enter | Save this crop and move to the next image |
| Backspace | Skip this image |
| Ctrl+Z | Undo the last save or skip |
| Ctrl+O | Choose an image folder |

Use the on-screen zoom and tilt sliders for larger adjustments. The mouse wheel changes a slider while the pointer is over it.

## Build on Linux

Requirements: Qt 6 with Quick, QML, and Quick Controls; qmake6; a C++17 compiler; and make. On Arch-based systems, install `qt6-base`, `qt6-declarative`, and `base-devel`.

```sh
./bin/build
./kindlewall
```

To install a local desktop launcher on Linux:

```sh
sudo install -Dm755 ./kindlewall /usr/local/bin/kindlewall
sudo install -Dm644 ./pkgbuild/kindlewall.desktop /usr/local/share/applications/kindlewall.desktop
```

## Build and package on Windows

Use a Windows machine with Qt 6 (Quick, QML, Quick Controls), an MSVC kit, and the matching Visual Studio C++ Build Tools. Open the Qt 6 MSVC command prompt, clone the repository, then run:

```bat
bin\build-windows.bat
```

For a portable folder, run `windeployqt` on the built `kindlewall.exe` from that same Qt kit and distribute the executable together with the deployed Qt files. Build and test the package on Windows before publishing it; the Windows build has not yet been verified by this project.

## Platform status

| Platform | Status | Notes |
| --- | --- | --- |
| Linux | Working desktop build | Qt 6 / qmake build script included. |
| Windows | Build path prepared; unverified | Requires a Windows Qt/MSVC environment and runtime deployment. |
| Android | Experimental; not release-ready | Android scoped storage needs a Storage Access Framework adapter with persistent folder permissions. Kindle transfer also needs an Android-specific workflow. Do not publish the current Android script as a usable release. |

## Project layout

```text
src/       Qt Quick UI and C++ image/queue backend
bin/       Platform build scripts
pkgbuild/  Linux desktop launcher metadata
android/   Early Android manifest
```

## Data and privacy

Kindlewall processes images locally. It does not upload photos. It does not modify source files. The configured output and Kindle folder paths are stored in the operating system's application settings.

## License

MIT. See [LICENSE](LICENSE).
