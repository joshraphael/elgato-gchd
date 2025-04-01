#!/bin/bash

# https://edge.elgato.com/egc/windows/egcw/3.20/final/GameCaptureSetup_3.20.33.1533_x64.msi

GAME_CAPTURE_VERSION=3.20
GAME_CAPTURE_SLUG=GameCaptureSetup_3.20.33.1533_x64

sudo apt-get update
sudo apt-get upgrade -y
sudo apt-get install git libusb-dev clang make build-essential cmake libusb-1.0 pkg-config 7zip p7zip-full -y
rm -rf firmware
rm -rf dmg
rm -rf build
rm -f ${GAME_CAPTURE_SLUG}.dmg
rm -f ${GAME_CAPTURE_SLUG}.dmg.img
rm firmware_extract/!\(extract_firmware_windows.sh\)
sudo rm -rf /usr/local/lib/firmware/gchd
curl --output firmware_extract/${GAME_CAPTURE_SLUG}.msi --location "https://edge.elgato.com/egc/windows/egcw/${GAME_CAPTURE_VERSION}/final/${GAME_CAPTURE_SLUG}.msi"