# [libeng](https://github.com/STUDIO-Artaban/libeng.git)
**libeng** is a C++ Framework designed to create 2D/3D applications for Android &amp; iOS (only 2D has been implemented yet).

![MegaMan level](https://github.com/STUDIO-Artaban/libeng/blob/master/Android/Screenshots/LevelMega.png)

Check it directly by installing on your connected device the [AppTest.apk](https://github.com/STUDIO-Artaban/libeng/blob/master/Android/AppTest/bin/AppTest.apk) file using command below from the **Android/AppTest/bin** directory:

```bash
$ adb install AppTest.apk
```

**Note:** Before any installation do not forget to import the submodules with the **git** commands below.

```bash
$ cd libeng/
$ git submodule init
$ git submodule update
```

# Android
To install and use **libeng** you have to create first an Android project with **Eclipse** from the **Android/Framework/libeng** project (**Native Support** added), and to include shared and static library dependencies below:
* boost 1.53.0 or later (static library)
* openal 1.15.1 or later (shared library)
* libogg-vorbis (shared library)<br>
...adding [dependency](https://github.com/STUDIO-Artaban/libeng/tree/master/Android/Framework/Dependencies) files in each library folder (see [libeng/jni/Android.mk](https://github.com/STUDIO-Artaban/libeng/blob/master/Android/Framework/libeng/jni/Android.mk) file at lines 49 to 51).

After having compiling the **libeng** project successfully you can now create your own Android Application Project then:
* Remove any Java files from your **src** folder
* Copy all [files](https://github.com/STUDIO-Artaban/libeng/tree/master/Android/AppTest/src/com/studio/artaban/apptest) into a new folder tree according your project
* Replace from Java files any `com.studio.artaban.apptest` package references with your own package name
* Replace the **PROJECT_NAME_LIB** definition with your project name from the [EngData.java](https://github.com/STUDIO-Artaban/libeng/blob/master/Android/AppTest/src/com/studio/artaban/apptest/data/EngData.java) file at line 21, then update it entirely according your project specifications (textures, sounds, advertising, etc.)
* Add **google-play-services_lib**, **FacebookSDK** and **twitter4j-core** references to your project, or comment any call to these references from [EngFacebook.java](https://github.com/STUDIO-Artaban/libeng/blob/master/Android/AppTest/src/com/studio/artaban/apptest/social/EngFacebook.java), [EngGoogle.java](https://github.com/STUDIO-Artaban/libeng/blob/master/Android/AppTest/src/com/studio/artaban/apptest/social/EngGoogle.java) and [EngTwitter.java](https://github.com/STUDIO-Artaban/libeng/blob/master/Android/AppTest/src/com/studio/artaban/apptest/social/EngTwitter.java) files if you don't need it
* **Add Native Support** to your project
* Replace all files from your **jni** folder with [AppTest/jni](https://github.com/STUDIO-Artaban/libeng/tree/master/Android/AppTest/jni) content
* Update [JNI.cpp](https://github.com/STUDIO-Artaban/libeng/blob/master/Android/AppTest/jni/JNI.cpp) file to rename all **JNIEXPORT** function declaration according your project (replace any `com_studio_artaban_apptest` string with your package name)
* In the same way, update your [jni/Sources/Main.h](https://github.com/v-pascal/AppTest_Sources/blob/master/Main.h) accordingly (at lines 36 and 38)

From now, you have to remove and update folders and files using the C++ **libeng** objects, started with your application object which inherits from **Game2D** (i.e [jni/Sources/AppTest.h](https://github.com/v-pascal/AppTest_Sources/blob/master/AppTest.h) &amp; [jni/Sources/AppTest.cpp](https://github.com/v-pascal/AppTest_Sources/blob/master/AppTest.cpp) files), by using the **eng** namespace as below:

```c++
using namespace eng;
```

Do not forget to update your [jni/Android.mk](https://github.com/STUDIO-Artaban/libeng/blob/master/Android/AppTest/jni/Android.mk) and [jni/Application.mk](https://github.com/STUDIO-Artaban/libeng/blob/master/Android/AppTest/jni/Application.mk) files depending on your needs.

Find on [Google Play](https://play.google.com/store) store some published applications that have been developed with **libeng**:
* [Crystal Ball](https://play.google.com/store/apps/details?id=com.studio.artaban.crystalballEN)
* [Flappy Witch's Pet](https://play.google.com/store/apps/details?id=com.studio.artaban.witchspet)
* [Kaleidoscope FREE](https://play.google.com/store/apps/details?id=com.studio.artaban.kaleidoscopeFREE)
* [Puzzle CAM](https://play.google.com/store/apps/details?id=com.studio.artaban.puzzlecam)
* [Watch Out!](https://play.google.com/store/apps/details?id=com.studio.artaban.watchout)
* [Matrix CAMERA](https://play.google.com/store/apps/details?id=com.studio.artaban.bullettime)

# iOS
To install and use **libeng** you have to create the Framework first. Open the `iOS/Framework/libeng/libeng.xcodeproj` project file with **Xcode** then install and replace Frameworks dependencies below (from the **Build Phases** project property):
* Ogg.framework
* Vorbis.framework
* boost.framework

Build the **libeng** project then edit the [iOS/Framework/libeng/framework.sh](https://github.com/STUDIO-Artaban/libeng/blob/master/iOS/Framework/libeng/framework.sh) file to modify default build path (i.e **Build/Products/**), with your build path, and launch the following command to add **Headers** declaration to the `libeng.framework`:

```bash
$ ./framework.sh MODE=Debug
```

...replace **MODE=Debug** with **MODE=Release** according your build configuration.

Create now your own **iOS** project application then:
* Open the `iOS/AppTest/AppTest.xcodeproj` project file
* Copy all of the **AppTest** properties into your project properties (e.g disable the **ARC** in the **Build Settings** property)
* Add all of Frameworks dependencies such as defined in The **AppTest** application, not forgetting the `libeng.framework` dependency
