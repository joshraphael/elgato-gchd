#!/bin/bash

sudo ./build/src/gchd -color-space rgb # FIFO -- open /tmp/gchd.ts in VLC

# sudo ./build/src/gchd -f socket -color-space rgb # Socket - shell script: ffplay udp://0.0.0.0:57384