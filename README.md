# HueLib

This is a C++ Qt library to control Philips Hue lights.

1. [Installation](#installation)
2. [Usage](#usage)
3. [Creating a HueBridge object](#create)
4. [Discovering HueLights and HueGroups](#discover)
5. [Controlling HueLights and HueGroups](#control)
6. [Keeping HueLights and HueGroups synchronized](#synchronization)
7. [Some implementation details](#implementation)

<a name="installation"></a>
## 1. Installation

### Download Qt and Qt Creator
This library is made to be used with Qt Creator. Qt and Qt Creator can be downloaded from [here](https://www.qt.io/download).

### Build the project to generate the static library file
Clone or download this repository and open `HueLib.pro`. Build the project. This should create a build folder with a static library file (`libHueLib.a` on Mac).

<a name="usage"></a>
## 2. Usage
To use the library, create a new Qt project (e.g. Qt Widgets Application or Qt Console Application), right click on the project folder and click `Add Library...`. Choose `External Library`, navigate to the `HueLib` **build** folder and select the static library file (`libHueLib.a` on Mac) generated in the previous step.

Copy the `HueLib` folder into your project folder. In Qt Creator, right click on the project folder and click `Add Existing Directory...` and select `HueLib`.

In your `main.cpp` file, add the following include:
```c++
#include <HueLib/huelib.h>
```
If you try compiling now, you will probably get a lot of errors about a missing `QNetworkAccessManager` file. This is because we must let Qt know that we will be needing access to the network classes. Open your `.pro` file and append
```qmake
QT += network
```
Now you should be good to go.

<a name="create"></a>
## 3. Creating a HueBridge object
To get access to the Hue network, you must first create a HueBridge object. To create this, you will need two things: the IP address of the bridge, and an authorized username. The easiest way to obtain these things is to head over to the [Philips Hue Get Started Page](https://developers.meethue.com/develop/get-started-2/) and follow their steps.

Once you have the IP address and username, create a `HueBridge` object like this:
```c++
#include <QCoreApplication>

#include <HueLib/huelib.h>
#include <QtDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HueBridge* bridge = new HueBridge("10.0.1.14", "1028d66426293e821ecfd9ef1a0731df");

    HueBridge::ConnectionStatus status;
    bridge->testConnection(status);

    if (status == HueBridge::ConnectionStatus::Success)
        qDebug() << "Connected to bridge.";
    else
        qDebug() << "No connection to bridge.";

    return a.exec();
}
```
If `status` does not return with `Success`, verify that your IP address and username is correct.

**Note:** The `HueBridge` class needs an instance of `QNetworkAccessManager` to work. Qt recommends only creating one instance of `QNetworkAccessManager` in an application, so if you need to use the network access manager for another purpose, you can instead create a `QNetworkAccessManager` somwhere else and initiate the `HueBridge` object with a pointer to the `QNetworkAccessManager` as a third optional argument:
```c++
QNetworkAccessManager* nam = new QNetworkAccessManager();
HueBridge* bridge = new HueBridge("10.0.1.14", "1028d66426293e821ecfd9ef1a0731df", nam);
```
<a name="discover"></a>
## 4. Discovering HueLights and HueGroups
The library gives you access to individual lights (`HueLight` objects), and groups of lights like e.g. rooms (`HueGroup` objects). The library currently has no functionality to set up new lights or groups - this is more easily done with the Philips Hue smartphone app.

The following code discovers lights and groups on the network and creates an object for each one on the heap:

```c++
HueBridge* bridge = new HueBridge("10.0.1.14", "1028d66426293e821ecfd9ef1a0731df");

HueLightList lights = HueLight::discoverLights(bridge);
HueGroupList groups = HueGroup::discoverGroups(bridge);

qDebug() << "Lights found: " << lights.size();
qDebug() << "Groups found: " << groups.size();
```

If everything worked correctly, you should see the number of lights you have connected to your bridge, and the number of groups you have created printed to the debug console.

The return types of `HueLight::discoverLights()` and `HueGroup::discoverGroups()` are respectively `HueLightList` and `HueGroupList`. These types can be traversed just like a normal C++ container e.g. using a range-based for loop:
```c++
for (auto group : groups) {
    qDebug() << group.name().getName();
}
```
<a name="control"></a>
## 5. Controlling HueLights and HueGroups
Once you have discovered the lights and/or groups on the network, you can manipulate their states by calling set-functions directly on the objects. You can fetch a specific light/group through its ID number or name using the functions `fetch(int ID)` or `fetch(QString name)` respectively. This will give you an `std::shared_ptr<HueLight>` or `std::shared_ptr<HueGroup>`. You can also use `fetchRaw(int ID)` or `fetchRaw(QString name)` to get a raw pointer. The following example shows how to turn off all the lights in a group labled "Living room" and set the brightness in "Bedroom" to 50:
```c++
HueGroup* livingroom = groups.fetchRaw("Living Room");
if (livingroom->isValid())
    livingroom->turnOn();

HueGroup* bedroom = groups.fetchRaw("Bedroom");
if (bedroom->isValid())
    bedroom->setBrightness(50);
}
```
The names passed to `fetch(QString name)` and `fetchRaw(QString name)` must clearly match the name given to that group, and is case sensitive.

Here is another example showing how to address individual lights. This code will fetch lights with ID 1, 2 and 3 and set their color to to red, green and blue. If those bulbs are installed in the same lamp, it creates a kind of rainbow effect:
```c++
HueLight* light1 = m_lights.fetchRaw(1);
HueLight* light2 = m_lights.fetchRaw(2);
HueLight* light3 = m_lights.fetchRaw(3);

light1->setHue(65535); // red
light2->setHue(21845); // green
light3->setHue(43690); // blue
```
The following functions can be called on `HueLight` objects and `HueGroup` objects to change their properties:
```c++
bool turnOn();
bool turnOff();
bool setHue(int hue);
bool setSaturation(int saturation);
bool setBrightness(int brightness);
bool setColorTemp(int colorTemp);
bool setXY(double x, double y);
bool setAlert(HueAlert alert);
bool setEffect(HueEffect effect);
```
All functions return a boolean indicating if the change was successful (`true`) or unsuccessful (`false`).

For
```c++
bool setAlert(HueAlert alert)
```
you have the option to use the following arguments:
```c++
HueAlert::NoAlert
HueAlert::BreatheSingle
HueAlert::Breathe15Sec
````

and for
```c++
bool setEffect(HueEffect effect)
```
you have the option to use the following arguments:
```c++
HueEffect::NoEffect
HueEffect::ColorLoop
````
<a name="synchronization"></a>
## 6. Keeping HueLights and HueGroups synchronized
You may have a scenario where multiple devices can change the state of your lights, e.g. using a Hue dimmer switch or the Hue smartphone app. In this case, you may need to synchronize the `HueLight`  and `HueGroup`  objects in your program. To synchronize an object, call its `synchronize()` function, e.g.
```c++
for (auto light : lights)
    light.synchronize();
````
Calling `synchronize()` on an object will fetch its current state stored on the bridge. This can be somewhat time consuming, especially when many objects are synchronized at the same time. Synchronizing with the bridge too often leads to heavy traffic and can make the bridge less responsive.

`HueLight` and `HueGroup` objects can also be set up to synchronize with the bridge periodically. To enable periodic synchronization of an object, call its `enablePeriodicSync(bool periodicSyncOn)` with `periodicSyncOn = true/false` to enable/disable, e.g.
```c++
for (auto light : lights)
    light.enablePeriodicSync(true);
````
The default synchronization interval is 10 seconds. The interval can be changed in the following way:
```c++
HueSynchronizer::setSyncIntervalMilliSec(5000);
````
As explained above, this should not be done too often.

<a name="implementation"></a>
## 7. Some implementation details
The library uses a synchronous request/reply pattern to keep the program state synchronized with the bridge. This means that a function call to e.g. `turnOn()` will not return until a request has been sent to the bridge and a reply has been received, or a timer has expired. The timeout period can be set by redefining `HUE_REQUEST_TIMEOUT_MILLISECONDS` before including `huelib.h`. The default value is 200 ms.

Note that according to the [Hue System Performance guidelines](https://developers.meethue.com/develop/application-design-guidance/hue-system-performance/), the maximum throughput of the Hue bridge is about 10 light commands per second and about 1 group command per second. Exceeding this limit will queue any further requests which can lead to long response time and commands being dropped if the queue exceeds a certain length. In the library, a sleep timer will start running after a command has been sendt to the bridge that will delay the next command by a certain duration. The sleep duration can be changed by redefining `HUE_BRIDGE_SLEEP_MILLISECONDS` before including `huelib.h`. The default value is 50 ms which has been tested to work well on a network with 15 Hue lights divided into 5 groups. Reducing the sleep duration will increase the response time for short bursts of commands, but can lead to more frequent requests being dropped or returning with an error.


