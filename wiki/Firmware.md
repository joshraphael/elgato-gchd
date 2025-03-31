This software needs Elgato Game Capture HD firmware files to work. Due to
licensing issues, they're not part of this repository.

You need to extract them yourself from the Windows "Game Capture" software,
or the official Mac OS X drivers. Getting it from each is a different process.

**Note**: If you're a Mac OS X user, simply install the official drivers. You
don't need to manually extract the firmware files.

## Getting Firmware from Windows "Game Capture" software

**Note: Currently these instructions only work currently for the development version of the driver. If you are not using the development version of the driver, [get the firmware from the Macintosh Driver](#getting-firmware-from-macintosh-driver).**

1. Download Game Capture from:

   https://www.elgato.com/en/game-capture-hd-support

   Game Capture 3.2 is known to work. You will not need the hardware driver,
   just the Game Capture software (which is a much bigger download).

2. Make sure p7zip is installed on your Linux box. On Debian based systems
   this can be achieved with the following command (as root).

   ```
   apt-get install p7zip-full
   ```

3. Copy/Move the GameCapture*.msi file to the `firmware_extract` directory found in your downloaded driver.

4. Change your current directory to the `firmware_extract` directory and then run:

    ```
    ./extract_firmware_windows
    ```

    This will create a `Firmware.tgz` file.

5. Change your current directory to either `/usr/lib/firmware` or `/usr/local/lib/firmware`
   You may need to be root.

6. Untar `Firmware.tgz` while in one of these two directories. You will probably
    need to be root.

    ```
    tar xvf /full/path/to/elgato-gchd/firmware_extract/Firmware.tgz
    ```

7. The firmware files should end up in `/usr/lib/firmware/gchd` or
   `/usr/local/lib/firmware/gchd` directories.

## Getting Firmware from Macintosh Driver

1. Download the Elgato Game Capture HD Mac OS X driver version 2.0.3 from the Elgato official
website: https://www.elgato.com/en/game-capture-hd-support

**NOTE**: Elgato seems to have removed version 2.0.3 from their site. 2.5.0 and above is available. Look through a list of older versions of the software here https://help.elgato.com/hc/de/articles/360027963512-Elgato-Game-Capture-HD-Software-Release-Notes-macOS for a .dmg file (2.5.0 tested to be working)

2. Install the tools to loopback mount the `gchd_*.dmg` file.
   There are two ways to do this:

    A) Using `dmg2img` and `hfsprogs`. These tools currently don't work
       properly on .dmg files create on later versions of OSX. So you will
       want to make sure you get the 2.0.3 version of the driver for this.

    B) Install `darling-dmg` from https://github.com/darlinghq/darling-dmg
       and use it to do this step. This is a much better method, but
       darling-dmg isn't currently available as a package for most
       Linux distros.

3. Once the tools are installed, mount .dmg file.

   **Method A**

      Uncompress Elgato Game Capture HD Mac OS X drivers:

      ```
      dmg2img gchdm_203_970.dmg -o gchdm_203_970.dmg.img
      ```

      Then you can mount the uncompressed HFS+ image with root permissions:

      ```
      mkdir /tmp/dmg
      mount -o loop -t hfsplus gchdm_203_970.dmg.img /tmp/dmg
      ```

   **Method B**

      Mount the HFS+ image directly. Root permissions are not required.

      ```
      mkdir /tmp/dmg
      darling-dmg gchdm_203_970.dmg /tmp/dmg
      ```

4. Copy `mb86h57_h58_idle.bin`, `mb86h57_h58_enc_h.bin`,
   `mb86m01_assp_nsec_idle.bin`, and `mb86m01_assp_nsec_enc_h.bin`
   from `/tmp/dmg/Game\ Capture\ HD.app/Contents/Resources/Firmware/Beddo/`.

5. Place the firmware files either in `/usr/lib/firmware/gchd` or
   `/usr/local/lib/firmware/gchd` folder. You might need root permissions.

**Note**: for testing purposes, you can also place the firmware files in the
same directory, where the compiled executable `gchd` is located.
