# Waterpump

**This is an ongoing project so do not expect everything to work !**

This project is an automatic system for watering plants. Everyday, pumps are switched on for a few seconds to provide enough water to some plants. The device can be controlled by sending SMS to change the schedule. In addition, a small push-button is present to water plants immediately.

## Build and flash instructions

First of all, you need to install the [ARM embedded toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm) and [JLink Software and Documentation Pack](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack).

There are two types of build: release and debug.

To build the project, run one of these commands:
```
$ make                  # BUILDTYPE set to release
$ BUILDTYPE=release make
$ BUILDTYPE=debug make
```

Connect the board to a SEGGER debug probe and run these commands to flash or debug the firmware:
```
$ make flash-target                 # Flash release firmware
$ BUILDTYPE=debug make flash-target # Flash debug firmware
$ BUILDTYPE=debug make debug-target # Start debugging firmware
```
