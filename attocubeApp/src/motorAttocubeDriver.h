/*
Jakub Wlodek
Dec 7 2022
*/


#include <epicsTime.h>
#include <epicsThread.h>

#include "asynMotorController.h"
#include "asynMotorAxis.h"

#include "attocubeJSONCall.h"
#include "generatedAPI.h"


#define AttocubeSerialNumberString "ATTOCUBE_SERIAL"
#define AttocubeDeviceNameString "ATTOCUBE_DEV_NAME"
#define AttocubeFirmwareVersionString "ATTOCUBE_FIRMWARE"
#define AttocubeHostnameString "ATTOCUBE_HOSTNAME"
#define AttocubeIpAddressString "ATTOCUBE_IP"
#define AttocubeMacAddressString "ATTOCUBE_MAC"



class epicsShareClass AttocubeAxis : public asynMotorAxis
{
public:

    AttocubeAxis(class AttocubeController *pController, int axisNum);
    ~AttocubeAxis();
    void report(FILE* fp, int level);
    asynStatus move(double position, int relative, double min_velocity, double max_velocity, double acceleration);
    //asynStatus moveVelocity(double min_velocity, double max_velocity, double acceleration);
    //asynStatus home(double min_velocity, double max_velocity, double acceleration, int forwards);
    asynStatus stop(double acceleration);
    asynStatus poll(bool *moving);
    //asynStatus setPosition(double position);

private:
    AttocubeController* pController;
    int channel;

friend class AttocubeController;
};


class epicsShareClass AttocubeController : public asynMotorController {
public:

    /* These are the fucntions we override from the base class */
    AttocubeController(const char *portName, const char* ip, int numAxes);
    ~AttocubeController();
    asynStatus writeInt32(asynUser* pasynUser, epicsInt32 value);
    void report(FILE* fp, int level);
    AttocubeAxis* getAxis(asynUser* pasynUser);
    AttocubeAxis* getAxis(int axisNun);
    int getHandle();

    asynUser* getAsynUser();
protected:
    int controllerHandle;
    int numAxes;

    int AttocubeSerialNumber;
#define FIRST_ATTOCUBE_PARAM AttocubeSerialNumber
    int AttocubeDeviceName;
    int AttocubeFirmwareVersion;
    int AttocubeHostname;
    int AttocubeIpAddress;
    int AttocubeMacAddress;


#define LAST_ATTOCUBE_PARAM AttocubeMacAddress
#define NUM_ATTOCUBE_PARAMS (&LAST_ATTOCUBE_PARAM - &FIRST_ATTOCUBE_PARAM + 1)
  
//friend class motorAttocubeAxis;
};
