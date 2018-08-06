# HoloSqape
A multiplatform Holochain app container that runs in the background and is accessible via a system tray icon based on Qt, QML and the C bindings of [holochain-rust](https://github.com/holochain/holochain-rust).

![](images/screenshots/system-tray.png)

## Dev Dependencies

[Rust](https://www.rust-lang.org/en-US/install.html)
[Qt](http://doc.qt.io/qt-5/index.html)
[cmake](https://cmake.org/install)

Note that on linux systems you may need to install [some dependencies](http://doc.qt.io/qt-5/linux.html) for successful build.

## Build
First you must get and build Holochain itself which is added here as a submodule:

```
git submodule init
git submodule update
```

Then you can build it just with:

```
cd holochain-rust
cargo build
```

given that you have rust installed.

Now either open and build holosqape.pro with [Qt Creator](https://en.wikipedia.org/wiki/Qt_Creator) (esp. if you want to build for Android or iOS) or run:

```
qmake
make
```

![](images/screenshots/mainwindow.png)
