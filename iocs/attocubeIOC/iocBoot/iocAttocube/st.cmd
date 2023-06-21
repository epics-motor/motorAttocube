#!../../bin/linux-x86_64/attocube

< envPaths
< /epics/common/xf18idb-ioc1-netsetup.cmd


epicsEnvSet("PREFIX", "XF:18IDB-OP{ATTOCUBE}")

## Register all support components
dbLoadDatabase "$(TOP)/dbd/attocube.dbd"
attocube_registerRecordDeviceDriver pdbbase


## motorUtil (allstop & alldone)
#dbLoadRecords("$(MOTOR)/db/motorUtil.db", "P=attocube:")

##
< AMC100.cmd

iocInit()

## motorUtil (allstop & alldone)
#motorUtilInit("attocube:")

# Boot complete
