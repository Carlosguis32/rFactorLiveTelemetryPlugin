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

#include "Example.hpp"					// corresponding header file
#include <math.h>						// for atan2, sqrt
#include <stdio.h>						// for sample output
#include <string>						// for JSON manipulation
#include <stdio.h>						// for JSON manipulation
#include <iostream>						// for JSON manipulation
#include <fstream>						// for JSON manipulation
#include <sstream>						// parsing JSON
#include <chrono>						// obtaining current time
#include "rapidjson/document.h"			// JSON library
#include <winsock2.h>					// needed for socket UDP transmission
#include <ws2tcpip.h>					// needed for socket UDP transmission
#pragma comment(lib, "Ws2_32.lib")		// needed for socket UDP transmission

// plugin information
unsigned g_uPluginID          = 0;
char     g_szPluginName[]     = "LiveTelemetryPlugin";
unsigned g_uPluginVersion     = 001;
unsigned g_uPluginObjectCount = 1;
InternalsPluginInfo g_PluginInfo;

// UDP Socket
std::string IPAddress = "";
long port;

// JSON config file
rapidjson::Document document;

// Previous timestamp
long long previousTimestamp = 0.0f;

// Data structure to send to the server
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
	float steeringArmForce;			// Force on steering arms
	float cloudDarkness; 			// Cloud darkness (0.0 - 1.0)
	float rainSeverity; 			// Rain severity (0.0 - 1.0)
	float ambientTemp; 				// Ambient temperature
	float trackTemp; 				// Track temperature
	float windSpeedX; 				// Wind speed x-axis
	float windSpeedY; 				// Wind speed y-axis
	float windSpeedZ; 				// Wind speed z-axis
	float onPathWetness; 			// On-path wetness (0.0 - 1.0)
	float offPathWetness; 			// Off-path wetness (0.0 - 1.0)
	float speed;					// Speed
	float roll;						// Roll
	float pitch;					// Pitch
	float rpm;						// RPM
	float maxRPM;					// Max RPM
	float lapIniTime;				// Lap initial time
	float endingTime; 				// Ending time
	float currentTime; 				// Current time
	float brake;					// Brake
	float clutch;					// Clutch
	float steering;					// Steering
	float throttle;					// Throttle
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
	float lastImpactET;				// Elapsed time of the last impact
	float lastImpactMagnitude;		// Magnitude of the last impact
	float lastImpactPosX;			// Position X of the last impact
	float lastImpactPosY;			// Position Y of the last impact
	float lastImpactPosZ;			// Position Z of the last impact
	float fuel; 					// Fuel
	float deltaTimeBehind; 			// Time behind the vehicle in front
	float deltaTimeLeader; 			// Time behind the leader
	long lapNumber;					// Lap number
	long gear;						// Gear
	long deltaLapsBehind; 			// Laps behind the vehicle in front
	long deltaLapsLeader; 			// Laps behind the leader
	long numOfVehicles; 			// Number of vehicles
	long maxLaps; 					// Maximum number of laps
	short numPitstops; 				// Number of pitstops
	short numPenalties; 			// Number of penalties
	bool detached;					// Whether any part (besides wheels) is detached
	bool detachedFL;				// Whether the Front Left wheel is detached
	bool detachedFR;				// Whether the Front Right wheel is detached
	bool detachedRL;				// Whether the Rear Left wheel is detached
	bool detachedRR;				// Whether the Rear Right wheel is detached
	bool overheating;				// Whether the overheating icon is shown in the screen
	bool flatFL;					// Whether the Front Left tire is flat
	bool flatFR;					// Whether the Front Right tire is flat
	bool flatRL;					// Whether the Rear Left tire is flat
	bool flatRR;					// Whether the Rear Right tire is flat
	bool isPlayer; 					// Whether the vehicle is the player's
	bool isInPit; 					// Whether the vehicle is in the pit
	unsigned char surfaceFL;		// Surface type Front Left
	unsigned char surfaceFR;		// Surface type Front Right
	unsigned char surfaceRL;		// Surface type Rear Left
	unsigned char surfaceRR;		// Surface type Rear Right
	unsigned char scheduledStops;	// Number of scheduled stops
	unsigned char position; 		// Position on the race
	unsigned char gamePhase; 		// Game phase (0=Before session has begun, 1=Reconnaissance laps (race only), 2=Grid walk-through (race only), 3=Formation lap (race only), 4=Starting-light countdown has begun (race only), 5=Green flag, 6=Full course yellow / safety car, 7=Session stopped, 8=Session over)
	unsigned char startLight; 		// Start light frame (number depends on track)
	unsigned char numRedLights; 	// Number of red lights in the start light sequence
	signed char whoIsInControl; 	// Who is in control of the vehicle
	signed char finishStatus; 		// Finish status (0=none, 1=finished, 2=dnf, 3=dq)
	signed char yellowFlagState; 	// Yellow flag state (-1=Invalid, 0=None, 1=Pending, 2=Pits closed, 3=Pit lead lap, 4=Pits open, 5=Last lap, 6=Resume, 7=Race halt (not used))
	const char* vehicleClass; 		// Vehicle class
	const char* terrainNameFL; 		// Terrain name of FL tyre from the TDF file
	const char* terrainNameFR; 		// Terrain name of FR tyre from the TDF file
	const char* terrainNameRL; 		// Terrain name of RL tyre from the TDF file
	const char* terrainNameRR; 		// Terrain name of RR tyre from the TDF file
	const char* playerName;			// Player name
	const char* trackName;			// Track name
	const char* vehicleName;		// Vehicle name
	const char* driverName;			// Driver name
};

