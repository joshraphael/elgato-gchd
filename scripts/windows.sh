#!/bin/bash

# https://edge.elgato.com/egc/windows/egcw/3.20/final/GameCaptureSetup_3.20.33.1533_x64.msi

GAME_CAPTURE_VERSION=3.20
GAME_CAPTURE_SLUG=GameCaptureSetup_3.20.33.1533_x64

cd firmware_extract/
curl --output ${GAME_CAPTURE_SLUG}.msi --location "https://edge.elgato.com/egc/windows/egcw/${GAME_CAPTURE_VERSION}/final/${GAME_CAPTURE_SLUG}.msi"
bash extract_firmware_windows.sh
tar xvf Firmware.tgz
mv gchd/* .
rm -rf gchd
sudo cp MB86H57_H58_ENC_H /usr/local/lib/firmware/gchd/
sudo cp MB86H57_H58_IDLE /usr/local/lib/firmware/gchd/
sudo cp MB86M01_ASSP_NSEC_ENC_H /usr/local/lib/firmware/gchd/
sudo cp MB86M01_ASSP_NSEC_IDLE /usr/local/lib/firmware/gchd/
cd ..