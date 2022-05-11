

mkfile_path		=  $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir		=  $(shell dirname $(mkfile_path))
repoRootPathCppUtils	:= $(shell curDir=`pwd` && cd $(mkfile_dir)/../../.. && pwd && cd ${curDir})
ifndef repoRootPath
	repoRootPath	:= $(repoRootPathCppUtils)
endif
osSystem		:= $(shell uname)
ifeq ($(osSystem),Darwin)
	lsbCode		:= mac
	DEFAULT_CC	:= clang
	DEFAULT_CXX	:= clang++
	DEFAULT_LINK	:= clang++
else
	lsbCode		:= $(shell lsb_release -sc)
	DEFAULT_CC	:= gcc
	DEFAULT_CXX	:= g++
	DEFAULT_LINK	:= g++
endif

MAKEFLAGS=-j 2

#CXX=ccache g++
ifndef CC_IN_USE
	CC_IN_USE=$(DEFAULT_CC)
endif
ifndef CXX_IN_USE
	CXX_IN_USE=$(DEFAULT_CXX)
endif
ifdef LINK_IN_USE
	LINK = $(LINK_IN_USE)
else
	LINK = $(DEFAULT_LINK)
endif

# android configuration
# Android arch
ifndef ANDROID_ABI
	ANDROID_ABI	= armeabi-v7a
endif
# Android target depends on arch (-target option argument)
ifndef ANDROID_TARGET
	ifeq ($(ANDROID_ABI),armeabi-v7a)
		ANDROID_TARGET = armv7a-linux-androideabi21
	else ifeq ($(ANDROID_ABI),arm64-v8a)
		ANDROID_TARGET = aarch64-linux-android23
	else ifeq ($(ANDROID_ABI),x86)
		ANDROID_TARGET = i686-linux-android23
	else ifeq ($(ANDROID_ABI),x86_64)
		ANDROID_TARGET = x86_64-linux-android23
	endif
endif
# Android NDK root
ifndef ANDROID_NDK_BIN
	ANDROID_NDK_BIN = ~/Android/Sdk/ndk/21.3.6528147/toolchains/llvm/prebuilt/linux-x86_64/bin
endif
# Android cxx compiler
ifndef ANDROID_CXX
	ANDROID_CXX = $(ANDROID_NDK_BIN)/clang++
endif
# Android cc compiler
ifndef ANDROID_CC
	ANDROID_CC = $(ANDROID_NDK_BIN)/clang
endif
# Android ar
ifndef ANDROID_AR
	ANDROID_AR = $(ANDROID_NDK_BIN)/llvm-ar
endif
# android arch option (-marm for armeabi-v7a)
ifndef ANDROID_ARCH
	ifeq ($(ANDROID_ABI),armeabi-v7a)
		ANDROID_ARCH = -marm
	else ifeq ($(ANDROID_ABI),x86)
		ANDROID_ARCH = -mstackrealign
	endif
endif

#EMXX=env CCACHE_CPP2=1 ccache em++
EMXX=em++
EMCC=em

COMMON_FLAGS	+= -I$(repoRootPathCppUtils)/include

CPPFLAGS	+=  $(COMMON_FLAGS)
CFLAGS		+=  $(COMMON_FLAGS)

DEBUG_FLAGS_DEBUG=-O0 -g
DEBUG_FLAGS_RELEASE=-O3

ifdef CPPUTILS_DEBUG
	DEBUG_FLAGS=$(DEBUG_FLAGS_DEBUG) -DCPPUTILS_DEBUG
	Configuration=Debug
	nameExtension=d
else
	DEBUG_FLAGS=$(DEBUG_FLAGS_RELEASE)
	Configuration=Release
	nameExtension=
endif

EMFLAGS+=$(COMMON_FLAGS)
#EMFLAGS+=-isystem cpp/_system -Os
#EMFLAGS+=-s ASSERTIONS=1 -s ENVIRONMENT=web -s EXPORT_ES6=1
#EMFLAGS+=-s MODULARIZE=1 -s USE_ES6_IMPORT_META=0
EMFLAGS+=-s DISABLE_EXCEPTION_CATCHING=0
EMFLAGS+=-s ALLOW_MEMORY_GROWTH=1
#EMFLAGS+=-s USE_BOOST_HEADERS=1
#EMFLAGS+=-fexceptions

ANDROID_FLAGS += $(COMMON_FLAGS)
ANDROID_FLAGS += -target $(ANDROID_TARGET)
ANDROID_FLAGS += -fstack-protector-strong
ANDROID_FLAGS += -DANDROID
ANDROID_FLAGS += $(ANDROID_ARCH)

# host desktop
$(repoRootPath)/sys/$(lsbCode)/$(Configuration)/.objects/$(targetName)/%.cc.o : %.cc
	mkdir -p $(dir $@)
	$(CXX_IN_USE) -c $(CPPFLAGS) $(DEBUG_FLAGS) -o $@ $<

$(repoRootPath)/sys/$(lsbCode)/$(Configuration)/.objects/$(targetName)/%.cpp.o : %.cpp
	mkdir -p $(@D)
	$(CXX_IN_USE) -c $(CPPFLAGS) $(DEBUG_FLAGS) -o $@ $<
	
$(repoRootPath)/sys/$(lsbCode)/$(Configuration)/.objects/$(targetName)/%.c.o : %.c
	mkdir -p $(@D)
	$(CC_IN_USE) -c $(CFLAGS) $(DEBUG_FLAGS) -o $@ $<

# webassembly
$(repoRootPath)/sys/wasm/$(Configuration)/.objects/$(targetName)/%.cc.bc : %.cc
	mkdir -p $(@D)
	$(EMXX) -c $(EMFLAGS) -o $@ $<

$(repoRootPath)/sys/wasm/$(Configuration)/.objects/$(targetName)/%.cpp.bc : %.cpp
	mkdir -p $(@D)
	$(EMXX) -c $(EMFLAGS) -o $@ $<
	
$(repoRootPath)/sys/wasm/$(Configuration)/.objects/$(targetName)/%.c.bc : %.c
	mkdir -p $(@D)
	$(EMCC) -c $(EMFLAGS) -o $@ $<

# android
$(repoRootPath)/sys/android_$(ANDROID_ABI)/$(Configuration)/.objects/$(targetName)/%.cc.ao : %.cc
	mkdir -p $(@D)
	$(ANDROID_CXX) -c $(ANDROID_FLAGS) -o $@ $<

$(repoRootPath)/sys/android_$(ANDROID_ABI)/$(Configuration)/.objects/$(targetName)/%.cpp.ao : %.cpp
	mkdir -p $(@D)
	$(ANDROID_CXX) -c $(ANDROID_FLAGS) -o $@ $<
	
$(repoRootPath)/sys/android_$(ANDROID_ABI)/$(Configuration)/.objects/$(targetName)/%.c.ao : %.c
	mkdir -p $(@D)
	$(ANDROID_CC) -c $(ANDROID_FLAGS) -o $@ $<
