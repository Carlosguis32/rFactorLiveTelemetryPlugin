//���������������������������������������������������������������������������
//�                                                                         �
//� Module: Internals Example Source File                                   �
//�                                                                         �
//� Description: Declarations for the Internals Example Plugin              �
//�                                                                         �
//�                                                                         �
//� This source code module, and all information, data, and algorithms      �
//� associated with it, are part of CUBE technology (tm).                   �
//�                           PROPRIETARY                                   �
//� Copyright (c) 1996-2006 Image Space Incorporated.  All rights reserved. �
//�                                                                         �
//�                                                                         �
//� Change history:                                                         �
//�   tag.2005.11.30: created                                               �
//�   jmm.2006.01.06: modified for public                                   �
//�                                                                         �
//���������������������������������������������������������������������������

//***************************************************************************//
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "main.hpp"
#include <math.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

//***************************************************************************//

//***************************************************************************//
// plugin information
// Change this information to siut your needs. rFactor will use this information
// to ensure that it is using the most recent version
unsigned g_uPluginID          = 2;
char     g_szPluginName[]     = "LiveTelemetryPlugin";
unsigned g_uPluginVersion     = 002;
unsigned g_uPluginObjectCount = 1;
InternalsPluginInfo g_PluginInfo;
//***************************************************************************//

//***************************************************************************//
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
//***************************************************************************//

//***************************************************************************//
// InternalsPluginInfo class
InternalsPluginInfo::InternalsPluginInfo()
{
  // put together a name for this plugin
  sprintf( m_szFullName, "%s - %s", g_szPluginName, InternalsPluginInfo::GetName() );
}
//***************************************************************************//

//***************************************************************************//
const char*    InternalsPluginInfo::GetName()     const { return ExampleInternalsPlugin::GetName(); }
const char*    InternalsPluginInfo::GetFullName() const { return m_szFullName; }
// Change this to suit your needs
const char*    InternalsPluginInfo::GetDesc()     const { return "Plugin for rFactor and Automobilista to get live telemetry plots"; }
const unsigned InternalsPluginInfo::GetType()     const { return ExampleInternalsPlugin::GetType(); }
const char*    InternalsPluginInfo::GetSubType()  const { return ExampleInternalsPlugin::GetSubType(); }
const unsigned InternalsPluginInfo::GetVersion()  const { return ExampleInternalsPlugin::GetVersion(); }
void*          InternalsPluginInfo::Create()      const { return new ExampleInternalsPlugin(); }
//***************************************************************************//

//***************************************************************************//
// InternalsPlugin class
const char ExampleInternalsPlugin::m_szName[] = "InternalsPlugin";
const char ExampleInternalsPlugin::m_szSubType[] = "Internals";
const unsigned ExampleInternalsPlugin::m_uID = 1;
const unsigned ExampleInternalsPlugin::m_uVersion = 1;
//***************************************************************************//
// UDP Socket
std::string IPAddress = "127.0.0.1";
int port = 6000;

struct SignalsData {
    float steering;
	float throttle;
	float brake;
	float gear;
	float speed;
	float accelLat;
	float accelLong;
	float roll;
	float pitch;
	float rpm;
	float fzF;
	float fzR;
	float lapIniTime;
};

WSADATA wsaData;
SOCKET sockfd;
sockaddr_in servaddr;
SignalsData data;
//***************************************************************************//

//***************************************************************************//
PluginObjectInfo *ExampleInternalsPlugin::GetInfo() { return &g_PluginInfo; }
//***************************************************************************//

//***************************************************************************//
void ExampleInternalsPlugin::Startup()
{
  // Open ports, read configs, whatever you need to do.  For now, I'll just clear out the
  // example output data.
  FILE *fo = fopen( "ExampleInternalsOutput.txt", "w" );
  fclose( fo );
  
  // default enabled to true
  mEnabled = true;

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

//***************************************************************************//

void ExampleInternalsPlugin::EnterRealtime()
{
  // start up timer every time we enter realtime
  mET = 0.0f;
}

//***************************************************************************//

void ExampleInternalsPlugin::UpdateTelemetry(const TelemInfo& info)  // This function updates the data constantly, as the game is running
{
    // Compute some auxiliary info based on the above
    TelemVect3 forwardVector = { -info.mOriX.z, -info.mOriY.z, -info.mOriZ.z };
    TelemVect3    leftVector = { info.mOriX.x,  info.mOriY.x,  info.mOriZ.z };

    // These are normalized vectors, and remember that our world Y coordinate is up.  So you can
    // determine the current pitch and roll (w.r.t. the world x-z plane) as follows:
    const float pitch = atan2f(forwardVector.y, sqrtf((forwardVector.x * forwardVector.x) + (forwardVector.z * forwardVector.z)));
    const float  roll = atan2f(leftVector.y, sqrtf((leftVector.x * leftVector.x) + (leftVector.z * leftVector.z)));
    const float radsToDeg = 57.296f;
    const float metersPerSec = sqrtf((info.mLocalVel.x * info.mLocalVel.x) +
        (info.mLocalVel.y * info.mLocalVel.y) +
        (info.mLocalVel.z * info.mLocalVel.z));


    try {
        data.steering = static_cast<int>(info.mUnfilteredSteering * 100);
        data.throttle = static_cast<int>(info.mUnfilteredThrottle * 100);
        data.brake = static_cast<int>(info.mUnfilteredBrake * 100);
        data.gear = static_cast<int>(info.mGear);
		data.speed = static_cast<int>(metersPerSec * 3.6);
		data.accelLat = static_cast<int>(info.mLocalAccel.x / 9.80665);
		data.accelLong = static_cast<int>(info.mLocalAccel.z / 9.80665);
		data.roll = static_cast<int>(roll * radsToDeg);
		data.pitch = static_cast<int>(pitch * radsToDeg);
		data.rpm = static_cast<int>(info.mEngineRPM);
		data.fzF = static_cast<int>(info.mWheel[0].mTireLoad + info.mWheel[1].mTireLoad);
		data.fzR = static_cast<int>(info.mWheel[2].mTireLoad + info.mWheel[3].mTireLoad);
		data.lapIniTime = static_cast<int>(info.mLapStartET);

        if (sendto(sockfd, (const char*)&data, sizeof(data), 0,
            (const sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
            std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
            closesocket(sockfd);
            WSACleanup();
        }
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what();
    }
}



