# Makefile
TOP = ../..
include $(TOP)/configure/CONFIG
#
# Both the following line, and a line in the *.dbd file,
# must be uncommented to use diagnostic debugging messages.
#!USR_CXXFLAGS += -DDEBUG

DBD += devAttocube.dbd attocubeDriver.dbd

INC += motorAttocubeDriver.h

LIBRARY_IOC = Attocube

# ANC 150 asyn motor driver.
Attocube_SRCS += drvANC150Asyn.cc
# The following driver doesn't work on OS X
Attocube_SRCS_Linux += motorAttocubeDriver.cc
Attocube_SRCS_WIN32 += motorAttocubeDriver.cc

Attocube_LIBS += motor asyn
Attocube_LIBS_Linux += $(EPICS_BASE_IOC_LIBS) attocubeJSON
Attocube_LIBS_WIN32 += $(EPICS_BASE_IOC_LIBS) attocubeJSON

include $(TOP)/configure/RULES