// Socket variables
WSADATA wsaData;
SOCKET sockfd;
sockaddr_in servaddr;

// Data to send
SignalsData data;

long long getCurrentTimestampMs() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now().time_since_epoch()
	).count();
}

bool shouldSendData() {
	long long currentTimestamp = getCurrentTimestampMs();
	int frecuenciaPorSegundo = document["frequency"][0].GetInt();

	long long intervaloMs = 1000 / frecuenciaPorSegundo;

	if (currentTimestamp >= (previousTimestamp + intervaloMs)) {
		previousTimestamp = currentTimestamp;
		return true;
	}
	return false;
}

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
	WriteToAllExampleOutputFiles("w", "-STARTUP-");

	// default enabled to true
	mEnabled = true;

	// Read JSON configuration file
	try {
		const std::string filename = "Plugins/telemetryConfig.json";

		std::ifstream file(filename);
		if (!file.is_open()) {
			MessageBeep(MB_ICONERROR);
			std::string errorMessage = "Configuration file could not be opened, check that " + filename + " exists";
			MessageBox(NULL, errorMessage.c_str(), "Error", MB_OK);
		}

		// Parse JSON file
		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string jsonContent = buffer.str();

		if (document.Parse(jsonContent.c_str()).HasParseError()) {
			MessageBeep(MB_ICONERROR);
			MessageBox(NULL, "Error parsing JSON configuration file", "Error", MB_OK);
		}

		// Get server IP from JSON configuration file
		if (document["server"]["ip"][0] != "") {
			IPAddress = document["server"]["ip"][0].GetString();
			MessageBox(NULL, IPAddress.c_str(), "IP", MB_OK);
		}
		else {
			// Default IP
			IPAddress = "127.0.0.1";
		}

		// Get server port from JSON configuration file
		if (document["server"]["port"][0] != 0) {
			port = document["server"]["port"][0].GetInt();
			MessageBox(NULL, std::to_string(port).c_str(), "Port", MB_OK);
		}
		else {
			// Default port
			port = 6000;
		}
	}
	catch (const std::exception& e) {
		MessageBeep(MB_ICONERROR);
        MessageBox(NULL, e.what(), "Error", MB_OK);
	}

	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		MessageBeep(MB_ICONERROR);
		MessageBox(NULL, "WSAStartup failed", "Error", MB_OK);
	}

	// Create socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == INVALID_SOCKET) {
		MessageBeep(MB_ICONERROR);
		MessageBox(NULL, "Socket creation failed", "Error", MB_OK);
		WSACleanup();
	}

	// Set up server address structure
	ZeroMemory(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	inet_pton(AF_INET, IPAddress.c_str(), &servaddr.sin_addr);
}

