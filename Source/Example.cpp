//‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹
//›                                                                         ﬁ
//› Module: Internals Example Source File                                   ﬁ
//›                                                                         ﬁ
//› Description: Declarations for the Internals Example Plugin              ﬁ
//›                                                                         ﬁ
//›                                                                         ﬁ
//› This source code module, and all information, data, and algorithms      ﬁ
//› associated with it, are part of CUBE technology (tm).                   ﬁ
//›                 PROPRIETARY AND CONFIDENTIAL                            ﬁ
//› Copyright (c) 1996-2007 Image Space Incorporated.  All rights reserved. ﬁ
//›                                                                         ﬁ
//›                                                                         ﬁ
//› Change history:                                                         ﬁ
//›   tag.2005.11.30: created                                               ﬁ
//›                                                                         ﬁ
//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ

// Needed for winsock libraries
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define YAML_CPP_DLL
#include "Example.hpp"					// corresponding header file
#include <math.h>						// for atan2, sqrt
#include <stdio.h>						// for sample output
#include <string>
#include <stdio.h>
#include <iostream>
#include <yaml-cpp/yaml.h>				// for yaml parsing
#include <winsock2.h>					// needed for socket UDP transmission
#include <ws2tcpip.h>					// needed for socket UDP transmission
#pragma comment(lib, "Ws2_32.lib")		// needed for socket UDP transmission

// plugin information
unsigned g_uPluginID          = 0;
char     g_szPluginName[]     = "LiveTelemetryPlugin";
unsigned g_uPluginVersion     = 001;
unsigned g_uPluginObjectCount = 1;
InternalsPluginInfo g_PluginInfo;

// loading YAML configuration file
YAML::Node config = YAML::LoadFile("telemetryConfig.yaml");

// UDP Socket
std::string IPAddress = "";
int port;

