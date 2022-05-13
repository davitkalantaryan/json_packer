#
# file:			Makefile
# path:			Makefile
# created on:		2022 May 13
# created by:		Davit Kalantaryan (davit.kalantaryan@gmail.com)
#
# To compile debug version call this way make JSONPACKER_DEBUG=1
#

mkfile_path		=  $(abspath $(lastword $(MAKEFILE_LIST)))
repositoryRoot		=  $(shell dirname $(mkfile_path))


default: packer

packer:
	make -C $(repositoryRoot)/prj/tests/jsonpacker_test01_mkfl
