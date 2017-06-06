# build/os-auto.mak.  Generated from os-auto.mak.in by configure.

export OS_CFLAGS   := $(CC_DEF)PJ_AUTOCONF=1 -O2 -Wno-unused-label -miphoneos-version-min=7.0 -DPJ_SDK_NAME="\"iPhoneOS10.3.sdk\"" -arch arm64 -isysroot /Applications/XCode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS10.3.sdk -DPJ_IS_BIG_ENDIAN=0 -DPJ_IS_LITTLE_ENDIAN=1

export OS_CXXFLAGS := $(CC_DEF)PJ_AUTOCONF=1 -O2 -Wno-unused-label -miphoneos-version-min=7.0 -DPJ_SDK_NAME="\"iPhoneOS10.3.sdk\"" -arch arm64 -isysroot /Applications/XCode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS10.3.sdk 

export OS_LDFLAGS  := -O2 -miphoneos-version-min=7.0 -arch arm64 -isysroot /Applications/XCode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS10.3.sdk -framework AudioToolbox -framework Foundation -lm -lpthread  -framework CoreAudio -framework CoreFoundation -framework AudioToolbox -framework CFNetwork -framework UIKit -framework UIKit -framework AVFoundation -framework CoreGraphics -framework QuartzCore -framework CoreVideo -framework CoreMedia -framework OpenGLES

export OS_SOURCES  := 


