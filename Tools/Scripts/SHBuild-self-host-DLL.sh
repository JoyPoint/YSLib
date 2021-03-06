#!/usr/bin/env bash
# (C) 2014-2016 FrankHB.
# Test script for self-hosting SHBuild.

set -e
SHBuild_ToolDir=$(cd `dirname "$0"`; pwd)
: ${SHBuild_BaseDir:="$SHBuild_ToolDir/../SHBuild"}
. $SHBuild_ToolDir/SHBuild-bootstrap.sh
SHBuild_CheckHostPlatform
SHBuild_AssertNonempty SHBuild_Host_Platform

# XXX: Depends on DLLs in '/usr/lib'.
export LIBS='-L/usr/lib -lYFramework -lYBase'
CXXFLAGS="$CXXFLAGS -fwhole-program -DYF_DLL -DYB_DLL"
export LDFLAGS="${LDFLAGS/-Wl,--dn/}"

SHBuild_BuildDir="$SHBuild_ToolDir/\
../../build/$SHBuild_Host_Platform/.shbuild-dll"

SHBuild_Pushd
cd $SHBuild_BaseDir

$SHBuild_BaseDir/SHBuild . "-xd,$SHBuild_BuildDir" -xmode,2 $@ $CXXFLAGS \
	$INCLUDES
# NOTE: The output be same as following commands, except for "SHBuild.a".
#$SHBuild_BaseDir/SHBuild . "-xd,$SHBuild_BuildDir" $@ $CXXFLAGS $INCLUDES
#"$LD" $SHBuild_BuildDir/SHBuild.a -o$SHBuild_BuildDir/SHBuild \
#	$CXXFLAGS $LDFLAGS $INCLUDES $LIBS

SHBuild_Popd
SHBuild_Puts Done.

