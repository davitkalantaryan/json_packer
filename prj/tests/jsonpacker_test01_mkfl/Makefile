#
# file:			Makefile
# path:			prj/tests/jsonpacker_test01_mkfl/Makefile
# created on:		2022 May 12
# created by:		Davit Kalantaryan (davit.kalantaryan@gmail.com)
#
# To compile debug version call this way make JSONPACKER_DEBUG=1
#

PROJECT_NAME		=  jsonpacker_test01
mkfile_path		=  $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir		=  $(shell dirname $(mkfile_path))
repositoryRoot		:= $(shell curDir=`pwd` && cd $(mkfile_dir)/../../.. && pwd && cd ${curDir})
tlvSources		=  $(repositoryRoot)/contrib/TLV/cpp

osSystem		:= $(shell uname)
ifeq ($(osSystem),Darwin)
	lsbCode		:= mac
	CC		:= clang
	CXX		:= clang++
	LINK		:= clang++
else
	lsbCode		:= $(shell lsb_release -sc)
	CC		:= gcc
	CXX		:= g++
	LINK		:= g++
endif

ifndef QT_ROOT_DIR
	$(error no QT_RROT_DIR)
endif

ifdef JSONPACKER_DEBUG
	DEBUG_FLAGS=$(DEBUG_FLAGS_DEBUG) -DJSONPACKER_DEBUG
	Configuration=Debug
	CPPPARAMS += -O0 -g
	nameExtension=d
else
	DEBUG_FLAGS=$(DEBUG_FLAGS_RELEASE)
	Configuration=Release
	CPPPARAMS += -O3
	nameExtension=
endif

CPPPARAMS += -fPIC 
CPPPARAMS += -I$(QT_ROOT_DIR)/include/QtCore
CPPPARAMS += -I$(QT_ROOT_DIR)/include
CPPPARAMS += -I$(tlvSources)
CPPPARAMS += -I$(repositoryRoot)/include

LFLAGS		+=  -L$(QT_ROOT_DIR)/lib
LFLAGS		+=  -lQt5Core

SOURCES_BASE_DIR	=  $(repositoryRoot)
TARGET_DIR		=  $(repositoryRoot)/sys/$(lsbCode)/$(Configuration)/test
OBJECT_FILES_DIR	=  $(repositoryRoot)/sys/$(lsbCode)/$(Configuration)/.other/objects/$(PROJECT_NAME)

OBJECTS = \
	$(OBJECT_FILES_DIR)/contrib/TLV/cpp/tlv.o		\
	$(OBJECT_FILES_DIR)/contrib/TLV/cpp/tlv_box.o		\
	$(OBJECT_FILES_DIR)/src/core/jsonpacker_packer.o	\
	$(OBJECT_FILES_DIR)/src/tests/main_jsonpacker_test01.o
	
default: $(PROJECT_NAME)


$(PROJECT_NAME): $(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	@$(LINK) $^ $(LFLAGS) -o $(TARGET_DIR)/$@


$(OBJECT_FILES_DIR)/%.o: $(SOURCES_BASE_DIR)/%.cpp
	@echo $(CPPPARAMS)
	@mkdir -p $(@D)
	@$(CXX) $(CPPPARAMS) -o $@ -c $<
	
.PHONY: clean
clean:
	@rm -rf $(OBJECT_FILES_DIR)
	@rm -f  $(TARGET_DIR)/$(PROJECT_NAME)
	@echo "  " cleaning complete !!!
