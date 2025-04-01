#!/bin/bash

# https://edge.elgato.com/egc/macos/egcm/2.5/final/gchdm_25_1119.dmg
# https://edge.elgato.com/egc/macos/egcm/2.5.1/final/gchdm_251_1123.dmg
# https://edge.elgato.com/egc/macos/egcm/2.5.2/final/gchdm_252_1130.dmg
# https://edge.elgato.com/egc/macos/egcm/2.6/final/gchdm_26_1214.dmg
# https://edge.elgato.com/egc/macos/egcm/2.6.1/final/gchdm_261_1222.dmg
# https://edge.elgato.com/egc/macos/egcm/2.7/final/gchdm_27_1304.dmg

GAME_CAPTURE_VERSION=2.5
GAME_CAPTURE_SLUG=gchdm_25_1119

sudo apt-get update
sudo apt-get upgrade -y
sudo apt-get install git libusb-dev clang make build-essential cmake libusb-1.0 pkg-config 7zip p7zip-full -y
rm -rf firmware
rm -rf dmg
rm -rf build
rm -f ${GAME_CAPTURE_SLUG}.dmg
rm -f ${GAME_CAPTURE_SLUG}.dmg.img
sudo rm -rf /usr/local/lib/firmware/gchd
mkdir -p firmware
curl --output ${GAME_CAPTURE_SLUG}.dmg --location "https://edge.elgato.com/egc/macos/egcm/${GAME_CAPTURE_VERSION}/final/${GAME_CAPTURE_SLUG}.dmg"
mkdir -p dmg
7z x -odmg -y '-x!*/Applications*' gchdm_25_1119.dmg
sudo mkdir -p /usr/local/lib/firmware/gchd
ls -la dmg/
sudo cp dmg/Game\ Capture\ HD/Game\ Capture\ HD.app/Contents/Resources/Firmware/Beddo/mb86h57_h58_idle.bin /usr/local/lib/firmware/gchd/
sudo cp dmg/Game\ Capture\ HD/Game\ Capture\ HD.app/Contents/Resources/Firmware/Beddo/mb86h57_h58_enc_h.bin /usr/local/lib/firmware/gchd/
sudo cp dmg/Game\ Capture\ HD/Game\ Capture\ HD.app/Contents/Resources/Firmware/Beddo/mb86m01_assp_nsec_idle.bin /usr/local/lib/firmware/gchd/
sudo cp dmg/Game\ Capture\ HD/Game\ Capture\ HD.app/Contents/Resources/Firmware/Beddo/mb86m01_assp_nsec_enc_h.bin /usr/local/lib/firmware/gchd/
mkdir -p build
cd build
cmake ..
make