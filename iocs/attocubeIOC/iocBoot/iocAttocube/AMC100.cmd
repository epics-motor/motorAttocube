
epicsEnvSet("PORT", "AC1")
epicsEnvSet("CONTROLLER_IP", "10.18.0.122")
epicsEnvSet("PREFIX", "XF:18IDB-OP{ATTOCUBE}")


AttocubeControllerConfig("$(PORT)", "$(CONTROLLER_IP)", 3)
dbLoadRecords("$(MOTOR_ATTOCUBE)/db/Attocube.template", "PREFIX=$(PREFIX), PORT=$(PORT), ADDR=0, TIMEOUT=1")


dbLoadTemplate("AMC100.substitutions")


iocInit()