struct SignalsData {
	float clutchRPM;				// Clutch RPM
	float deltaTime;				// Time between frames
	float engineOilTemp; 			// Engine oil temperature	
	float engineWaterTemp; 			// Engine water temperature
	float lapStartET;				// Lap start elapsed time
	float lapDistance;				// Lap distance
	float bestS1; 					// Best sector 1 time
	float bestS2; 					// Best sector 2 time (plus sector 1)
	float bestLap; 					// Best laptime
	float lastS1; 					// Last sector 1 time
	float lastS2; 					// Last sector 2 time (plus sector 1)
	float lastLap; 					// Last laptime
	float currentS1; 				// Current sector 1 time
	float currentS2; 				// Current sector 2 time (plus sector 1)
	float numPitstops; 				// Number of pitstops
	float numPenalties; 			// Number of penalties
	float localAccelX;				// Local X acceleration
	float localAccelY;				// Local Y acceleration
	float localAccelZ;				// Local Z acceleration
	float localRotX;				// Local X rotation
	float localRotY;				// Local Y rotation
	float localRotZ;				// Local Z rotation
	float localRotAccelX;			// Local X rotation acceleration
	float localRotAccelY;			// Local Y rotation acceleration
	float localRotAccelZ;			// Local Z rotation acceleration
	float localVelX;				// Local X velocity
	float localVelY;				// Local Y velocity
	float localVelZ;				// Local Z velocity
	float oriXX;					// Orientation XX
	float oriXY;					// Orientation XY
	float oriXZ;					// Orientation XZ
	float oriYX;					// Orientation YX
	float oriYY;					// Orientation YY
	float oriYZ;					// Orientation YZ
	float oriZX;					// Orientation ZX
	float oriZY;					// Orientation ZY
	float oriZZ;					// Orientation ZZ
	float posX;						// Position X
	float posY;						// Position Y
	float posZ;						// Position Z
	float steeringArmForce;			// Steering arm force
	const char* trackName;			// Track name
	const char* vehicleName;		// Vehicle name
	const char* driverName;			// Driver name
	float speed;					// Speed
	float accelLat;					// Lateral acceleration
	float accelLong;				// Longitudinal acceleration
	float roll;						// Roll
	float pitch;					// Pitch
	float rpm;						// RPM
	float maxRPM;					// Max RPM
	float lapIniTime;				// Lap initial time
	float lapNumber;				// Lap number
	float brake;					// Brake
	float clutch;					// Clutch
	float steering;					// Steering
	float throttle;					// Throttle
	float gear;						// Gear
	float brakeTempFL;				// Brake temperature Front Left
	float brakeTempFR;				// Brake temperature Front Right
	float brakeTempRL;				// Brake temperature Rear Left
	float brakeTempRR;				// Brake temperature Rear Right
	float gripFactorFL;				// Grip factor Front Left
	float gripFactorFR;				// Grip factor Front Right
	float gripFactorRL;				// Grip factor Rear Left
	float gripFactorRR;				// Grip factor Rear Right
	float lateralForceFL;			// Lateral force Front Left
	float lateralForceFR;			// Lateral force Front Right
	float lateralForceRL;			// Lateral force Rear Left
	float lateralForceRR;			// Lateral force Rear Right
	float pressureFL;				// Pressure Front Left
	float pressureFR;				// Pressure Front Right
	float pressureRL;				// Pressure Rear Left
	float pressureRR;				// Pressure Rear Right
	float rideHeightFL;				// Ride height Front Left
	float rideHeightFR;				// Ride height Front Right
	float rideHeightRL;				// Ride height Rear Left
	float rideHeightRR;				// Ride height Rear Right
	float rotationFL;				// Rotation Front Left
	float rotationFR;				// Rotation Front Right
	float rotationRL;				// Rotation Rear Left
	float rotationRR;				// Rotation Rear Right
	float shockDeflectionFL;		// Shock deflection Front Left
	float shockDeflectionFR;		// Shock deflection Front Right
	float shockDeflectionRL;		// Shock deflection Rear Left
	float shockDeflectionRR;		// Shock deflection Rear Right
	float tireTempFLI;				// Tire temperature Front Left Inner
	float tireTempFLM;				// Tire temperature Front Left Middle
	float tireTempFLO;				// Tire temperature Front Left Outer
	float tireTempFRI;				// Tire temperature Front Right Inner
	float tireTempFRM;				// Tire temperature Front Right Middle
	float tireTempFRO;				// Tire temperature Front Right Outer
	float tireTempRLI;				// Tire temperature Rear Left Inner
	float tireTempRLM;				// Tire temperature Rear Left Middle
	float tireTempRLO;				// Tire temperature Rear Left Outer
	float tireTempRRI;				// Tire temperature Rear Right Inner
	float tireTempRRM;				// Tire temperature Rear Right Middle
	float tireTempRRO;				// Tire temperature Rear Right Outer
	float tireLoadFL;				// Tire load Front Left
	float tireLoadFR;				// Tire load Front Right
	float tireLoadRL;				// Tire load Rear Left
	float tireLoadRR;				// Tire load Rear Right
	float tireWearFL;				// Tire wear Front Left
	float tireWearFR;				// Tire wear Front Right
	float tireWearRL;				// Tire wear Rear Left
	float tireWearRR;				// Tire wear Rear Right
	float detached;					// Whether any part (besides wheels) is detached
	float detachedFL;				// Whether the Front Left wheel is detached
	float detachedFR;				// Whether the Front Right wheel is detached
	float detachedRL;				// Whether the Rear Left wheel is detached
	float detachedRR;				// Whether the Rear Right wheel is detached
	float overheating;				// Whether the overheating icon is shown in the screen
	float flatFL;					// Whether the Front Left tire is flat
	float flatFR;					// Whether the Front Right tire is flat
	float flatRL;					// Whether the Rear Left tire is flat
	float flatRR;					// Whether the Rear Right tire is flat
	float surfaceFL;				// Surface type Front Left
	float surfaceFR;				// Surface type Front Right
	float surfaceRL;				// Surface type Rear Left
	float surfaceRR;				// Surface type Rear Right
	float lastImpactET;				// Elapsed time of the last impact
	float lastImpactMagnitude;		// Magnitude of the last impact
	float lastImpactPosX;			// Position X of the last impact
	float lastImpactPosY;			// Position Y of the last impact
	float lastImpactPosZ;			// Position Z of the last impact
	float scheduledStops;			// Number of scheduled stops
	float fuel; 					// Fuel
	float isPlayer; 				// Whether the vehicle is the player's
	float whoIsInControl; 			// Who is in control of the vehicle
	float isInPit; 					// Whether the vehicle is in the pit
	const char* vehicleClass; 			// Vehicle class
	float deltaTimeBehind; 			// Time behind the vehicle in front
	float deltaLapsBehind; 			// Laps behind the vehicle in front
	float deltaTimeLeader; 			// Time behind the leader
	float deltaLapsLeader; 			// Laps behind the leader
	float numOfVehicles; 			// Number of vehicles
	const char* terrainNameFL; 			// Terrain name of FL tyre from the TDF file
	const char* terrainNameFR; 			// Terrain name of FR tyre from the TDF file
	const char* terrainNameRL; 			// Terrain name of RL tyre from the TDF file
	const char* terrainNameRR; 			// Terrain name of RR tyre from the TDF file
	float finishStatus; 			// Finish status (0=none, 1=finished, 2=dnf, 3=dq)
	unsigned char position; 		// Position on the race
	float maxLaps; 					// Maximum number of laps
	float endingTime; 				// Ending time
	float currentTime; 				// Current time
	float gamePhase; 				// Game phase (0=Before session has begun, 1=Reconnaissance laps (race only), 2=Grid walk-through (race only), 3=Formation lap (race only), 4=Starting-light countdown has begun (race only), 5=Green flag, 6=Full course yellow / safety car, 7=Session stopped, 8=Session over)
	float yellowFlagState; 			// Yellow flag state (-1=Invalid, 0=None, 1=Pending, 2=Pits closed, 3=Pit lead lap, 4=Pits open, 5=Last lap, 6=Resume, 7=Race halt (not used))
	float startLight; 				// Start light frame (number depends on track)
	float numRedLights; 			// Number of red lights in the start light sequence
	const char* playerName;				// Player name
	float cloudDarkness; 			// Cloud darkness (0.0 - 1.0)
	float rainSeverity; 			// Rain severity (0.0 - 1.0)
	float ambientTemp; 				// Ambient temperature
	float trackTemp; 				// Track temperature
	float windSpeedX; 				// Wind speed x-axis
	float windSpeedY; 				// Wind speed y-axis
	float windSpeedZ; 				// Wind speed z-axis
	float onPathWetness; 			// On-path wetness (0.0 - 1.0)
	float offPathWetness; 			// Off-path wetness (0.0 - 1.0)
};

