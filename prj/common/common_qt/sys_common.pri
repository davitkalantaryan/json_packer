#
# name:			sys_common.pri
# path:			${repositoryRoot}/prj/common/common_qt/sys_common.pri
# created on:		2017 Feb 12
# created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)  
# usage:		Use this qt include file to calculate some platform specific stuff
#

#QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter


STATIC_LIB_EXTENSION	= a
LIB_PREFIX		= lib
TARGET_PATH_EXTRA	=

isEmpty( repositoryRoot ) {
	repositoryRoot = $${PWD}/../../..
}


isEmpty( cpputilsRepoRoot ) {
	cpputilsRepoRoot = $${PWD}/../../..
}

isEmpty( TARGET_PATH ) {
	contains( TEMPLATE, lib ) {
	    TARGET_PATH=lib
	} else {
	    TARGET_PATH=bin
	}
}


isEmpty(artifactRoot) {
    artifactRoot = $${repositoryRoot}
}


win32 {
	STATIC_LIB_EXTENSION = lib
	LIB_PREFIX =
    contains(QMAKE_TARGET.arch, x86_64) {
        ## Windows x64 (64bit) specific build here
        CODENAME = win_x64
        SYSTEM_PATH = sys/win_x64

    } else {
        ## Windows x86 (32bit) specific build here
        CODENAME = win_x86
        SYSTEM_PATH = sys/win_x86

    }

} else:mac {
    CODENAME = mac
    SYSTEM_PATH = sys/mac
} else:android {
    # ANDROID_TARGET_ARCH values 1."armeabi-v7a", 2."arm64-v8a", 3."x86", 4."x86_64"
    # x86 and x86_64 are simulators
    message("------------ ANDROID_TARGET_ARCH=$$ANDROID_TARGET_ARCH")
    CODENAME = android_$${ANDROID_TARGET_ARCH}
    SYSTEM_PATH = sys/android_$${ANDROID_TARGET_ARCH}

    #contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
	##ANDROID_EXTRA_LIBS += $$PWD/../platform/android/openssl/armeabi-v7a/libcrypto.so
	##ANDROID_EXTRA_LIBS += $$PWD/../platform/android/openssl/armeabi-v7a/libssl.so
	#CODENAME = android_armeabi-v7a
	#SYSTEM_PATH = sys/android_armeabi-v7a
    #}

} else:ios {
    CODENAME = ios
    SYSTEM_PATH = sys/$$CODENAME
} else:wasm {
	STATIC_LIB_EXTENSION	= ba
    CODENAME = wasm
    SYSTEM_PATH = sys/$$CODENAME
	TARGET_PATH_EXTRA = /$${TARGET}
} else {
    DEFINES += LINUX
    CODENAME = $$system(lsb_release -sc)
	message("------------ Linux. Codename:$${CODENAME}")
    SYSTEM_PATH = sys/$$CODENAME
}

CONFIGURATION=Profile
nameExtension=

CONFIG(debug, debug|release) {
	nameExtension=d
	CONFIGURATION=Debug
	DEFINES += CPPUTILS_DEBUG
} else:CONFIG(release, debug|release) {
	CONFIGURATION=Release
}

message("!!! sys_common.pri: SYSTEM_PATH/CONFIGURATION=$${artifactRoot}/$${SYSTEM_PATH}/$${CONFIGURATION}")

# Debug:DESTDIR = debug1
SYSTEM_PATH=$$SYSTEM_PATH/$$CONFIGURATION
MAKEFILE = Makefile.$${TARGET}.$${CODENAME}.$$CONFIGURATION
DESTDIR     = $${artifactRoot}/$${SYSTEM_PATH}/$${TARGET_PATH}$${TARGET_PATH_EXTRA}
OBJECTS_DIR = $${artifactRoot}/$${SYSTEM_PATH}/.other/objects/$${TARGET}
MOC_DIR     = $${artifactRoot}/$${SYSTEM_PATH}/.other/mocs/$${TARGET}
UI_DIR      = $${artifactRoot}/$${SYSTEM_PATH}/.other/uics/$${TARGET}
