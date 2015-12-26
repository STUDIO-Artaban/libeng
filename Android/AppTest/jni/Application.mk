APP_PLATFORM := android-13
APP_STL      := gnustl_static
APP_ABI      := armeabi #armeabi-v7a
APP_OPTIM    := debug #release
APP_CFLAGS   := -DDEBUG -UNDEBUG #-DNDEBUG -UDEBUG

APP_MODULES  := libogg libvorbis openal libeng AppTest
