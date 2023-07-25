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
#define ERR_AXIS(msg)                                                                                 \
    asynPrint(this->pController->getAsynUser(), ASYN_TRACE_ERROR, "ERROR | %s::%s: %s\n", axisClassName, functionName, \
              msg)

#define ERR_AXIS_ARGS(fmt, ...)                                                              \
    asynPrint(this->pController->getAsynUser(), ASYN_TRACE_ERROR, "ERROR | %s::%s: " fmt "\n", axisClassName, \
              functionName, __VA_ARGS__);

// Warning message formatters
#define WARN_AXIS(msg) \
    asynPrint(this->pController->getAsynUser(), ASYN_TRACE_ERROR, "WARN | %s::%s: %s\n", axisClassName, functionName, msg)

#define WARN_AXIS_ARGS(fmt, ...)                                                            \
    asynPrint(this->pController->getAsynUser(), ASYN_TRACE_ERROR, "WARN | %s::%s: " fmt "\n", axisClassName, \
              functionName, __VA_ARGS__);

// Log message formatters
#define LOG_AXIS(msg) \
    asynPrint(this->pController->getAsynUser(), ASYN_TRACE_ERROR, "%s::%s: %s\n", axisClassName, functionName, msg)

#define LOG_AXIS_ARGS(fmt, ...)                                                                       \
    asynPrint(this->pController->getAsynUser(), ASYN_TRACE_ERROR, "%s::%s: " fmt "\n", axisClassName, functionName, \
              __VA_ARGS__);


// Error message formatters
#define ERR(msg)                                                                                 \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "ERROR | %s::%s: %s\n", controllerClassName, functionName, \
              msg)

#define ERR_ARGS(fmt, ...)                                                              \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "ERROR | %s::%s: " fmt "\n", controllerClassName, \
              functionName, __VA_ARGS__);

// Warning message formatters
#define WARN(msg) \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "WARN | %s::%s: %s\n", controllerClassName, functionName, msg)

#define WARN_ARGS(fmt, ...)                                                            \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "WARN | %s::%s: " fmt "\n", controllerClassName, \
              functionName, __VA_ARGS__);

// Log message formatters
#define LOG(msg) \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "%s::%s: %s\n", controllerClassName, functionName, msg)

#define LOG_ARGS(fmt, ...)                                                                       \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "%s::%s: " fmt "\n", controllerClassName, functionName, \
              __VA_ARGS__);


static const char* controllerClassName = "AttocubeController";
static const char* axisClassName = "AttocubeAxis";



static void exitCallbackC(void* pPvt) {
    AttocubeController* pController = (AttocubeController*) pPvt;
    delete pController;
}
/*
extern "C" asynStatus AttocubeAxisConfig(const char* portName) {
    new AttocubeAxis(portName);
    return asynSuccess;
}
*/

void AttocubeAxis::report(FILE* fp, int level){
    asynMotorAxis::report(fp, level);
}

asynStatus AttocubeAxis::move(double target, int relative, double minVelocity, double maxVelocity, double acceleration) {
    asynStatus status = asynSuccess;
    static const char* functionName = "move";

    // Since we configured the outward facing interface to use um, we need to multiply the target by 1000 to get nm
    //double target_nm = target * 1000;
    double target_nm = target;
    printf("Got move command: Target: %lf, Relative: %d, minVel: %lf, maxVel: %lf, acc: %lf\n", target_nm, relative, minVelocity, maxVelocity, acceleration);

    double position;
	AMC_move_getPosition(this->pController->getHandle(), this->channel, &position);

    double finalPosition = target_nm;
    if(relative == 1)
        finalPosition = position + target_nm;

	AMC_move_setControlTargetPosition(this->pController->getHandle(), this->channel, finalPosition);
	AMC_control_setControlMove(this->pController->getHandle(), this->channel, true);

    bool inTargetRange;
    AMC_status_getStatusTargetRange(this->pController->getHandle(), this->channel, &inTargetRange);
    while (!inTargetRange) {
        // Read out position in nm
        AMC_move_getPosition(this->pController->getHandle(), this->channel, &position);
        printf("Position %f nm\n", position);
        AMC_status_getStatusTargetRange(this->pController->getHandle(), this->channel, &inTargetRange);
    }

    AMC_control_setControlMove(this->pController->getHandle(), this->channel, false);
    return status;
}


