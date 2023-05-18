# orianna

> Connect-four, five, and fun variations with AI and network multiplayer support 🎲

![img](./resources/screenshot.png)

## Features

...


## Development

Requirements: gcc/g++10, python3.4, and cmake 3.14.

This project works in the latest Visual Studio preview (with C++20 support).

* Install dependencies (debian/ubuntu)

> `sudo apt-get install libx11-dev xorg-dev doctest-dev libglu1-mesa-dev python-dev python3-dev`

* Install dependencies (windows, mingw/msys UCTR64)

> `pacman -Sy base-devel python3 mingw-w64-x86_64-doctest mingw-w64-x86_64-mesa mingw-w64-ucrt-x86_64-cmake`

Note that in mingw, the default cmake generator is `ninja`. Replace the `make` commands below in the build section with just `ninja`

* Install git submodules

> `git submodule update --init --recursive`


### Building

Build by making a build directory (i.e. `build/`), run `cmake` in that dir, and then use `make` to build the desired target.

Example:

``` bash
> mkdir build && cd build
> cmake .. -DCMAKE_BUILD_TYPE=[Debug | Coverage | Release]
> make
> ./main
> make test      # Makes and runs the tests.
```

**Note**: Copy the root `resources` folder to the directory of the built binary

---

![img](https://cdn.vox-cdn.com/thumbor/j-iyo0MUGZMWtbr3RxBOuSwq8DY=/0x0:1200x674/920x0/filters:focal(0x0:1200x674):format(webp):no_upscale()/cdn.vox-cdn.com/uploads/chorus_asset/file/13264983/DpZaNreW4AAkjwO.jpg)