void ExampleInternalsPlugin::Shutdown()
{
  //WriteToAllExampleOutputFiles( "a", "-SHUTDOWN-" );
  // Close the socket
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

    if (shouldSendData()) {
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

		// Assign new data to data struct, checking if the signal is enabled from the JSON configuration file
		try {
			if (document["signals"]["clutchRPM"][0] == 1) data.clutchRPM = info.mClutchRPM;
			if (document["signals"]["deltaTime"][0] == 1) data.deltaTime = info.mDeltaTime;
			if (document["signals"]["engineOilTemp"][0] == 1) data.engineOilTemp = info.mEngineOilTemp;
			if (document["signals"]["engineWaterTemp"][0] == 1) data.engineWaterTemp = info.mEngineWaterTemp;
			if (document["signals"]["lapNumber"][0] == 1) data.lapNumber = info.mLapNumber;
			if (document["signals"]["lapStartET"][0] == 1) data.lapStartET = info.mLapStartET;
			if (document["signals"]["localAccelX"][0] == 1) data.localAccelX = info.mLocalAccel.x;
			if (document["signals"]["localAccelY"][0] == 1) data.localAccelY = info.mLocalAccel.y;
			if (document["signals"]["localAccelZ"][0] == 1) data.localAccelZ = info.mLocalAccel.z;
			if (document["signals"]["localRotX"][0] == 1) data.localRotX = info.mLocalRot.x;
			if (document["signals"]["localRotY"][0] == 1) data.localRotY = info.mLocalRot.y;
			if (document["signals"]["localRotZ"][0] == 1) data.localRotZ = info.mLocalRot.z;
			if (document["signals"]["localRotAccelX"][0] == 1) data.localRotAccelX = info.mLocalRotAccel.x;
			if (document["signals"]["localRotAccelY"][0] == 1) data.localRotAccelY = info.mLocalRotAccel.y;
			if (document["signals"]["localRotAccelZ"][0] == 1) data.localRotAccelZ = info.mLocalRotAccel.z;
			if (document["signals"]["localVelX"][0] == 1) data.localVelX = info.mLocalVel.x;
			if (document["signals"]["localVelY"][0] == 1) data.localVelY = info.mLocalVel.y;
			if (document["signals"]["localVelZ"][0] == 1) data.localVelZ = info.mLocalVel.z;
			if (document["signals"]["oriXX"][0] == 1) data.oriXX = info.mOriX.x;
			if (document["signals"]["oriXY"][0] == 1) data.oriXY = info.mOriX.y;
			if (document["signals"]["oriXZ"][0] == 1) data.oriXZ = info.mOriX.z;
			if (document["signals"]["oriYX"][0] == 1) data.oriYX = info.mOriY.x;
			if (document["signals"]["oriYY"][0] == 1) data.oriYY = info.mOriY.y;
			if (document["signals"]["oriYZ"][0] == 1) data.oriYZ = info.mOriY.z;
			if (document["signals"]["oriZX"][0] == 1) data.oriZX = info.mOriZ.x;
			if (document["signals"]["oriZY"][0] == 1) data.oriZY = info.mOriZ.y;
			if (document["signals"]["oriZZ"][0] == 1) data.oriZZ = info.mOriZ.z;
			if (document["signals"]["posX"][0] == 1) data.posX = info.mPos.x;
			if (document["signals"]["posY"][0] == 1) data.posY = info.mPos.y;
			if (document["signals"]["posZ"][0] == 1) data.posZ = info.mPos.z;
			if (document["signals"]["steeringArmForce"][0] == 1) data.steeringArmForce = info.mSteeringArmForce;
			if (document["signals"]["trackName"][0] == 1) data.trackName = info.mTrackName;
			if (document["signals"]["speed"][0] == 1) data.speed = metersPerSec;
			if (document["signals"]["roll"][0] == 1) data.roll = roll * radsToDeg;
			if (document["signals"]["pitch"][0] == 1) data.pitch = pitch * radsToDeg;
			if (document["signals"]["rpm"][0] == 1) data.rpm = info.mEngineRPM;
			if (document["signals"]["maxRPM"][0] == 1) data.maxRPM = info.mEngineMaxRPM;
			if (document["signals"]["lapIniTime"][0] == 1) data.lapIniTime = info.mLapStartET;
			if (document["signals"]["lapNumber"][0] == 1) data.lapNumber = info.mLapNumber;
			if (document["signals"]["brake"][0] == 1) data.brake = info.mUnfilteredBrake;
			if (document["signals"]["clutch"][0] == 1) data.clutch = info.mUnfilteredClutch;
			if (document["signals"]["steering"][0] == 1) data.steering = info.mUnfilteredSteering;
			if (document["signals"]["throttle"][0] == 1) data.throttle = info.mUnfilteredThrottle;
			if (document["signals"]["gear"][0] == 1) data.gear = info.mGear;
			if (document["signals"]["vehicleName"][0] == 1) data.vehicleName = info.mVehicleName;
			if (document["signals"]["brakeTempFL"][0] == 1) data.brakeTempFL = info.mWheel[0].mBrakeTemp;
			if (document["signals"]["brakeTempFR"][0] == 1) data.brakeTempFR = info.mWheel[1].mBrakeTemp;
			if (document["signals"]["brakeTempRL"][0] == 1) data.brakeTempRL = info.mWheel[2].mBrakeTemp;
			if (document["signals"]["brakeTempRR"][0] == 1) data.brakeTempRR = info.mWheel[3].mBrakeTemp;
			if (document["signals"]["gripFactorFL"][0] == 1) data.gripFactorFL = info.mWheel[0].mGripFract;
			if (document["signals"]["gripFactorFR"][0] == 1) data.gripFactorFR = info.mWheel[1].mGripFract;
			if (document["signals"]["gripFactorRL"][0] == 1) data.gripFactorRL = info.mWheel[2].mGripFract;
			if (document["signals"]["gripFactorRR"][0] == 1) data.gripFactorRR = info.mWheel[3].mGripFract;
			if (document["signals"]["lateralForceFL"][0] == 1) data.lateralForceFL = info.mWheel[0].mLateralForce;
			if (document["signals"]["lateralForceFR"][0] == 1) data.lateralForceFR = info.mWheel[1].mLateralForce;
			if (document["signals"]["lateralForceRL"][0] == 1) data.lateralForceRL = info.mWheel[2].mLateralForce;
			if (document["signals"]["lateralForceRR"][0] == 1) data.lateralForceRR = info.mWheel[3].mLateralForce;
			if (document["signals"]["pressureFL"][0] == 1) data.pressureFL = info.mWheel[0].mPressure;
			if (document["signals"]["pressureFR"][0] == 1) data.pressureFR = info.mWheel[1].mPressure;
			if (document["signals"]["pressureRL"][0] == 1) data.pressureRL = info.mWheel[2].mPressure;
			if (document["signals"]["pressureRR"][0] == 1) data.pressureRR = info.mWheel[3].mPressure;
			if (document["signals"]["rideHeightFL"][0] == 1) data.rideHeightFL = info.mWheel[0].mRideHeight;
			if (document["signals"]["rideHeightFR"][0] == 1) data.rideHeightFR = info.mWheel[1].mRideHeight;
			if (document["signals"]["rideHeightRL"][0] == 1) data.rideHeightRL = info.mWheel[2].mRideHeight;
			if (document["signals"]["rideHeightRR"][0] == 1) data.rideHeightRR = info.mWheel[3].mRideHeight;
			if (document["signals"]["rotationFL"][0] == 1) data.rotationFL = info.mWheel[0].mRotation;
			if (document["signals"]["rotationFR"][0] == 1) data.rotationFR = info.mWheel[1].mRotation;
			if (document["signals"]["rotationRL"][0] == 1) data.rotationRL = info.mWheel[2].mRotation;
			if (document["signals"]["rotationRR"][0] == 1) data.rotationRR = info.mWheel[3].mRotation;
			if (document["signals"]["shockDeflectionFL"][0] == 1) data.shockDeflectionFL = info.mWheel[0].mSuspensionDeflection;
			if (document["signals"]["shockDeflectionFR"][0] == 1) data.shockDeflectionFR = info.mWheel[1].mSuspensionDeflection;
			if (document["signals"]["shockDeflectionRL"][0] == 1) data.shockDeflectionRL = info.mWheel[2].mSuspensionDeflection;
			if (document["signals"]["shockDeflectionRR"][0] == 1) data.shockDeflectionRR = info.mWheel[3].mSuspensionDeflection;
			if (document["signals"]["tireTempFLI"][0] == 1) data.tireTempFLI = info.mWheel[0].mTemperature[0];
			if (document["signals"]["tireTempFLM"][0] == 1) data.tireTempFLM = info.mWheel[0].mTemperature[1];
			if (document["signals"]["tireTempFLO"][0] == 1) data.tireTempFLO = info.mWheel[0].mTemperature[2];
			if (document["signals"]["tireTempFRI"][0] == 1) data.tireTempFRI = info.mWheel[1].mTemperature[0];
			if (document["signals"]["tireTempFRM"][0] == 1) data.tireTempFRM = info.mWheel[1].mTemperature[1];
			if (document["signals"]["tireTempFRO"][0] == 1) data.tireTempFRO = info.mWheel[1].mTemperature[2];
			if (document["signals"]["tireTempRLI"][0] == 1) data.tireTempRLI = info.mWheel[2].mTemperature[0];
			if (document["signals"]["tireTempRLM"][0] == 1) data.tireTempRLM = info.mWheel[2].mTemperature[1];
			if (document["signals"]["tireTempRLO"][0] == 1) data.tireTempRLO = info.mWheel[2].mTemperature[2];
			if (document["signals"]["tireTempRRI"][0] == 1) data.tireTempRRI = info.mWheel[3].mTemperature[0];
			if (document["signals"]["tireTempRRM"][0] == 1) data.tireTempRRM = info.mWheel[3].mTemperature[1];
			if (document["signals"]["tireTempRRO"][0] == 1) data.tireTempRRO = info.mWheel[3].mTemperature[2];
			if (document["signals"]["tireLoadFL"][0] == 1) data.tireLoadFL = info.mWheel[0].mTireLoad;
			if (document["signals"]["tireLoadFR"][0] == 1) data.tireLoadFR = info.mWheel[1].mTireLoad;
			if (document["signals"]["tireLoadRL"][0] == 1) data.tireLoadRL = info.mWheel[2].mTireLoad;
			if (document["signals"]["tireLoadRR"][0] == 1) data.tireLoadRR = info.mWheel[3].mTireLoad;
			if (document["signals"]["tireWearFL"][0] == 1) data.tireWearFL = info.mWheel[0].mWear;
			if (document["signals"]["tireWearFR"][0] == 1) data.tireWearFR = info.mWheel[1].mWear;
			if (document["signals"]["tireWearRL"][0] == 1) data.tireWearRL = info.mWheel[2].mWear;
			if (document["signals"]["tireWearRR"][0] == 1) data.tireWearRR = info.mWheel[3].mWear;
			if (document["signals"]["detached"][0] == 1) data.detached = info.mDetached;
			if (document["signals"]["detachedFL"][0] == 1) data.detachedFL = info.mWheel[0].mDetached;
			if (document["signals"]["detachedFR"][0] == 1) data.detachedFR = info.mWheel[1].mDetached;
			if (document["signals"]["detachedRL"][0] == 1) data.detachedRL = info.mWheel[2].mDetached;
			if (document["signals"]["detachedRR"][0] == 1) data.detachedRR = info.mWheel[3].mDetached;
			if (document["signals"]["overheating"][0] == 1) data.overheating = info.mOverheating;
			if (document["signals"]["flatFL"][0] == 1) data.flatFL = info.mWheel[0].mFlat;
			if (document["signals"]["flatFR"][0] == 1) data.flatFR = info.mWheel[1].mFlat;
			if (document["signals"]["flatRL"][0] == 1) data.flatRL = info.mWheel[2].mFlat;
			if (document["signals"]["flatRR"][0] == 1) data.flatRR = info.mWheel[3].mFlat;
			if (document["signals"]["surfaceFL"][0] == 1) data.surfaceFL = info.mWheel[0].mSurfaceType;
			if (document["signals"]["surfaceFR"][0] == 1) data.surfaceFR = info.mWheel[1].mSurfaceType;
			if (document["signals"]["surfaceRL"][0] == 1) data.surfaceRL = info.mWheel[2].mSurfaceType;
			if (document["signals"]["surfaceRR"][0] == 1) data.surfaceRR = info.mWheel[3].mSurfaceType;
			if (document["signals"]["lastImpactET"][0] == 1) data.lastImpactET = info.mLastImpactET;
			if (document["signals"]["lastImpactMagnitude"][0] == 1) data.lastImpactMagnitude = info.mLastImpactMagnitude;
			if (document["signals"]["lastImpactPosX"][0] == 1) data.lastImpactPosX = info.mLastImpactPos.x;
			if (document["signals"]["lastImpactPosY"][0] == 1) data.lastImpactPosY = info.mLastImpactPos.y;
			if (document["signals"]["lastImpactPosZ"][0] == 1) data.lastImpactPosZ = info.mLastImpactPos.z;
			if (document["signals"]["scheduledStops"][0] == 1) data.scheduledStops = info.mScheduledStops;
			if (document["signals"]["fuel"][0] == 1) data.fuel = info.mFuel;
			if (document["signals"]["terrainNameFL"][0] == 1) data.terrainNameFL = info.mWheel[0].mTerrainName;
			if (document["signals"]["terrainNameFR"][0] == 1) data.terrainNameFR = info.mWheel[1].mTerrainName;
			if (document["signals"]["terrainNameRL"][0] == 1) data.terrainNameRL = info.mWheel[2].mTerrainName;
			if (document["signals"]["terrainNameRR"][0] == 1) data.terrainNameRR = info.mWheel[3].mTerrainName;
		}
		catch (const std::exception& e) {
			MessageBox(NULL, e.what(), "Error", MB_OK);
		}

		try {
			// Send data to server
			int result = sendto(sockfd, (const char*)&data, sizeof(data), 0, (const sockaddr*)&servaddr, sizeof(servaddr));
			if (result == SOCKET_ERROR) {
				int errorCode = WSAGetLastError();

				switch (errorCode) {
				case WSAEACCES:
					MessageBox(NULL, "Permission denied.", "Error", MB_OK | MB_ICONERROR);
					break;
				case WSAEADDRNOTAVAIL:
					MessageBox(NULL, "The specified address is not available.", "Error", MB_OK | MB_ICONERROR);
					break;
				case WSAECONNRESET:
					MessageBox(NULL, "Connection reset by peer.", "Error", MB_OK | MB_ICONERROR);
					break;
				case WSAEFAULT:
					MessageBox(NULL, "Bad address.", "Error", MB_OK | MB_ICONERROR);
					break;
				case WSAEINTR:
					MessageBox(NULL, "Interrupted function call.", "Error", MB_OK | MB_ICONERROR);
					break;
				case WSAEINVAL:
					MessageBox(NULL, "Invalid argument.", "Error", MB_OK | MB_ICONERROR);
					break;
				case WSAEISCONN:
					MessageBox(NULL, "Socket is already connected.", "Error", MB_OK | MB_ICONERROR);
					break;
				case WSAENETDOWN:
					MessageBox(NULL, "Network is down.", "Error", MB_OK | MB_ICONERROR);
					break;
				case WSAENETUNREACH:
					MessageBox(NULL, "Network is unreachable.", "Error", MB_OK | MB_ICONERROR);
					break;
				case WSAENOBUFS:
					MessageBox(NULL, "No buffer space available.", "Error", MB_OK | MB_ICONERROR);
					break;
				case WSAENOTCONN:
					MessageBox(NULL, "Socket is not connected.", "Error", MB_OK | MB_ICONERROR);
					break;
				case WSAEOPNOTSUPP:
					MessageBox(NULL, "Operation not supported.", "Error", MB_OK | MB_ICONERROR);
					break;
				case WSAESHUTDOWN:
					MessageBox(NULL, "Cannot send after socket shutdown.", "Error", MB_OK | MB_ICONERROR);
					break;
				case WSAETIMEDOUT:
					MessageBox(NULL, "Connection timed out.", "Error", MB_OK | MB_ICONERROR);
					break;
				case WSAEWOULDBLOCK:
					MessageBox(NULL, "Resource temporarily unavailable.", "Error", MB_OK | MB_ICONERROR);
					break;
				default:
					MessageBox(NULL, "Unknown error.", "Error", MB_OK | MB_ICONERROR);
					break;
				}
			}

		}
		catch (const std::exception& e) {
			MessageBeep(MB_ICONERROR);
			MessageBox(NULL, e.what(), "Error", MB_OK);

			// Close the socket in case of error
			closesocket(sockfd);
			WSACleanup();
		}
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
	// Assign new data to data struct, checking if the signal is enabled from the JSON configuration file
	try {
		if (document["signals"]["lapDistance"][0] == 1) data.lapDistance = info.mLapDist;
		if (document["signals"]["bestS1"][0] == 1) data.bestS1 = info.mVehicle->mBestSector1;
		if (document["signals"]["bestS2"][0] == 1) data.bestS2 = info.mVehicle->mBestSector2;
		if (document["signals"]["bestLap"][0] == 1) data.bestLap = info.mVehicle->mBestLapTime;
		if (document["signals"]["lastS1"][0] == 1) data.lastS1 = info.mVehicle->mLastSector1;
		if (document["signals"]["lastS2"][0] == 1) data.lastS2 = info.mVehicle->mLastSector2;
		if (document["signals"]["lastLap"][0] == 1) data.lastLap = info.mVehicle->mLastLapTime;
		if (document["signals"]["currentS1"][0] == 1) data.currentS1 = info.mVehicle->mCurSector1;
		if (document["signals"]["currentS2"][0] == 1) data.currentS2 = info.mVehicle->mCurSector2;
		if (document["signals"]["numPitstops"][0] == 1) data.numPitstops = info.mVehicle->mNumPitstops;
		if (document["signals"]["numPenalties"][0] == 1) data.numPenalties = info.mVehicle->mNumPenalties;
		if (document["signals"]["driverName"][0] == 1) data.driverName = info.mVehicle->mDriverName;
		if (document["signals"]["vehicleName"][0] == 1) data.vehicleName = info.mVehicle->mVehicleName;
		if (document["signals"]["isPlayer"][0] == 1) data.isPlayer = info.mVehicle->mIsPlayer;
		if (document["signals"]["whoIsInControl"][0] == 1) data.whoIsInControl = info.mVehicle->mControl;
		if (document["signals"]["isInPit"][0] == 1) data.isInPit = info.mVehicle->mInPits;
		if (document["signals"]["vehicleClass"][0] == 1) data.vehicleClass = info.mVehicle->mVehicleClass;
		if (document["signals"]["deltaTimeBehind"][0] == 1) data.deltaTimeBehind = info.mVehicle->mTimeBehindNext;
		if (document["signals"]["deltaLapsBehind"][0] == 1) data.deltaLapsBehind = info.mVehicle->mLapsBehindNext;
		if (document["signals"]["deltaTimeLeader"][0] == 1) data.deltaTimeLeader = info.mVehicle->mTimeBehindLeader;
		if (document["signals"]["deltaLapsLeader"][0] == 1) data.deltaLapsLeader = info.mVehicle->mLapsBehindLeader;
		if (document["signals"]["numOfVehicles"][0] == 1) data.numOfVehicles = info.mNumVehicles;
		if (document["signals"]["finishStatus"][0] == 1) data.finishStatus = info.mVehicle->mFinishStatus;
		if (document["signals"]["position"][0] == 1) data.position = info.mVehicle->mPlace;
		if (document["signals"]["maxLaps"][0] == 1) data.maxLaps = info.mMaxLaps;
		if (document["signals"]["endingTime"][0] == 1) data.endingTime = info.mEndET;
		if (document["signals"]["currentTime"][0] == 1) data.currentTime = info.mCurrentET;
		if (document["signals"]["gamePhase"][0] == 1) data.gamePhase = info.mGamePhase;
		if (document["signals"]["yellowFlagState"][0] == 1) data.yellowFlagState = info.mYellowFlagState;
		if (document["signals"]["startLight"][0] == 1) data.startLight = info.mStartLight;
		if (document["signals"]["numRedLights"][0] == 1) data.numRedLights = info.mNumRedLights;
		if (document["signals"]["playerName"][0] == 1) data.playerName = info.mPlayerName;
		if (document["signals"]["cloudDarkness"][0] == 1) data.cloudDarkness = info.mDarkCloud;
		if (document["signals"]["rainSeverity"][0] == 1) data.rainSeverity = info.mRaining;
		if (document["signals"]["ambientTemp"][0] == 1) data.ambientTemp = info.mAmbientTemp;
		if (document["signals"]["trackTemp"][0] == 1) data.trackTemp = info.mTrackTemp;
		if (document["signals"]["windSpeedX"][0] == 1) data.windSpeedX = info.mWind.x;
		if (document["signals"]["windSpeedY"][0] == 1) data.windSpeedY = info.mWind.y;
		if (document["signals"]["windSpeedZ"][0] == 1) data.windSpeedZ = info.mWind.z;
		if (document["signals"]["onPathWetness"][0] == 1) data.onPathWetness = info.mOnPathWetness;
		if (document["signals"]["offPathWetness"][0] == 1) data.offPathWetness = info.mOffPathWetness;
	}
	catch (const std::exception& e) {
		MessageBox(NULL, e.what(), "Error", MB_OK);
	}

	try {
		// Send data to server
		int result = sendto(sockfd, (const char*)&data, sizeof(data), 0, (const sockaddr*)&servaddr, sizeof(servaddr));
		if (result == SOCKET_ERROR) {
			int errorCode = WSAGetLastError();

			switch (errorCode) {
			case WSAEACCES:
				MessageBox(NULL, "Permission denied.", "Error", MB_OK | MB_ICONERROR);
				break;
			case WSAEADDRNOTAVAIL:
				MessageBox(NULL, "The specified address is not available.", "Error", MB_OK | MB_ICONERROR);
				break;
			case WSAECONNRESET:
				MessageBox(NULL, "Connection reset by peer.", "Error", MB_OK | MB_ICONERROR);
				break;
			case WSAEFAULT:
				MessageBox(NULL, "Bad address.", "Error", MB_OK | MB_ICONERROR);
				break;
			case WSAEINTR:
				MessageBox(NULL, "Interrupted function call.", "Error", MB_OK | MB_ICONERROR);
				break;
			case WSAEINVAL:
				MessageBox(NULL, "Invalid argument.", "Error", MB_OK | MB_ICONERROR);
				break;
			case WSAEISCONN:
				MessageBox(NULL, "Socket is already connected.", "Error", MB_OK | MB_ICONERROR);
				break;
			case WSAENETDOWN:
				MessageBox(NULL, "Network is down.", "Error", MB_OK | MB_ICONERROR);
				break;
			case WSAENETUNREACH:
				MessageBox(NULL, "Network is unreachable.", "Error", MB_OK | MB_ICONERROR);
				break;
			case WSAENOBUFS:
				MessageBox(NULL, "No buffer space available.", "Error", MB_OK | MB_ICONERROR);
				break;
			case WSAENOTCONN:
				MessageBox(NULL, "Socket is not connected.", "Error", MB_OK | MB_ICONERROR);
				break;
			case WSAEOPNOTSUPP:
				MessageBox(NULL, "Operation not supported.", "Error", MB_OK | MB_ICONERROR);
				break;
			case WSAESHUTDOWN:
				MessageBox(NULL, "Cannot send after socket shutdown.", "Error", MB_OK | MB_ICONERROR);
				break;
			case WSAETIMEDOUT:
				MessageBox(NULL, "Connection timed out.", "Error", MB_OK | MB_ICONERROR);
				break;
			case WSAEWOULDBLOCK:
				MessageBox(NULL, "Resource temporarily unavailable.", "Error", MB_OK | MB_ICONERROR);
				break;
			default:
				MessageBox(NULL, "Unknown error.", "Error", MB_OK | MB_ICONERROR);
				break;
			}
		}

	}
	catch (const std::exception& e) {
		MessageBeep(MB_ICONERROR);
		MessageBox(NULL, e.what(), "Error", MB_OK);

		// Close the socket in case of error
		closesocket(sockfd);
		WSACleanup();
	}
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