asynStatus AttocubeAxis::stop(double acceleration){

    AMC_control_setControlMove(this->pController->getHandle(), this->channel, false);
    printf("Detected stop command with accel %lf\n", acceleration);
    return asynSuccess;

}


asynStatus AttocubeAxis::home(double minVelocity, double maxVelocity, double acceleration, int forwards) {
    static const char* functionName = "home";
    printf("Home command recieved for axis %d\n", this->channel);
    AMC_control_searchReferencePosition(this->pController->getHandle(), this->channel);
}


asynStatus AttocubeAxis::poll(bool* moving) {
    int done;
    int positionerType;
    int isReferenced;
    bool closedLoop;
    int axisState;
    double position;
    int referencePosition;
    bool inverted;
/** 
    AMC_control_getActorType(this->pController->getHandle(), this->channel, &positionerType);
    AMC_control_getControlMove(this->pController->getHandle(), this->channel, &closedLoop);
    AMC_control_getReferencePosition(this->pController->getHandle(), this->channel, &referencePosition);
    AMC_control_getSensorDirection(this->pController->getHandle(), this->channel, &inverted);
   */ 
    AMC_move_getPosition(this->pController->getHandle(), this->channel, &position);
    AMC_status_getStatusTargetRange(this->pController->getHandle(), this->channel, moving);


    done = *moving ? 0:1;
    //int isClosedLoop = closedLoop ? 1:0;
    setIntegerParam(this->pController->motorStatusDone_, done);
    //setIntegerParam(this->pController->motorStatusDone_, isClosedLoop);
    
    // AMC software works in nm, since we configured the outward facing interface to be um, divide by 1000.
    //position = position / 1000.0;
    setDoubleParam(this->pController->motorPosition_, (double) position);

    callParamCallbacks();
    return asynSuccess;
}

AttocubeAxis::AttocubeAxis(AttocubeController* pC, int axisNum)
    : asynMotorAxis(pC, axisNum),
    pController(pC)
{
    asynStatus status;
    const char* functionName = "AttocubeAxis";
    LOG_AXIS_ARGS("Configuring Axis %d", axisNum);
    this->pController = pController;
    this->channel = axisNum;

    AMC_control_setControlOutput(this->pController->getHandle(), this->channel, true);
    //LOG_AXIS_ARGS("ACTIVATED AXIS %d!", axisNum);

    callParamCallbacks();
}

AttocubeAxis::~AttocubeAxis(){
    static const char* functionName = "~AttocubeAxis";
    bool enabled;


    AMC_control_getControlOutput(this->pController->getHandle(), this->channel, &enabled);

    if(enabled){
        LOG_AXIS_ARGS("Disabling activated axis: %d", this->channel);
        AMC_control_setControlOutput(this->pController->getHandle(), this->channel, false);
    }
}


extern "C" asynStatus AttocubeControllerConfig(const char* portName, const char* ip, int numAxes) {
    new AttocubeController(portName, ip, numAxes);
    return asynSuccess;
}


void AttocubeController::report(FILE* fp, int level){
    fprintf(fp, "Attocube motor driver: %s\n", this->portName);
    asynMotorController::report(fp, level);
}

AttocubeAxis* AttocubeController::getAxis(asynUser* pasynUser){
    return static_cast<AttocubeAxis*>(asynMotorController::getAxis(pasynUser));
}


AttocubeAxis* AttocubeController::getAxis(int axisNum){
    return static_cast<AttocubeAxis*>(asynMotorController::getAxis(axisNum));
}


asynStatus AttocubeController::writeInt32(asynUser* pasynUser, epicsInt32 value){

    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    AttocubeAxis* pAxis = getAxis(pasynUser);
    static const char* functionName = "writeInt32";

    status = setIntegerParam(pAxis->axisNo_, function, value);

    status = asynMotorController::writeInt32(pasynUser, value);

    callParamCallbacks(pAxis->axisNo_);
    if(status){
        ERR_ARGS("Error, status=%d, function=%d, value=%d", status, function, value);
    }

    return status;
}


int AttocubeController::getHandle(){
    return this->controllerHandle;
}


asynUser* AttocubeController::getAsynUser(){
    return pasynUserSelf;
}





static void pollThreadC(void* pPvt){
    AttocubeController* pController = (AttocubeController*) pPvt;
    pController->pollThread();
}


