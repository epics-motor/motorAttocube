TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = attocube
# attocube.dbd will be created and installed
DBD += attocube.dbd

# attocube.dbd will be made up from these files:
attocube_DBD += base.dbd

# Include dbd files from all support applications:
#attocube_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#attocube_LIBS += xxx

# attocube_registerRecordDeviceDriver.cpp derives from attocube.dbd
attocube_SRCS += attocube_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
attocube_SRCS_DEFAULT += attocubeMain.cpp
attocube_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#attocube_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
attocube_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

