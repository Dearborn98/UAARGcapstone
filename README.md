# UAARG Capstone: Multicamera Image Control

@author Ryan Sandoval
@author Ricky Au
@author Andrew Dearborn
@author Imtisal Ahmad

## Description

This library uses OpenCV to manage a camera array and provide flexible processing 
functionality through a processing pipeline.

## Setup Instructions

Install the following apt packages:
```bash
sudo apt install libopencv-dev libboost-all-dev meson doxygen
```
Note: Tested on Ubuntu 20.04. These packages may not exist on other distributions.

Afterwards, generate the build directory by running:

```bash
meson build
```

To enable the demo application to build, use this instead:

```bash
meson build -Denable_demo
```

## Generate docs

Run the following commands:

```bash
cd build && ninja docs
```

HTML documentation will be in `docs/html/`

## Running the Tests

Run the following commands:

```bash
cd build && ninja test
```

## How to use

See the example in @ref demo/main.cpp.

The primary API class for this project is @ref CameraController.