#!/bin/bash

# Check mode
if [ "$1" != "MODE=Debug" ] && [ "$1" != "MODE=Release" ]; then
	echo "Wrong expected mode: MODE=Debug|Release"
	exit 1
fi

rm -R Build/Products/${1:5}-iphoneos/libeng.framework/Headers

cp -R libeng/ Build/Products/${1:5}-iphoneos/libeng.framework/Headers
find Build/Products/${1:5}-iphoneos/libeng.framework/Headers/libeng -name *.cpp -delete

rm Build/Products/${1:5}-iphoneos/libeng.framework/Headers/Info.plist
rm Build/Products/${1:5}-iphoneos/libeng.framework/Headers/libeng/Render/Render.h
rm Build/Products/${1:5}-iphoneos/libeng.framework/Headers/libeng/Render/2D/Render2D.h
