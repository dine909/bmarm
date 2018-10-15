# Bare Metal Arm

Bare Metal multi vendor Blinky ARM SDK in QBS 


## What is this?

This is a project utilising Qt's QBS build system to manage the compilation of an application targeted for Arm MCUs.  The structure enables fast porting between different MCUs and also facilitates compilation on a desktop.

It is comprised of:

 * Arm CMSIS libraries
 * Vendor specific driver libraries
 * Managed linker generation 
 * Blinky example (using systick) that compiles for every board
 * Vendor specific LED test library to facilitate the blinky.
 
## Which MCU's are currently supported/tested?

Currently there are examples for TI, NXP/LPC, NXP/Freescale/Kinetis and Cypress/Spansion

MCU definitions are located in modules/ and formatted like: chip_* 

## Which boards are currently supported/tested?

A select few boards are implemented.

Board definitions are located in modules/ and formatted like: board_* 
 
## Who should use this?

The intent of this library is to provide a ready to go environment for development, testing and verification of production firmware for commercial use.  Secondarily, it enables porting between MCU's and Desktop targets to be trivial.  This is especially handy as it allows offline development on desktop targets.

It is **not** aimed at arduino lovers, or mbed fanatics as it does require vendor integration with vendor specific system and peripheral libraries, and thus requires a level of knowledge that is masked by these frameworks.

If you have never downloaded and read an MCU datasheet, then this project is not for you.

## But why not Arduino or mbed?

Apart from inheriting all their bugs and shortcomings, these frameworks are tragically implemented, especially in terms of efficiency.  If you are serious about developing firmware commercially with arduino or mbed frameworks, then i humbly suggest you reconsider and learn more about your target MCU and do it all yourself, it isn't hard at all.  If that isn't possible, you should probably reconsider your career path, perhaps mcdonalds or something.

## Why QBS?

QBS accomplishes the same task as make and cmake, but is far more flexible and readable.

It allows conditional forward and backwards dependency attributes to be applied to the build.  For this project QBS is essential to organising build variant's compilation source and build environment per vendor MCU.  

Make and cmake just cant deliver this amount of flexibility, whilst retaining readability.

## Do i need Qt Creator to use this?

No, QBS is a cli tool akin to make & cmake.  Qt Creator does compliment QBS in order to display only the current compilation source in the tree.