void AttocubeController::pollThread(){

    bool moving;
    AttocubeAxis* pAxis;
    epicsTimeStamp nowTime;
    int status;
    bool anyMoving;
    int i;

    while(pollActive){
        epicsThreadSleep(0.1);
        lock();
        this->poll();

        for(i=0; i<numAxes_; i++){
            pAxis = getAxis(i);
            if (!pAxis) continue;
            else pAxis->poll(&moving);
        }
        unlock();
    }
}



AttocubeController::AttocubeController(const char* portName, const char* ip, int numAxes)
    : asynMotorController(portName, 3, NUM_ATTOCUBE_PARAMS,
                          0, 0,
                          ASYN_CANBLOCK | ASYN_MULTIDEVICE, 
                          1, // autoconnect
                          0, 0)  // Default priority and stack size)
{
    int axis;
    this->numAxes = numAxes;
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
    result = Connect(ip, &this->controllerHandle);
    if (result != ATTOCUBE_Ok) {
        ERR_ARGS("Failed to connect to controller with IP: %s! Error Code: %d", ip, result);
    }
    else {
        LOG("Collecting controller information");
        char serial[256], devName[256], firmware[256], hostname[256], cip[256], mac[256];

        system_getSerialNumber(this->getHandle(), serial, 256);
        setStringParam(AttocubeSerialNumber, serial);

        system_getFirmwareVersion(this->getHandle(), firmware, 256);
        setStringParam(AttocubeFirmwareVersion, firmware);

        system_getHostname(this->getHandle(), hostname, 256);
        setStringParam(AttocubeHostname, hostname);

        system_getDeviceName(this->getHandle(), devName, 256);
        setStringParam(AttocubeDeviceName, devName);

        system_network_getIpAddress(this->getHandle(), cip, 256);
        setStringParam(AttocubeIpAddress, cip);

        system_getMacAddress(this->getHandle(), mac, 256);
        setStringParam(AttocubeMacAddress, mac);

        printf("Connected to device: %s\n", serial);
    }
    for(axis = 0; axis < numAxes; axis++){
        new AttocubeAxis(this, axis);
    }

    epicsAtExit(exitCallbackC, (void*) this);

    epicsThreadOpts opts;
    opts.priority = epicsThreadPriorityMedium;
    opts.stackSize = epicsThreadGetStackSize(epicsThreadStackMedium);
    opts.joinable = 1;

    // Spawn our data collection thread. Make it joinable
    this->pollThreadId =
        epicsThreadCreateOpt("AttocubePollThread", (EPICSTHREADFUNC)pollThreadC, this, &opts);

}

AttocubeController::~AttocubeController(){
    const char* functionName = "~AttocubeController";

    LOG("Shutting down poll thread...");
    this->pollActive = false;
    epicsThreadMustJoin(this->pollThreadId);

    LOG("Disabling axes...");
    int axis;
    for(axis=0;axis<this->numAxes;axis++){
        AttocubeAxis* pAxis = this->getAxis(axis);
        if(pAxis != NULL)
            delete pAxis;
    }

    LOG("Disconnecting from controller...");
    int result = Disconnect(this->getHandle());
    if(result != ATTOCUBE_Ok) {
        ERR_ARGS("Failed to disconnect from controller. Error Code: %d", result);
    }
    else
        LOG("Goodbye.");

}


/** Code for iocsh registration */
static const iocshArg AttocubeControllerConfigArg0 = {"Port name", iocshArgString};
static const iocshArg AttocubeControllerConfigArg1 = {"IP", iocshArgString};
static const iocshArg AttocubeControllerConfigArg2 = {"Num axes", iocshArgInt};
static const iocshArg * const AttocubeControllerConfigArgs[] = {&AttocubeControllerConfigArg0,
                                                            &AttocubeControllerConfigArg1,
                                                            &AttocubeControllerConfigArg2};
static const iocshFuncDef AttocubeControllerConfigDef = {"AttocubeControllerConfig", 3, AttocubeControllerConfigArgs};

static void AttocubeControllerCallFunc(const iocshArgBuf *args)
{
    AttocubeControllerConfig(args[0].sval, args[1].sval, args[2].ival);
}

static void AttocubeRegister(void)
{
    iocshRegister(&AttocubeControllerConfigDef, AttocubeControllerCallFunc);
}

extern "C" {
    epicsExportRegistrar(AttocubeRegister);
}
