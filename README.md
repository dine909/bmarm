# Bare Metal [QBS](http://doc.qt.io/qbs) GCC Arm

Bare Metal multi vendor Blinky ARM SDK in [QBS](http://doc.qt.io/qbs) for GCC compilers.


------------------	
## What is this?

This is a project utilising Qt build system ([QBS](http://doc.qt.io/qbs)) to manage the cross-compilation of an application targeted for Arm MCUs.  The structure enables fast porting between different MCUs and also facilitates compilation for desktop targets.

It is comprised of:

 * Latest Arm CMSIS driver libraries
 * Vendor specific driver libraries
 * Managed linker generation 
 * Blinky example (using systick) that compiles for every board
 * Mini vendor specific GPIO library to facilitate the blinky.
 
------------------	
## Which MCU's are currently supported/tested?

Currently there are examples for TI, NXP/LPC, NXP/Freescale/Kinetis and Cypress/Spansion (STM examples to follow)

MCU definitions are located in `modules/` and formatted like: `chip_*`

------------------	
## Which boards are currently supported/tested?

A select few boards are implemented.

Board definitions are located in `modules/` and formatted like: `board_*`
 
------------------	
## Who should use this?

The intent of this library is to provide a ready to go environment for development, testing and verification of production firmware for commercial use.  Secondarily, it enables porting between MCU's and Desktop targets to be trivial.  This is especially handy as it allows offline development of your application on desktop targets, which with a little thought and pre-planning, can make debugging a sizeable project a pleasure and a breeze.

**This project is not** aimed at arduino lovers, or mbed fanatics, as it does require integration with vendor specific system & peripheral libraries, and thus requires a level of understanding that is usually masked by these frameworks.  It must be stressed that the knowledge required to achieve this is not as complicated as you may think, but does involve extensive reading, a good place to start skimming is Arm's [website](http://arm-software.github.io/CMSIS_5/General/html/index.html), and [CMSIS](https://github.com/ARM-software/CMSIS_5) libraries.

If you have never downloaded and read an MCU datasheet, then this project is probably not for you.

------------------	
## But why not Arduino or mbed?

Apart from inheriting all their bugs and shortcomings, these frameworks are tragically implemented, especially in terms of efficiency.  Their communities are toxic and full of misinformation.  If you are serious about developing commercially viable firmware with arduino or mbed frameworks, then I humbly suggest you reconsider and learn more about your target MCU and implement the peripherals you need as the vendor intended, this repo will facilitate both learning of that process and implementation.  

------------------	
## Why [QBS](http://doc.qt.io/qbs)?

[QBS](http://doc.qt.io/qbs) accomplishes the same task as make and cmake, but is far more flexible and readable.

It allows conditional forward and backwards dependency attributes to be applied to the build.  For this project [QBS](http://doc.qt.io/qbs) is essential to organising build variant's compilation source and build environment per vendor MCU.  

Make and cmake just cant deliver this amount of flexibility, whilst retaining readability.

------------------	
## Do I need Qt Creator to use this?

No, [QBS](http://doc.qt.io/qbs) is a cli tool akin to make & cmake.  

Qt Creator does compliment [QBS](http://doc.qt.io/qbs) in order to display only the current compilation source in the tree.

------------------	
# Getting started

Firstly, I urge you to file a bug report if you are struggling to 'get started' as the intention of this project is to expedite the 'getting started' process, if you run into problems, then that is a bug.

`THIS README IS WIP AND WILL BE COMPLETED OVER THE NEXT COUPLE OF DAYS - PLEASE BARE WITH.`