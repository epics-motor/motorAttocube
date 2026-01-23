TOP=..
include $(TOP)/configure/CONFIG

ifeq (win32-x86, $(findstring win32-x86, $(T_A)))
SRC_DIRS += ../AMC-APIs/c-dll/windows/32bit
LIB_INSTALLS_WIN32    += ../AMC-APIs/c-dll/windows/32bit/attocubeJSON.lib
BIN_INSTALLS_WIN32    += ../AMC-APIs/c-dll/windows/32bit/attocubeJSON.dll

else ifeq (windows-x64, $(findstring windows-x64, $(T_A)))
SRC_DIRS += ../AMC-APIs/c-dll/windows/64bit
LIB_INSTALLS_WIN32    += ../AMC-APIs/c-dll/windows/64bit/attocubeJSON.lib
BIN_INSTALLS_WIN32    += ../AMC-APIs/c-dll/windows/64bit/attocubeJSON.dll


else ifeq (linux-x86_64, $(findstring linux-x86_64, $(T_A)))
SRC_DIRS += ../AMC-APIs/c-dll/linux/64bit
LIB_INSTALLS_Linux += ../AMC-APIs/c-dll/linux/64bit/libattocubeJSON.so

else ifeq (linux-x86, $(findstring linux-x86, $(T_A)))
SRC_DIRS += ../AMC-APIs/c-dll/linux/32bit
LIB_INSTALLS_Linux += ../AMC-APIs/c-dll/linux/32bit/libattocubeJSON.so

else ifeq (linux-arm, $(findstring linux-arm, $(T_A)))
SRC_DIRS += ../AMC-APIs/c-dll/linux/arm-32bit
LIB_INSTALLS_Linux += ../AMC-APIs/c-dll/linux/arm-32bit/libattocubeJSON.so

endif


# Header file names are the same regardless of arch, but should be pulled from respective directories
INC += attocubeJSONCall.h
INC += generatedAPI.h


include $(TOP)/configure/RULES
