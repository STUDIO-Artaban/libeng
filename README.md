# [libeng](https://github.com/STUDIO-Artaban/libeng.git)
**libeng** is a C++ Framework designed to create 2D/3D applications for Android &amp; iOS (only 2D has been implemented yet).

![MegaMan level](https://github.com/STUDIO-Artaban/libeng/blob/master/Android/Screenshots/LevelMega.png)

Check it directly by installing on your connected device the [AppTest.apk](https://github.com/STUDIO-Artaban/libeng/blob/master/Android/AppTest/bin/AppTest.apk) file using command below from the **Android/AppTest/bin** directory:

```bash
$ adb install AppTest.apk
```

Note: Before any installation do not forget to import the submodules with the **git** commands below.

```bash
$ git submodule init
$ git submodule update
```

# Android
To install and use **libeng** you have to create first an Android project with Eclipse from the **Android/Framework/libeng** project (Native support added), and to include shared and static library dependencies below:
* boost 1.53.0 or later (static library)
* openal 1.15.1 or later (shared library)
* libogg-vorbis (shared library)<br>
See [Android/Framework/Dependencies](https://github.com/STUDIO-Artaban/libeng/tree/master/Android/Framework/Dependencies) content.

Then...

Find on [Google Play](https://play.google.com/store) store some published applications that have been developed with **libeng**:
* [Crystal Ball](https://play.google.com/store/apps/details?id=com.studio.artaban.crystalballEN)
* [Flappy Witch's Pet](https://play.google.com/store/apps/details?id=com.studio.artaban.witchspet)
* [Kaleidoscope FREE](https://play.google.com/store/apps/details?id=com.studio.artaban.kaleidoscopeFREE)
* [Puzzle CAM](https://play.google.com/store/apps/details?id=com.studio.artaban.puzzlecam)
* [Watch Out!](https://play.google.com/store/apps/details?id=com.studio.artaban.watchout)
* [Matrix CAMERA](https://play.google.com/store/apps/details?id=com.studio.artaban.bullettime)
