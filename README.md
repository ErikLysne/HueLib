# HueLib

This is a C++ Qt library to control Philips Hue lights.

## Installation

### Download Qt and Qt Creator
This library is made to be used with Qt Creator. Qt and Qt Creator can be downloaded from [here](https://www.qt.io/download).

### Build the project to generate the static library file
Clone or download this repository and open `HueLib.pro`. Build the project. This should create a build folder with a static library file (`libHueLib.a` on Mac).

## Usage
To use the library, create a new Qt project (e.g. Qt Widgets Application or Qt Console Application), right click on the project folder and click `Add Library...`. Choose `External Library`, navigate to the `HueLib` **build** folder and select the static library file (`libHueLib.a` on Mac) generated in the previous step.

Copy the `HueLib` folder into your project folder. In Qt Creator, right click on the project folder and click `Add Existing Directory...` and select `HueLib`.

In your `main.cpp` file, add the following include:
```c++
#include <HueLib/huelib.h>
```
If you try compiling now, you will probably get a lot of errors about a missing `QNetworkAccessManager` file. This is because we must let Qt know that we will be needing access to the build in network classes. Open your `.pro` file and append `QT += network`. Now you should be good to go.

## Creating a HueBridge object
The first thing you must do
