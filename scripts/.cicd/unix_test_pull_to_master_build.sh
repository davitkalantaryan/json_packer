#!/bin/bash

# script to prepare developer host, to work with the code on this repo

# https://intoli.com/blog/exit-on-errors-in-bash-scripts/
# exit when any command fails
set -e

# keep track of the last executed command
trap 'last_command=$current_command; current_command=$BASH_COMMAND' DEBUG
# echo an error message before exiting
#trap 'echo "\"${last_command}\" command filed with exit code $?."' EXIT

#scriptFileFullPath=`readlink -f ${0}`
#scriptDirectory=`dirname ${scriptFileFullPath}`
#cd ${scriptDirectory}/../..
#repositoryRoot=`pwd`

# in mac os above short calculation will not work
# also `realpath` utilit is missing in mac

scriptDirectory=`dirname "${0}"`
scriptFileName=`basename "${0}"`
cd "${scriptDirectory}"
fileOrigin=`readlink "${scriptFileName}"` || :
while [ ! -z "${fileOrigin}" ]
do
	scriptDirectory=`dirname "${fileOrigin}"`
	scriptFileName=`basename "${fileOrigin}"`
	cd "${scriptDirectory}"
	fileOrigin=`readlink "${scriptFileName}"`  || :
done
cd ../..
repositoryRoot=`pwd`
echo repositoryRoot=$repositoryRoot

configuration=Release
source ${repositoryRoot}/scripts/unix_per_session.sh ${repositoryRoot}/scripts/unix_per_session.sh 1
configuration=Debug
source ${repositoryRoot}/scripts/unix_per_session.sh ${repositoryRoot}/scripts/unix_per_session.sh 1


# thanks to https://stackoverflow.com/questions/3466166/how-to-check-if-running-in-cygwin-mac-or-linux
if [[ "$(uname)" == "Darwin" ]]; then
	# Do something under Mac OS X platform
	lsbCode=mac
elif [[ "$(expr substr $(uname -s) 1 5)" == "Linux" ]]; then
	# Do something under GNU/Linux platform
	lsbCode=`lsb_release -sc`
#elif [[ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]]; then
#	# Do something under 32 bits Windows NT platform
#elif [[ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]]; then
#	# Do something under 64 bits Windows NT platform
#else
fi

#cd ${repositoryRoot}/prj/core/libdevsheet_mkfl
##make -f unix.Makefile all $@
#unset CPPUTILS_DEBUG
#make -f unix.Makefile all
#make -f unix.Makefile all CPPUTILS_DEBUG=1 # this should be used for debug build
#
#
cd ${repositoryRoot}/prj/tests/googletest_mult
unset CPPUTILS_DEBUG
#make -f unix.Makefile $@
make -f unix.Makefile
make -f unix.Makefile CPPUTILS_DEBUG=1 # this should be used for debug build
