If you have a problem with the driver we could use a capture! This is easily accomplished using Wireshark.

# Installing the Utilities
You need to make sure you have the comamands `lsusb` and `tshark` installed (`tshark` is console version of Wireshark). 

`lsusb` is part of usbutils`, and can be installed on Debian with:
```
apt-get install usbutils
```

`tshark` is part of wireshark, and may be a separate package or part of wireshark. On Debian it is installed with:
```
apt-get install tshark
```

Now there is a chance that your Linux distribution doesn't install usb capture support for tshark, even after you install it. If not, look for packages with the names `pcap`, `wireshark`, or `tshark` in their name that also mention usb.

# Testing the Utilities

Run the following command with the device attached:
```
lsusb | grep -i Elgato
```
This should give you output similar to this:
`Bus 008 Device 117: ID 0fd9:005d Elgato Systems GmbH`

Please note the bus number, as you'll need it for the Wireshark capture.

Next run:
```
sudo tshark -D
```
This will list interfaces that Wireshark can capture on. There should be some interfaces listed as `usbmon1`, `usbmon2`, etc. If not you probably need to install USB support for wireshark.

# Performing the Capture

1. Open up two console windows or tabs. One for tshark, the other for running gchd, and another. If you use an application for reading from a fifo, such as OBS, vlc, ffmpeg, make sure that application is prepped to open the fifo.
2. Identify what bus your device is on with:

  ```
  lsusb | grep -i Elgato
  ```

3. Unplug the USB cable from the device. 
4. Run the following command, substituting for usbmon1 the appropriate usbmonX that matches the devices bus. 

  ```
  sudo tshark -i usbmon1 -w mycapture.pcap
  ```

5. Plug the USB cable for the device back in, make sure you don't change the usb port from what it was plugged into before.
6. Run gchd in the other window. Specify any command line options necessary for the capture. I do recommend you specify the input source to avoid autodetect issues.

Example for HDMI:

  ```
  sudo gchd -i hdmi
  ```

7. Start your application that reads from the fifo.
8. Capture for a few seconds.
9. Shutdown the capture. ctrl-c the console running gchd, then ctrl-c the console running tshark.
10. Verify `mycapture.pcap` has been created. 
11. Send it to us with a description of the problem.
