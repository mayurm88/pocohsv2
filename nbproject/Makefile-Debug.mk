#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_DLIB_EXT=dylib
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/JsonPost.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++11
CXXFLAGS=-std=c++11

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/local/lib /usr/local/lib/libPocoUtil.dylib /usr/local/lib/libPocoNet.dylib /usr/local/lib/libPocoJSON.dylib /usr/local/lib/libPocoFoundation.dylib /usr/local/lib/libcppunit.dylib

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpohohsv2.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpohohsv2.${CND_DLIB_EXT}: /usr/local/lib/libPocoUtil.dylib

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpohohsv2.${CND_DLIB_EXT}: /usr/local/lib/libPocoNet.dylib

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpohohsv2.${CND_DLIB_EXT}: /usr/local/lib/libPocoJSON.dylib

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpohohsv2.${CND_DLIB_EXT}: /usr/local/lib/libPocoFoundation.dylib

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpohohsv2.${CND_DLIB_EXT}: /usr/local/lib/libcppunit.dylib

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpohohsv2.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpohohsv2.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -dynamiclib -install_name libpohohsv2.${CND_DLIB_EXT} -fPIC

${OBJECTDIR}/JsonPost.o: JsonPost.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/JsonPost.o JsonPost.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/JsonPlaceHolderRunner.o ${TESTDIR}/tests/JsonPlaceHolderTests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   


${TESTDIR}/tests/JsonPlaceHolderRunner.o: tests/JsonPlaceHolderRunner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/JsonPlaceHolderRunner.o tests/JsonPlaceHolderRunner.cpp


${TESTDIR}/tests/JsonPlaceHolderTests.o: tests/JsonPlaceHolderTests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/JsonPlaceHolderTests.o tests/JsonPlaceHolderTests.cpp


${OBJECTDIR}/JsonPost_nomain.o: ${OBJECTDIR}/JsonPost.o JsonPost.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/JsonPost.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -I/usr/local/include -fPIC  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/JsonPost_nomain.o JsonPost.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/JsonPost.o ${OBJECTDIR}/JsonPost_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpohohsv2.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
