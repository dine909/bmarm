# Bare Metal Arm

Bare Metal multi vendor Blinky ARM SDK in QBS 


## What is this?

This is a project utilising Qt's QBS build system to manage the compilation of an application targeted for Arm MCUs.  The structure enables fast porting between different MCUs and also facilitates compilation on a desktop.

## Who should use this?

The intent of this library is to provide a ready to go environment for development, testing and verification of production firmware for commercial use. 

It is **not** aimed at arduino lovers, or mbed fanatics as it does require vendor integration with vendor specific system and peripheral libraries, and thus requires a level of knowledge that is masked by these frameworks.

If you have never downloaded and read an MCU datasheet, then this project is not for you.

## But why not Arduino and mbed?

These frameworks are tragic, especially in terms of efficiency.  If you are serious about developing firmware commercially with arduino or mbed frameworks, then you should probably reconsider your career path, perhaps mcdonalds or something.

## Why QBS?

QBS accomplishes the same task as make and cmake, but is far more flexible and readable.

It allows conditional forward and backwards dependency attributes to be applied to the build.  For this project QBS is essential to organising build variant's compilation source and build environment per vendor MCU.  

Make and cmake just cant deliver this amount of flexibility, whilst retaining readability.

## Do i need Qt Creator to use this?

No, QBS is a cli tool akin to make & cmake.  Qt Creator does compliment QBS in order to display only the current compilation source in the tree.