WSADATA wsaData;
SOCKET sockfd;
sockaddr_in servaddr;
SignalsData data;

// interface to plugin information
extern "C" __declspec(dllexport)
const char* __cdecl GetPluginName() { return g_szPluginName; }

extern "C" __declspec(dllexport)
unsigned __cdecl GetPluginVersion() { return g_uPluginVersion; }

extern "C" __declspec(dllexport)
unsigned __cdecl GetPluginObjectCount() { return g_uPluginObjectCount; }

// get the plugin-info object used to create the plugin.
extern "C" __declspec(dllexport)

PluginObjectInfo* __cdecl GetPluginObjectInfo( const unsigned uIndex )
{
  switch(uIndex)
  {
    case 0:
      return  &g_PluginInfo;
    default:
      return 0;
  }
}


// InternalsPluginInfo class

InternalsPluginInfo::InternalsPluginInfo()
{
  // put together a name for this plugin
  sprintf( m_szFullName, "%s - %s", g_szPluginName, InternalsPluginInfo::GetName() );
}

const char*    InternalsPluginInfo::GetName()     const { return ExampleInternalsPlugin::GetName(); }
const char*    InternalsPluginInfo::GetFullName() const { return m_szFullName; }
const char*    InternalsPluginInfo::GetDesc()     const { return "Example Internals Plugin"; }
const unsigned InternalsPluginInfo::GetType()     const { return ExampleInternalsPlugin::GetType(); }
const char*    InternalsPluginInfo::GetSubType()  const { return ExampleInternalsPlugin::GetSubType(); }
const unsigned InternalsPluginInfo::GetVersion()  const { return ExampleInternalsPlugin::GetVersion(); }
void*          InternalsPluginInfo::Create()      const { return new ExampleInternalsPlugin(); }


// InternalsPlugin class

const char ExampleInternalsPlugin::m_szName[] = "InternalsPlugin";
const char ExampleInternalsPlugin::m_szSubType[] = "Internals";
const unsigned ExampleInternalsPlugin::m_uID = 1;
const unsigned ExampleInternalsPlugin::m_uVersion = 3;  // set to 3 for InternalsPluginV3 functionality and added graphical and vehicle info


PluginObjectInfo *ExampleInternalsPlugin::GetInfo()
{
  return &g_PluginInfo;
}


void ExampleInternalsPlugin::WriteToAllExampleOutputFiles( const char * const openStr, const char * const msg )
{
  FILE *fo;

  fo = fopen( "ExampleInternalsTelemetryOutput.txt", openStr );
  if( fo != NULL )
  {
    fprintf( fo, "%s\n", msg );
    fclose( fo );
  }

  fo = fopen( "ExampleInternalsGraphicsOutput.txt", openStr );
  if( fo != NULL )
  {
    fprintf( fo, "%s\n", msg );
    fclose( fo );
  }

  fo = fopen( "ExampleInternalsScoringOutput.txt", openStr );
  if( fo != NULL )
  {
    fprintf( fo, "%s\n", msg );
    fclose( fo );
  }
}


void ExampleInternalsPlugin::Startup()
{
	// Open ports, read configs, whatever you need to do.  For now, I'll just clear out the
	// example output data.
	WriteToAllExampleOutputFiles( "w", "-STARTUP-" );

	// default enabled to true
	mEnabled = true;

	if (config["server"]["ip"].as<std::string>() != "") {
		IPAddress = config["server"]["ip"].as<std::string>();
	}
	else {
		IPAddress = "127.0.0.1";
	}

	if (config["server"]["port"].as<int>() != 0) {
		port = config["server"]["port"].as<int>();
	}
	else {
		port = 6000;
	}

	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup failed.\n";
	}

	// Create socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == INVALID_SOCKET) {
		std::cerr << "Socket creation failed.\n";
		WSACleanup();
	}

	// Set up server address structure
	ZeroMemory(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);  // Port number
	inet_pton(AF_INET, IPAddress.c_str(), &servaddr.sin_addr);
}

