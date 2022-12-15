/*
FILENAME... motorAttocubeDriver.cc
USAGE...    Driver support for the Attocube line of controllers using their Programmable interface. Tested with AMC100

Jakub Wlodek
Dec 7, 2022
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iocsh.h>
#include <epicsThread.h>
#include <epicsExit.h>

#include "asynOctetSyncIO.h"

#include "asynMotorController.h"
#include "asynMotorAxis.h"

#include <epicsExport.h>
#include "motorAttocubeDriver.h"



// Error message formatters
#define ERR(msg)                                                                                 \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "ERROR | %s::%s: %s\n", driverName, functionName, \
              msg)

#define ERR_ARGS(fmt, ...)                                                              \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "ERROR | %s::%s: " fmt "\n", driverName, \
              functionName, __VA_ARGS__);

// Warning message formatters
#define WARN(msg) \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "WARN | %s::%s: %s\n", driverName, functionName, msg)

#define WARN_ARGS(fmt, ...)                                                            \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "WARN | %s::%s: " fmt "\n", driverName, \
              functionName, __VA_ARGS__);

// Log message formatters
#define LOG(msg) \
    asynPrint(pasynUserSelf, ASYN_TRACE_FLOW, "%s::%s: %s\n", driverName, functionName, msg)

#define LOG_ARGS(fmt, ...)                                                                       \
    asynPrint(pasynUserSelf, ASYN_TRACE_FLOW, "%s::%s: " fmt "\n", driverName, functionName, \
              __VA_ARGS__);


static const char* driverName = "MotorAttocubeDriver";


static void exitCallbackC(void* pPvt) {
    AttocubeController* pController = (AttocubeController) pPvt;
    delete pController;
}

extern "C" asynStatus AttocubeControllerConfig(const char* portName, const char* ip) {
    new AttocubeController(portName, ip);
    return asynSuccess;
}

AttocubeController::AttocubeController(const char* portName, const char* ip)
    : asynMotorController(portName, 3, NUM_ATTOCUBE_PARAMS,
                          0, 0,
                          ASYN_CANBLOCK | ASYN_MULTIDEVICE, 
                          1, // autoconnect
                          0, 0)  // Default priority and stack size)
{
    int axis;
    asynStatus status;
    int result;


    createParam(AttocubeSerialNumberString, asynParamOctet, &AttocubeSerialNumber);
    createParam(AttocubeDeviceNameString, asynParamOctet, &AttocubeDeviceName);
    createParam(AttocubeFirmwareVersionString, asynParamOctet, &AttocubeFirmwareVersion);
    createParam(AttocubeHostnameString, asynParamOctet, &AttocubeHostname);
    createParam(AttocubeIpAddressString, asynParamOctet, &AttocubeIpAddress);
    createParam(AttocubeMacAddressString, asynParamOctet, &AttocubeMacAddress);

    static const char* functionName = "AttocubeController";

    LOG("Initializing Attocube Controller");

    LOG_ARGS("Connecting to controller with IP: %s", ip);

    // Use the Attocube API to connect to a controller by IP.
    result = Connect(ip, &(this->controllerHandle));
    if (result != ATTOCUBE_Ok) {
        ERR_ARGS("Failed to connect to controller with IP: %s! Error Code: %d", ip, result);
    }
    else {
        LOG("Collecting controller information");
        char serial[256], devName[256], firmware[256], hostname[256], cip[256], mac[256];

        system_getSerialNumber(this->controllerHandle, serial, 256);
        setStringParam(AttocubeSerialNumber, serial);

        system_getFirmwareVersion(this->controllerHandle, firmware, 256);
        setStringParam(AttocubeFirmwareVersion, firmware);

        system_getHostname(this->controllerHandle, hostname, 256);
        setStringParam(AttocubeHostname, hostname);

        system_getDeviceName(this->controllerHandle, devName, 256);
        setStringParam(AttocubeDeviceName, devName);

        system_network_getIpAddress(this->controllerHandle, cip, 256);
        setStringParam(AttocubeIpAddress, cip);

        system_getMacAddress(this->controllerHandle, mac, 256);
        setStringParam(AttocubeMacAddress, mac);

        printf("Connected to device: %s\n", serial);
    }

    epicsAtExit(exitCallbackC, (void*) this);

}

AttocubeController::~AttocubeController(){
    const char* functionName = "~AttocubeController";
    LOG("Disconnecting from controller...");
    int result = Disconnect(this->controllerHandle);
    if(result != ATTOCUBE_Ok) {
        ERR_ARGS("Failed to disconnect from controller. Error Code: %d", result);
    }
    else
        LOG("Goodbye.");

}


/** Code for iocsh registration */
static const iocshArg AttocubeControllerConfigArg0 = {"Port name", iocshArgString};
static const iocshArg AttocubeControllerConfigArg1 = {"IP", iocshArgString};
static const iocshArg * const AttocubeControllerConfigArgs[] = {&AttocubeControllerConfigArg0,
                                                            &AttocubeControllerConfigArg1};
static const iocshFuncDef AttocubeControllerConfigDef = {"AttocubeControllerConfig", 2, AttocubeControllerConfigArgs};
static void AttocubeControllerCallFunc(const iocshArgBuf *args)
{
  AttocubeControllerConfig(args[0].sval, args[1].sval);
}

static void AttocubeRegister(void)
{
  iocshRegister(&AttocubeControllerConfigDef, AttocubeControllerCallFunc);
}

extern "C" {
epicsExportRegistrar(AttocubeRegister);
}