TOP=..
include $(TOP)/configure/CONFIG

# Sanity checking
#!$(info $$OS_CLASS = $(OS_CLASS))
#!$(info $$T_A = $(T_A))

ifeq (win32-x86, $(findstring win32-x86, $(T_A)))
SRC_DIRS += ../AMC-APIs/c-dll/windows/32bit
LIB_INSTALLS_WIN32    += ../AMC-APIs/c-dll/windows/32bit/attocubeJSON.lib
BIN_INSTALLS_WIN32    += ../AMC-APIs/c-dll/windows/32bit/attocubeJSON.dll

else ifeq (windows-x64, $(findstring windows-x64, $(T_A)))
SRC_DIRS += ../AMC-APIs/c-dll/windows/64bit
LIB_INSTALLS_WIN32    += ../AMC-APIs/c-dll/windows/64bit/attocubeJSON.lib
BIN_INSTALLS_WIN32    += ../AMC-APIs/c-dll/windows/64bit/attocubeJSON.dll

# Use OS_CLASS check for linux to avoid breaking custom Linux arches
else ifeq ($(OS_CLASS),Linux)

# Check T_A for 64-bit, 32-bit, and arm
ifeq (x86_64, $(findstring x86_64, $(T_A)))
SRC_DIRS += ../AMC-APIs/c-dll/linux/64bit
LIB_INSTALLS_Linux += ../AMC-APIs/c-dll/linux/64bit/libattocubeJSON.so

else ifeq (x86, $(findstring x86, $(T_A)))
SRC_DIRS += ../AMC-APIs/c-dll/linux/32bit
LIB_INSTALLS_Linux += ../AMC-APIs/c-dll/linux/32bit/libattocubeJSON.so

else ifeq (arm, $(findstring arm, $(T_A)))
SRC_DIRS += ../AMC-APIs/c-dll/linux/arm-32bit
LIB_INSTALLS_Linux += ../AMC-APIs/c-dll/linux/arm-32bit/libattocubeJSON.so

# End T_A check
endif

# The following lines install the headers into include/
INC += attocubeJSONCall.h
INC += generatedAPI.h

# End OS_CLASS=Linux
endif


# Header file names are the same regardless of arch, but should be pulled from respective directories
INC_WIN32 += attocubeJSONCall.h
INC_WIN32 += generatedAPI.h
# The following lines install the headers into include/os/Linux/
#!INC_Linux += attocubeJSONCall.h
#!INC_Linux += generatedAPI.h


include $(TOP)/configure/RULES
