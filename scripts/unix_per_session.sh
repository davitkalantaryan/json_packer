

if [ $# -eq 0 ]; then
	if [ $0 != "/bin/bash" -a $0 != "bash" -a $0 != "-bash" ]; then
		sourcePath_local=`bash -c "source $0 1"`
	else
		sourcePath_local=${BASH_SOURCE[0]}
	fi
else
	if [ $# -eq 1 ]; then
		echo ${BASH_SOURCE[0]}
		return 0
	else
		sourcePath_local=${1}
	fi
fi


makeMainJob (){

	local sourcePath=${sourcePath_local}
	unset sourcePath_local
	echo sourcePath=$sourcePath

	local currentDirectory=`pwd`
	echo currentDirectory=$currentDirectory

	if [[ "$(uname)" == "Darwin" ]]; then
		# Do something under Mac OS X platform
		lsbCode=mac
		Qt_patform=clang_64
		Qt_deploy=macdeployqt
	elif [[ "$(expr substr $(uname -s) 1 5)" == "Linux" ]]; then
		# Do something under GNU/Linux platform
		lsbCode=`lsb_release -sc`
		Qt_patform=gcc_64
		Qt_deploy=linuxdeployqt
	#elif [[ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]]; then
	#	# Do something under 32 bits Windows NT platform
	#elif [[ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]]; then
	#	# Do something under 64 bits Windows NT platform
	#else
	fi
	echo "lsbCode="$lsbCode

	# calculate repository root
	#local scriptFileFullPath=`readlink -f ${sourcePath}`
	#local scriptDirectory=`dirname ${scriptFileFullPath}`
	#cd ${scriptDirectory}/..
	#local repositoryRoot=`pwd`
	#echo "repositoryRoot="$repositoryRoot
	
	# in mac os above short calculation will not work
	# also `realpath` utilit is missing in mac
	
	local scriptDirectory=`dirname "${sourcePath}"`
	local scriptFileName=`basename "${sourcePath}"`
	cd "${scriptDirectory}"
	local fileOrigin=`readlink "${scriptFileName}"` || :
	while [ ! -z "${fileOrigin}" ]
	do
		local scriptDirectory=`dirname "${fileOrigin}"`
		local scriptFileName=`basename "${fileOrigin}"`
		cd "${scriptDirectory}"
		local fileOrigin=`readlink "${scriptFileName}"`  || :
	done
	cd ..
	repositoryRoot=`pwd`
	echo repositoryRoot=$repositoryRoot

	cd ~
	local currentUserHome=`pwd`
	echo currentUserHome=$currentUserHome

	# todo : check this
	#local nosetestsFile=`find ~/.local/bin -name nosetests`
	#echo nosetestsFile=$nosetestsFile
	#local nosetestsFileDir_local=
	#if [ ! -z "$nosetestsFile" ]; then
	#	local nosetestsFileDirBese=`dirname ${nosetestsFile}`
	#	echo nosetestsFileDirBese=${nosetestsFileDirBese}
	#	cd ${nosetestsFileDirBese}
	#	local nosetestsFileDir_local=`pwd`
	#fi
	#echo nosetestsFileDir_local=${nosetestsFileDir_local}

	case "$PATH" in
		${repositoryRoot}/sys/$lsbCode/bin:${nosetestsFileDir_local}:${currentUserHome}/bin:* )
			echo "PATH for protobuf and nosetests and antlr has been already set"
			;;
		* )
			export PATH=${repositoryRoot}/sys/$lsbCode/bin:${nosetestsFileDir_local}:${currentUserHome}/bin:$PATH
			;;
	esac

	#case "$CPLUS_INCLUDE_PATH" in
	#	${repositoryRoot}/sys/$lsbCode/include:* )
	#		echo "CPLUS_INCLUDE_PATH for protobuf has been already set"
	#		;;
	#	* )
	#		export CPLUS_INCLUDE_PATH=${repositoryRoot}/sys/$lsbCode/include:$CPLUS_INCLUDE_PATH
	#		;;
	#esac
	#
	#case "$LIBRARY_PATH" in
	#	${repositoryRoot}/sys/$lsbCode/lib:* )
	#		echo "LIBRARY_PATH for protobuf has been already set"
	#		;;
	#	* )
	#		export LIBRARY_PATH=${repositoryRoot}/sys/$lsbCode/lib:$LIBRARY_PATH
	#		;;
	#esac

	case "$LD_LIBRARY_PATH" in
		${repositoryRoot}/sys/$lsbCode/${configuration}/lib:* )
			echo "LD_LIBRARY_PATH for protobuf has been already set"
			;;
		* )
			export LD_LIBRARY_PATH=${repositoryRoot}/sys/$lsbCode/${configuration}/lib:$LD_LIBRARY_PATH
			;;
	esac

        if [ -z "$EMSDK_FOR_SSS_SET" ]
        then
              source ${repositoryRoot}/contrib/emsdk/emsdk_env.sh
        	  export EMSDK_FOR_SSS_SET=1
        else
              echo "emscriptian already inited"
        fi

	cd ${currentDirectory}
}


makeMainJob
return 0
