#!/bin/bash


lsbCode=`lsb_release -sc`
echo "lsbCode="$lsbCode

scriptDirectoryBase=`dirname ${0}`
scriptFileName=`basename ${0}`
cd ${scriptDirectoryBase}
fileOrigin=`readlink ${scriptFileName}`
if [ ! -z "$fileOrigin" ]; then
	relativeSourceDir=`dirname ${fileOrigin}`
	cd ${relativeSourceDir}
fi
scriptDirectory=`pwd`
echo scriptDirectory=$scriptDirectory

cd ..
repositoryRoot=`pwd`


git submodule sync --recursive
git submodule update --init --recursive

${repositoryRoot}/scripts/part_debian_per_host_using_apt.sh
