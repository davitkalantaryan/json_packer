#
# file:			googletest_getter.pro
# path:			prj/tests/googletest_mult/googletest_getter.pro
# created on:	2021 Nov 03
# created by:	Davit Kalantaryan
#

repositoryRoot = $${PWD}/../../..
artifactRoot = $${repositoryRoot}
include ( "$${repositoryRoot}/prj/common/common_qt/sys_common.pri" )

# we reevaluate this
DESTDIR = "$${artifactRoot}/$${SYSTEM_PATH}/test"

tlvSources = $${repositoryRoot}/contrib/TLV/cpp

INCLUDEPATH += "$${tlvSources}"
INCLUDEPATH += "$${repositoryRoot}/include"

QT -= gui
#QT -= core
QT -= widgets
#CONFIG -= qt
CONFIG += console

win32{
} else {
	LIBS += -pthread
}

SOURCES += "$${repositoryRoot}/src/tests/main_jsonpacker_test01.cpp"
SOURCES += $$files($${repositoryRoot}/src/core/*.cpp,true)
SOURCES += "$${tlvSources}/tlv.cpp"
SOURCES += "$${tlvSources}/tlv_box.cpp"

HEADERS += $$files($${tlvSources}/*.h,true)
HEADERS += $$files($${repositoryRoot}/include/*.h,true)
HEADERS += $$files($${repositoryRoot}/include/*.hpp,true)

OTHER_FILES +=	\
	"$${PWD}/../jsonpacker_test01_mkfl/Makefile"
