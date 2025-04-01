#!/bin/bash

CUR_DIR=$(pwd)

sudo apt-get update
sudo apt-get upgrade -y
sudo apt-get install git libusb-dev clang make build-essential cmake libusb-1.0 pkg-config 7zip p7zip-full -y
rm -rf firmware
rm -rf dmg
rm -rf build
rm -f gchdm_*.dmg
cd $CUR_DIR/firmware_extract
ls | grep -v extract_firmware_windows.sh | xargs rm -f
cd $CUR_DIR
sudo rm -rf /usr/local/lib/firmware/gchd
sudo mkdir -p /usr/local/lib/firmware/gchd