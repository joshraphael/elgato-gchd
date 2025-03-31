It is possible to configure the Elgato Game Capture HD device to run without root permissions. To do so you must configure [udev](https://en.wikipedia.org/wiki/Udev).

To do so, there is a [file](https://github.com/tolga9009/elgato-gchd/blob/dev/udev-rules/55-elgato-game-capture.rules) named `55-elgato-game-capture.rules` included in the `udev-rules` directory of the driver.

To set up non-root access:

1. Copy the file to /etc/udev/rules.d as root:

    ```
    sudo cp 55-elgato-game-capture.rules /etc/udev/rules.d
    ```    

2. After you install this, you will have to notify udev of the change:
   
   ```
   sudo udevadm control --reload
   ```

3. Unplug the Elgato Game Capture HD device and plug it back in.

4. You then must add the users who use this device to the "video" group. IE, for the user "jsmith":
   ```
   sudo usermod jsmith -a -G video
   ```

5. Make sure that the /tmp/gchd.ts is deleted, because the user will not be able to open it if it owned by root.
   ```
   sudo rm /tmp/gchd.ts
   ```

6. The user will then need to logout and log back in for it to work. Alternatively the user can type:
   ```
   newgrp video
   newgrp
   ```
   To use it without logging out. The 2nd `newgrp` is necessary to prevent all files the user creates until they log out from belonging to group video.