void ExampleInternalsPlugin::Shutdown()
{
  //WriteToAllExampleOutputFiles( "a", "-SHUTDOWN-" );
  closesocket(sockfd);
  WSACleanup();
}


void ExampleInternalsPlugin::StartSession()
{
  //WriteToAllExampleOutputFiles( "a", "--STARTSESSION--" );
}


void ExampleInternalsPlugin::EndSession()
{
  //WriteToAllExampleOutputFiles( "a", "--ENDSESSION--" );
}


void ExampleInternalsPlugin::EnterRealtime()
{
  // start up timer every time we enter realtime
  mET = 0.0f;
  //WriteToAllExampleOutputFiles( "a", "---ENTERREALTIME---" );
}


void ExampleInternalsPlugin::ExitRealtime()
{
  //WriteToAllExampleOutputFiles( "a", "---EXITREALTIME---" );
}


void ExampleInternalsPlugin::UpdateTelemetry( const TelemInfoV2 &info )
{
	// This function is called 90 times per seconds in manual mode and 40 times per seconds in I.A mode (approximate time)
	
	// Use the incoming data, for now I'll just write some of it to a file to a) make sure it
	// is working, and b) explain the coordinate system a little bit (see header for more info)

	// Compute some auxiliary info based on the above
	TelemVect3 forwardVector = { -info.mOriX.z, -info.mOriY.z, -info.mOriZ.z };
	TelemVect3    leftVector = { info.mOriX.x,  info.mOriY.x,  info.mOriZ.x };

	// These are normalized vectors, and remember that our world Y coordinate is up.  So you can
	// determine the current pitch and roll (w.r.t. the world x-z plane) as follows:
	const float pitch = atan2f(forwardVector.y, sqrtf((forwardVector.x * forwardVector.x) + (forwardVector.z * forwardVector.z)));
	const float  roll = atan2f(leftVector.y, sqrtf((leftVector.x * leftVector.x) + (leftVector.z * leftVector.z)));
	const float radsToDeg = 57.296f;

	const float metersPerSec = sqrtf((info.mLocalVel.x * info.mLocalVel.x) +
		(info.mLocalVel.y * info.mLocalVel.y) +
		(info.mLocalVel.z * info.mLocalVel.z));

	if (config["signals"]["clutchRPM"].as<bool>()) data.clutchRPM = info.mClutchRPM;
	if (config["signals"]["deltaTime"].as<bool>()) data.deltaTime = info.mDeltaTime;
	if (config["signals"]["engineOilTemp"].as<bool>()) data.engineOilTemp = info.mEngineOilTemp;
	if (config["signals"]["engineWaterTemp"].as<bool>()) data.engineWaterTemp = info.mEngineWaterTemp;
	if (config["signals"]["lapNumber"].as<bool>()) data.lapNumber = info.mLapNumber;
	if (config["signals"]["lapStartET"].as<bool>()) data.lapStartET = info.mLapStartET;
	if (config["signals"]["localAccelX"].as<bool>()) data.localAccelX = info.mLocalAccel.x;
	if (config["signals"]["localAccelY"].as<bool>()) data.localAccelY = info.mLocalAccel.y;
	if (config["signals"]["localAccelZ"].as<bool>()) data.localAccelZ = info.mLocalAccel.z;
	if (config["signals"]["localRotX"].as<bool>()) data.localRotX = info.mLocalRot.x;
	if (config["signals"]["localRotY"].as<bool>()) data.localRotY = info.mLocalRot.y;
	if (config["signals"]["localRotZ"].as<bool>()) data.localRotZ = info.mLocalRot.z;
	if (config["signals"]["localRotAccelX"].as<bool>()) data.localRotAccelX = info.mLocalRotAccel.x;
	if (config["signals"]["localRotAccelY"].as<bool>()) data.localRotAccelY = info.mLocalRotAccel.y;
	if (config["signals"]["localRotAccelZ"].as<bool>()) data.localRotAccelZ = info.mLocalRotAccel.z;
	if (config["signals"]["localVelX"].as<bool>()) data.localVelX = info.mLocalVel.x;
	if (config["signals"]["localVelY"].as<bool>()) data.localVelY = info.mLocalVel.y;
	if (config["signals"]["localVelZ"].as<bool>()) data.localVelZ = info.mLocalVel.z;
	if (config["signals"]["oriXX"].as<bool>()) data.oriXX = info.mOriX.x;
	if (config["signals"]["oriXY"].as<bool>()) data.oriXY = info.mOriX.y;
	if (config["signals"]["oriXZ"].as<bool>()) data.oriXZ = info.mOriX.z;
	if (config["signals"]["oriYX"].as<bool>()) data.oriYX = info.mOriY.x;
	if (config["signals"]["oriYY"].as<bool>()) data.oriYY = info.mOriY.y;
	if (config["signals"]["oriYZ"].as<bool>()) data.oriYZ = info.mOriY.z;
	if (config["signals"]["oriZX"].as<bool>()) data.oriZX = info.mOriZ.x;
	if (config["signals"]["oriZY"].as<bool>()) data.oriZY = info.mOriZ.y;
	if (config["signals"]["oriZZ"].as<bool>()) data.oriZZ = info.mOriZ.z;
	if (config["signals"]["posX"].as<bool>()) data.posX = info.mPos.x;
	if (config["signals"]["posY"].as<bool>()) data.posY = info.mPos.y;
	if (config["signals"]["posZ"].as<bool>()) data.posZ = info.mPos.z;
	if (config["signals"]["steeringArmForce"].as<bool>()) data.steeringArmForce = info.mSteeringArmForce;
	if (config["signals"]["trackName"].as<bool>()) data.trackName = info.mTrackName;
	if (config["signals"]["speed"].as<bool>()) data.speed = metersPerSec;
	if (config["signals"]["accelLat"].as<bool>()) data.accelLat = info.mLocalAccel.y;
	if (config["signals"]["accelLong"].as<bool>()) data.accelLong = info.mLocalAccel.z;
	if (config["signals"]["roll"].as<bool>()) data.roll = roll * radsToDeg;
	if (config["signals"]["pitch"].as<bool>()) data.pitch = pitch * radsToDeg;
	if (config["signals"]["rpm"].as<bool>()) data.rpm = info.mEngineRPM;
	if (config["signals"]["maxRPM"].as<bool>()) data.maxRPM = info.mEngineMaxRPM;
	if (config["signals"]["lapIniTime"].as<bool>()) data.lapIniTime = info.mLapStartET;
	if (config["signals"]["lapNumber"].as<bool>()) data.lapNumber = info.mLapNumber;
	if (config["signals"]["brake"].as<bool>()) data.brake = info.mUnfilteredBrake;
	if (config["signals"]["clutch"].as<bool>()) data.clutch = info.mUnfilteredClutch;
	if (config["signals"]["steering"].as<bool>()) data.steering = info.mUnfilteredSteering;
	if (config["signals"]["throttle"].as<bool>()) data.throttle = info.mUnfilteredThrottle;
	if (config["signals"]["gear"].as<bool>()) data.gear = info.mGear;
	if (config["signals"]["vehicleName"].as<bool>()) data.vehicleName = info.mVehicleName;
	if (config["signals"]["brakeTempFL"].as<bool>()) data.brakeTempFL = info.mWheel[0].mBrakeTemp;
	if (config["signals"]["brakeTempFR"].as<bool>()) data.brakeTempFR = info.mWheel[1].mBrakeTemp;
	if (config["signals"]["brakeTempRL"].as<bool>()) data.brakeTempRL = info.mWheel[2].mBrakeTemp;
	if (config["signals"]["brakeTempRR"].as<bool>()) data.brakeTempRR = info.mWheel[3].mBrakeTemp;
	if (config["signals"]["gripFactorFL"].as<bool>()) data.gripFactorFL = info.mWheel[0].mGripFract;
	if (config["signals"]["gripFactorFR"].as<bool>()) data.gripFactorFR = info.mWheel[1].mGripFract;
	if (config["signals"]["gripFactorRL"].as<bool>()) data.gripFactorRL = info.mWheel[2].mGripFract;
	if (config["signals"]["gripFactorRR"].as<bool>()) data.gripFactorRR = info.mWheel[3].mGripFract;
	if (config["signals"]["lateralForceFL"].as<bool>()) data.lateralForceFL = info.mWheel[0].mLateralForce;
	if (config["signals"]["lateralForceFR"].as<bool>()) data.lateralForceFR = info.mWheel[1].mLateralForce;
	if (config["signals"]["lateralForceRL"].as<bool>()) data.lateralForceRL = info.mWheel[2].mLateralForce;
	if (config["signals"]["lateralForceRR"].as<bool>()) data.lateralForceRR = info.mWheel[3].mLateralForce;
	if (config["signals"]["pressureFL"].as<bool>()) data.pressureFL = info.mWheel[0].mPressure;
	if (config["signals"]["pressureFR"].as<bool>()) data.pressureFR = info.mWheel[1].mPressure;
	if (config["signals"]["pressureRL"].as<bool>()) data.pressureRL = info.mWheel[2].mPressure;
	if (config["signals"]["pressureRR"].as<bool>()) data.pressureRR = info.mWheel[3].mPressure;
	if (config["signals"]["rideHeightFL"].as<bool>()) data.rideHeightFL = info.mWheel[0].mRideHeight;
	if (config["signals"]["rideHeightFR"].as<bool>()) data.rideHeightFR = info.mWheel[1].mRideHeight;
	if (config["signals"]["rideHeightRL"].as<bool>()) data.rideHeightRL = info.mWheel[2].mRideHeight;
	if (config["signals"]["rideHeightRR"].as<bool>()) data.rideHeightRR = info.mWheel[3].mRideHeight;
	if (config["signals"]["rotationFL"].as<bool>()) data.rotationFL = info.mWheel[0].mRotation;
	if (config["signals"]["rotationFR"].as<bool>()) data.rotationFR = info.mWheel[1].mRotation;
	if (config["signals"]["rotationRL"].as<bool>()) data.rotationRL = info.mWheel[2].mRotation;
	if (config["signals"]["rotationRR"].as<bool>()) data.rotationRR = info.mWheel[3].mRotation;
	if (config["signals"]["shockDeflectionFL"].as<bool>()) data.shockDeflectionFL = info.mWheel[0].mSuspensionDeflection;
	if (config["signals"]["shockDeflectionFR"].as<bool>()) data.shockDeflectionFR = info.mWheel[1].mSuspensionDeflection;
	if (config["signals"]["shockDeflectionRL"].as<bool>()) data.shockDeflectionRL = info.mWheel[2].mSuspensionDeflection;
	if (config["signals"]["shockDeflectionRR"].as<bool>()) data.shockDeflectionRR = info.mWheel[3].mSuspensionDeflection;
	if (config["signals"]["tireTempFLI"].as<bool>()) data.tireTempFLI = info.mWheel[0].mTemperature[0];
	if (config["signals"]["tireTempFLM"].as<bool>()) data.tireTempFLM = info.mWheel[0].mTemperature[1];
	if (config["signals"]["tireTempFLO"].as<bool>()) data.tireTempFLO = info.mWheel[0].mTemperature[2];
	if (config["signals"]["tireTempFRI"].as<bool>()) data.tireTempFRI = info.mWheel[1].mTemperature[0];
	if (config["signals"]["tireTempFRM"].as<bool>()) data.tireTempFRM = info.mWheel[1].mTemperature[1];
	if (config["signals"]["tireTempFRO"].as<bool>()) data.tireTempFRO = info.mWheel[1].mTemperature[2];
	if (config["signals"]["tireTempRLI"].as<bool>()) data.tireTempRLI = info.mWheel[2].mTemperature[0];
	if (config["signals"]["tireTempRLM"].as<bool>()) data.tireTempRLM = info.mWheel[2].mTemperature[1];
	if (config["signals"]["tireTempRLO"].as<bool>()) data.tireTempRLO = info.mWheel[2].mTemperature[2];
	if (config["signals"]["tireTempRRI"].as<bool>()) data.tireTempRRI = info.mWheel[3].mTemperature[0];
	if (config["signals"]["tireTempRRM"].as<bool>()) data.tireTempRRM = info.mWheel[3].mTemperature[1];
	if (config["signals"]["tireTempRRO"].as<bool>()) data.tireTempRRO = info.mWheel[3].mTemperature[2];
	if (config["signals"]["tireLoadFL"].as<bool>()) data.tireLoadFL = info.mWheel[0].mTireLoad;
	if (config["signals"]["tireLoadFR"].as<bool>()) data.tireLoadFR = info.mWheel[1].mTireLoad;
	if (config["signals"]["tireLoadRL"].as<bool>()) data.tireLoadRL = info.mWheel[2].mTireLoad;
	if (config["signals"]["tireLoadRR"].as<bool>()) data.tireLoadRR = info.mWheel[3].mTireLoad;
	if (config["signals"]["tireWearFL"].as<bool>()) data.tireWearFL = info.mWheel[0].mWear;
	if (config["signals"]["tireWearFR"].as<bool>()) data.tireWearFR = info.mWheel[1].mWear;
	if (config["signals"]["tireWearRL"].as<bool>()) data.tireWearRL = info.mWheel[2].mWear;
	if (config["signals"]["tireWearRR"].as<bool>()) data.tireWearRR = info.mWheel[3].mWear;
	if (config["signals"]["detached"].as<bool>()) data.detached = info.mDetached;
	if (config["signals"]["detachedFL"].as<bool>()) data.detachedFL = info.mWheel[0].mDetached;
	if (config["signals"]["detachedFR"].as<bool>()) data.detachedFR = info.mWheel[1].mDetached;
	if (config["signals"]["detachedRL"].as<bool>()) data.detachedRL = info.mWheel[2].mDetached;
	if (config["signals"]["detachedRR"].as<bool>()) data.detachedRR = info.mWheel[3].mDetached;
	if (config["signals"]["overheating"].as<bool>()) data.overheating = info.mOverheating;
	if (config["signals"]["flatFL"].as<bool>()) data.flatFL = info.mWheel[0].mFlat;
	if (config["signals"]["flatFR"].as<bool>()) data.flatFR = info.mWheel[1].mFlat;
	if (config["signals"]["flatRL"].as<bool>()) data.flatRL = info.mWheel[2].mFlat;
	if (config["signals"]["flatRR"].as<bool>()) data.flatRR = info.mWheel[3].mFlat;
	if (config["signals"]["surfaceFL"].as<bool>()) data.surfaceFL = info.mWheel[0].mSurfaceType;
	if (config["signals"]["surfaceFR"].as<bool>()) data.surfaceFR = info.mWheel[1].mSurfaceType;
	if (config["signals"]["surfaceRL"].as<bool>()) data.surfaceRL = info.mWheel[2].mSurfaceType;
	if (config["signals"]["surfaceRR"].as<bool>()) data.surfaceRR = info.mWheel[3].mSurfaceType;
	if (config["signals"]["lastImpactET"].as<bool>()) data.lastImpactET = info.mLastImpactET;
	if (config["signals"]["lastImpactMagnitude"].as<bool>()) data.lastImpactMagnitude = info.mLastImpactMagnitude;
	if (config["signals"]["lastImpactPosX"].as<bool>()) data.lastImpactPosX = info.mLastImpactPos.x;
	if (config["signals"]["lastImpactPosY"].as<bool>()) data.lastImpactPosY = info.mLastImpactPos.y;
	if (config["signals"]["lastImpactPosZ"].as<bool>()) data.lastImpactPosZ = info.mLastImpactPos.z;
	if (config["signals"]["scheduledStops"].as<bool>()) data.scheduledStops = info.mScheduledStops;
	if (config["signals"]["fuel"].as<bool>()) data.fuel = info.mFuel;
	if (config["signals"]["terrainNameFL"].as<bool>()) data.terrainNameFL = info.mWheel[0].mTerrainName;
	if (config["signals"]["terrainNameFR"].as<bool>()) data.terrainNameFR = info.mWheel[1].mTerrainName;
	if (config["signals"]["terrainNameRL"].as<bool>()) data.terrainNameRL = info.mWheel[2].mTerrainName;
	if (config["signals"]["terrainNameRR"].as<bool>()) data.terrainNameRR = info.mWheel[3].mTerrainName;

	if (sendto(sockfd, (const char*)&data, sizeof(data), 0,
		(const sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
		std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
		closesocket(sockfd);
		WSACleanup();
	}
}


void ExampleInternalsPlugin::UpdateGraphics( const GraphicsInfoV2 &info )
{

}


bool ExampleInternalsPlugin::CheckHWControl( const char * const controlName, float &fRetVal )
{
  // only if enabled, of course
  if( !mEnabled )
    return( false );

  // Note that incoming value is the game's computation, in case you're interested.

  // Sorry, no control allowed over actual vehicle inputs ... would be too easy to cheat!
  // However, you can still look at the values.

  // Note: since the game calls this function every frame for every available control, you might consider
  // doing a binary search if you are checking more than 7 or 8 strings, just to keep the speed up.
  if( _stricmp( controlName, "LookLeft" ) == 0 )
  {
    const float headSwitcheroo = fmodf( mET, 2.0f );
    if( headSwitcheroo < 0.5 )
      fRetVal = 1.0f;
    else
      fRetVal = 0.0f;
    return( true );
  }
  else if( _stricmp( controlName, "LookRight" ) == 0 )
  {
    const float headSwitcheroo = fmodf( mET, 2.0f );
    if( ( headSwitcheroo > 1.0f ) && ( headSwitcheroo < 1.5f ) )
      fRetVal = 1.0f;
    else
      fRetVal = 0.0f;
    return( true );
  }

  return( false );
}


bool ExampleInternalsPlugin::ForceFeedback( float &forceValue )
{
  // Note that incoming value is the game's computation, in case you're interested.

  // CHANGE COMMENTS TO ENABLE FORCE EXAMPLE
  return( false );

  // I think the bounds are -11500 to 11500 ...
//  forceValue = 11500.0f * sinf( mET );
//  return( true );
}


void ExampleInternalsPlugin::UpdateScoring( const ScoringInfoV2 &info )
{
	// Note: function is called twice per second now (instead of once per second in previous versions)

	if (config["signals"]["lapDistance"].as<bool>()) data.lapDistance = info.mLapDist;
	if (config["signals"]["bestS1"].as<bool>()) data.bestS1 = info.mVehicle->mBestSector1;
	if (config["signals"]["bestS2"].as<bool>()) data.bestS2 = info.mVehicle->mBestSector2;
	if (config["signals"]["bestLap"].as<bool>()) data.bestLap = info.mVehicle->mBestLapTime;
	if (config["signals"]["lastS1"].as<bool>()) data.lastS1 = info.mVehicle->mLastSector1;
	if (config["signals"]["lastS2"].as<bool>()) data.lastS2 = info.mVehicle->mLastSector2;
	if (config["signals"]["lastLap"].as<bool>()) data.lastLap = info.mVehicle->mLastLapTime;
	if (config["signals"]["currentS1"].as<bool>()) data.currentS1 = info.mVehicle->mCurSector1;
	if (config["signals"]["currentS2"].as<bool>()) data.currentS2 = info.mVehicle->mCurSector2;
	if (config["signals"]["numPitstops"].as<bool>()) data.numPitstops = info.mVehicle->mNumPitstops;
	if (config["signals"]["numPenalties"].as<bool>()) data.numPenalties = info.mVehicle->mNumPenalties;
	if (config["signals"]["driverName"].as<bool>()) data.driverName = info.mVehicle->mDriverName;
	if (config["signals"]["vehicleName"].as<bool>()) data.vehicleName = info.mVehicle->mVehicleName;
	if (config["signals"]["isPlayer"].as<bool>()) data.isPlayer = info.mVehicle->mIsPlayer;
	if (config["signals"]["whoIsInControl"].as<bool>()) data.whoIsInControl = info.mVehicle->mControl;
	if (config["signals"]["isInPit"].as<bool>()) data.isInPit = info.mVehicle->mInPits;
	if (config["signals"]["vehicleClass"].as<bool>()) data.vehicleClass = info.mVehicle->mVehicleClass;
	if (config["signals"]["deltaTimeBehind"].as<bool>()) data.deltaTimeBehind = info.mVehicle->mTimeBehindNext;
	if (config["signals"]["deltaLapsBehind"].as<bool>()) data.deltaLapsBehind = info.mVehicle->mLapsBehindNext;
	if (config["signals"]["deltaTimeLeader"].as<bool>()) data.deltaTimeLeader = info.mVehicle->mTimeBehindLeader;
	if (config["signals"]["deltaLapsLeader"].as<bool>()) data.deltaLapsLeader = info.mVehicle->mLapsBehindLeader;
	if (config["signals"]["numOfVehicles"].as<bool>()) data.numOfVehicles = info.mNumVehicles;
	if (config["signals"]["finishStatus"].as<bool>()) data.finishStatus = info.mVehicle->mFinishStatus;
	if (config["signals"]["position"].as<bool>()) data.position = info.mVehicle->mPlace;
	if (config["signals"]["maxLaps"].as<bool>()) data.maxLaps = info.mMaxLaps;
	if (config["signals"]["endingTime"].as<bool>()) data.endingTime = info.mEndET;
	if (config["signals"]["currentTime"].as<bool>()) data.currentTime = info.mCurrentET;
	if (config["signals"]["gamePhase"].as<bool>()) data.gamePhase = info.mGamePhase;
	if (config["signals"]["yellowFlagState"].as<bool>()) data.yellowFlagState = info.mYellowFlagState;
	if (config["signals"]["startLight"].as<bool>()) data.startLight = info.mStartLight;
	if (config["signals"]["numRedLights"].as<bool>()) data.numRedLights = info.mNumRedLights;
	if (config["signals"]["playerName"].as<bool>()) data.playerName = info.mPlayerName;
	if (config["signals"]["cloudDarkness"].as<bool>()) data.cloudDarkness = info.mDarkCloud;
	if (config["signals"]["rainSeverity"].as<bool>()) data.rainSeverity = info.mRaining;
	if (config["signals"]["ambientTemp"].as<bool>()) data.ambientTemp = info.mAmbientTemp;
	if (config["signals"]["trackTemp"].as<bool>()) data.trackTemp = info.mTrackTemp;
	if (config["signals"]["windSpeedX"].as<bool>()) data.windSpeedX = info.mWind.x;
	if (config["signals"]["windSpeedY"].as<bool>()) data.windSpeedY = info.mWind.y;
	if (config["signals"]["windSpeedZ"].as<bool>()) data.windSpeedZ = info.mWind.z;
	if (config["signals"]["onPathWetness"].as<bool>()) data.onPathWetness = info.mOnPathWetness;
	if (config["signals"]["offPathWetness"].as<bool>()) data.offPathWetness = info.mOffPathWetness;
}


bool ExampleInternalsPlugin::RequestCommentary( CommentaryRequestInfo &info )
{
  // COMMENT OUT TO ENABLE EXAMPLE
  return( false );

  // only if enabled, of course
  if( !mEnabled )
    return( false );

  // Note: function is called twice per second

  // Say green flag event for no particular reason every 20 seconds ...
  const float timeMod20 = fmodf( mET, 20.0f );
  if( timeMod20 > 19.0f )
  {
    strcpy( info.mName, "GreenFlag" );
    info.mInput1 = 0.0f;
    info.mInput2 = 0.0f;
    info.mInput3 = 0.0f;
    info.mSkipChecks = true;
    return( true );
  }

  return( false );
}

