# [libeng](https://github.com/STUDIO-Artaban/libeng.git)
**libeng** is a C++ Framework designed to create 2D/3D applications for Android &amp; iOS (only 2D has been implemented yet).

![MegaMan level](https://github.com/STUDIO-Artaban/libeng/blob/master/Android/Screenshots/LevelMega.png)

Check it directly by installing on your device the [AppTest.apk](https://github.com/STUDIO-Artaban/libeng/blob/master/Android/AppTest/bin/AppTest.apk) file using command below from the **Android/AppTest/bin** directory:

```bash
$ adb install AppTest.apk
```

# Android
To install and use **libeng** you have to create first an Android Project with Eclipse from the **Android/Framework/libeng** project (Native support added). Do not forget to import the **libeng** core submodule folder with the **git** commands below:

```bash
$ git submodule init
$ git submodule update
```

And to include these shared and static library dependencies below before compiling it:
