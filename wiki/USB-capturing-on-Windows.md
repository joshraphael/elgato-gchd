This is an overview on how to obtain a Wireshark capture under Windows:

1. Setup your environment, make sure you got Elgato HDMI Out connected to a monitor. Update Game Capture HD software to latest available stable release.
2. Run Game Capture HD software, make sure input resolution matches output resolution.
3. Download and Install Wireshark and install WinPcap during install. WinPcap is needed for capturing USB.
4. Reboot your PC. This is needed for installing the changed USB man-in-the-middle drivers and is essential.
5. Start Wireshark, plug in your Elgato and check, on which USBPcap interface your Elgato is connected to. Ideally, your Elgato should be exclusively on that interface. If it isn't, try another USB port (3.0 preferred, but USB 2.0 port is also okay). It helps to unplug *all* unneeded devices, so the noise keeps at a minimum level, which will make analyzing easier.
6. Plug out your Elgato, close Game Capture HD, and start a fresh Wireshark capture now. Ideally, Wireshark shouldn't capture anything, as long as your Elgato is plugged out.
7. With your Elgato plugged out, start Game Capture HD. You should now have unplugged Elgato, running Game Capture HD software and running Wireshark.
8. Now, plug in your Elgato. Watch Game Capture HD picking up your game video, wait a few seconds (5 - 10 seconds), then shut down Game Capture HD. Wait another 10 seconds. Now press the "Stop" button in Wireshark (no activity should be present), save the capture and upload it.

This way, we will have a very clean capture, which we can work with. Would be cool, if you could help us!