// Needed for winsock libraries
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "main.hpp"					// corresponding header file
#include <math.h>						// for atan2, sqrt
#include <stdio.h>						// for sample output
#include <string>						// for JSON manipulation
#include <vector>						// for JSON manipulation
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
char     g_szPluginName[]     = "rFactorTelemetryPlugin";
unsigned g_uPluginVersion     = 001;
unsigned g_uPluginObjectCount = 1;
InternalsPluginInfo g_PluginInfo;

// UDP Socket
std::vector<std::string> ipAddresses;
std::vector<std::string> ports;

// Error log file
FILE* fo;

// JSON config file
rapidjson::Document document;

// Previous timestamp
long long previousTimestamp = 0.0f;

// Shutdown flag
bool telemetryShutdown = false;

// Use default values if the JSON configuration file is not found
bool useDefaultValues = false;

// Socket variables
WSADATA wsaData;
SOCKET sockfd;
std::vector<sockaddr_in> servaddr;

// Additional signals struct (for adding custom signals to send)
AdditionalSignals additionalSignals;

long long getCurrentTimestampMs() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now().time_since_epoch()
	).count();
}

std::string getCurrentTimestampFormatted() {
	auto currentTime = std::chrono::system_clock::now();
	std::time_t currentTimeFormatted = std::chrono::system_clock::to_time_t(currentTime);

	char buf[100];
	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTimeFormatted));

	return std::string(buf);
}

bool shouldSendData(bool notAffectedByFrequency) {
	if (telemetryShutdown) {
		return false;
	}

	if (notAffectedByFrequency == true) {
		return true;
	}

	// Default frequency
	int frequencyPerSecond = 60;

	if (!useDefaultValues) {
		if (document["frequency"][0] != 0) {
			frequencyPerSecond = document["frequency"][0].GetInt();
		}
	}

	long long intervalMs = 1000 / frequencyPerSecond;
	long long currentTimestamp = getCurrentTimestampMs();

	if (currentTimestamp >= (previousTimestamp + intervalMs)) {
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
  /*FILE* fo;

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
  }*/
}


void ExampleInternalsPlugin::Startup()
{
	// Open ports, read configs, whatever you need to do.  For now, I'll just clear out the
	// example output data.
	// WriteToAllExampleOutputFiles("w", "-STARTUP-");

	// default enabled to true
	mEnabled = true;

	fo = fopen("logErrorTelemetryConfig.txt", "w+");

	// Read JSON configuration file
	const std::string filename = "Plugins/telemetryConfig.json";
	std::ifstream file(filename);

	if (!file.is_open()) {
		if (fo != nullptr) {
			fprintf(fo, "%s\n", (getCurrentTimestampFormatted() + ": Configuration file could not be opened, default values will be used, check that " + filename + " exists").c_str());
		}

		MessageBeep(MB_ICONERROR);
		std::string errorMessage = "Configuration file could not be opened, default values will be used, check that " + filename + " exists";
		MessageBox(NULL, errorMessage.c_str(), "Error", MB_OK);
		useDefaultValues = true;
	}

	// Parse JSON file
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string jsonContent = buffer.str();

	if (document.Parse(jsonContent.c_str()).HasParseError()) {
		if (fo != nullptr) {
			fprintf(fo, "%s\n", (getCurrentTimestampFormatted() + ": Error parsing JSON configuration file").c_str());
		}
		useDefaultValues = true;
	}

	// Get server IP from JSON configuration file
	if (!useDefaultValues) {
		const auto& ipsArray = document["server"]["ip"];
		if (ipsArray.IsArray() && ipsArray[0].IsArray()) {
			const auto& ips = ipsArray[0];
			for (rapidjson::SizeType i = 0; i < ips.Size(); ++i) {
				if (ips[i].IsString()) {
					ipAddresses.push_back(ips[i].GetString());
				} else {
					if (fo != nullptr) {
						fprintf(fo, "%s\n", (getCurrentTimestampFormatted() + ": Invalid IP format in JSON configuration file").c_str());
					}
				}
			}
		}
	} else {
		ipAddresses.push_back("127.0.0.1");
	}

	// Get server port from JSON configuration file
	if (!useDefaultValues) {
		const auto& portsArray = document["server"]["port"];
		if (portsArray.IsArray() && portsArray[0].IsArray()) {
			const auto& portsTemp = portsArray[0];
			for (rapidjson::SizeType i = 0; i < portsTemp.Size(); ++i) {
				if (portsTemp[i].IsString()) {
					ports.push_back(portsTemp[i].GetString());
				}
				else {
					if (fo != nullptr) {
						fprintf(fo, "%s\n", (getCurrentTimestampFormatted() + ": Invalid port format in JSON configuration file").c_str());
					}
				}
			}
		}
	}
	else {
		ports.push_back("6000");
	}

	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		if (fo != nullptr) {
			fprintf(fo, "%s\n", (getCurrentTimestampFormatted() + ": WSAStartup failed").c_str());
		}

		telemetryShutdown = true;
		MessageBeep(MB_ICONERROR);
		MessageBox(NULL, "WSAStartup failed, telemetry will not be enabled", "Error", MB_OK);
	}

	// Create socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == INVALID_SOCKET) {
		if (fo != nullptr) {
			fprintf(fo, "%s\n", (getCurrentTimestampFormatted() + ": Socket creation failed").c_str());
		}

		telemetryShutdown = true;
		MessageBeep(MB_ICONERROR);
		MessageBox(NULL, "Socket creation failed, telemetry will not be enabled", "Error", MB_OK);
		WSACleanup();
	}

	// Set up server configurations
	servaddr.resize(ipAddresses.size());
	for (size_t i = 0; i < ipAddresses.size(); ++i) {
		servaddr[i].sin_family = AF_INET;
		inet_pton(AF_INET, ipAddresses[i].c_str(), &servaddr[i].sin_addr);
		servaddr[i].sin_port = htons(static_cast<u_short>(std::stoi(ports[i])));
	}
}

void ExampleInternalsPlugin::Shutdown()
{
	//WriteToAllExampleOutputFiles( "a", "-SHUTDOWN-" );
	// Close the socket
	fclose(fo);
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

    if (shouldSendData(false)) {
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

		additionalSignals.mPitch = pitch * radsToDeg;
		additionalSignals.mRoll = roll * radsToDeg;
		additionalSignals.mSpeed = metersPerSec * 3.6;

		int sendSignalsStruct;
		int sendAdditionalSignalsStruct;

		for (u_int i = 0; i < ipAddresses.size(); i++) {
			try {
				sendSignalsStruct = sendto(sockfd, (const char*)&info, sizeof(info), 0, (const sockaddr*)&servaddr[i], sizeof(servaddr[i]));
				sendAdditionalSignalsStruct = sendto(sockfd, (const char*)&additionalSignals, sizeof(additionalSignals), 0, (const sockaddr*)&servaddr[i], sizeof(servaddr[i]));
			}
			catch (const std::exception& e) {
				if (fo != nullptr) {
					fprintf(fo, "%s\n", (getCurrentTimestampFormatted() + ": Data could not be sent to the server: " + e.what()).c_str());
				}
			}
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

void ShowErrorMessage(const char* line) {
	MessageBox(NULL, line, "Error", MB_OK | MB_ICONERROR);
}

void ExampleInternalsPlugin::UpdateScoring( const ScoringInfoV2 &info )
{
	// Note: function is called twice per second now (instead of once per second in previous versions)
	// Send data to server
	if (shouldSendData(true)) {
		size_t vehicleArraySize = info.mNumVehicles * sizeof(VehicleScoringInfoV2);
		int sendSignalsStruct;
		int sendAdditionalSignalsStruct;

		for (u_int i = 0; i < ipAddresses.size(); i++) {
			try {
				sendSignalsStruct = sendto(sockfd, (const char*)&info, sizeof(info), 0, (const sockaddr*)&servaddr[i], sizeof(servaddr[i]));
				sendAdditionalSignalsStruct = sendto(sockfd, (const char*)info.mVehicle, vehicleArraySize, 0, (const sockaddr*)&servaddr[i], sizeof(servaddr[i]));
			}
			catch (const std::exception& e) {
				if (fo != nullptr) {
					fprintf(fo, "%s\n", (getCurrentTimestampFormatted() + ": Data could not be sent to the server: " + e.what()).c_str());
				}
			}
		}

		/*if (info.mResultsStream != nullptr) {
			const char* resultsStream = info.mResultsStream;
			while (*resultsStream != '\0') {
				const char* endOfLine = strchr(resultsStream, '\n');
				if (endOfLine == nullptr) {
					endOfLine = resultsStream + strlen(resultsStream);
				}

				size_t lineLength = endOfLine - resultsStream;

				std::string line(resultsStream, lineLength);
				ShowErrorMessage(line.c_str());

				resultsStream = (*endOfLine == '\n') ? endOfLine + 1 : endOfLine;
			}
		}*/
	}
}


bool ExampleInternalsPlugin::RequestCommentary( CommentaryRequestInfo &info )
{
  // COMMENT OUT TO ENABLE EXAMPLE
  return( false );

  // only if enabled, of course
  /*if (!mEnabled)
    return( false );

  // Note: function is called twice per second

  // Say green flag event for no particular reason every 20 seconds ...
  const float timeMod20 = fmodf(mET, 20.0f);
  if( timeMod20 > 19.0f )
  {
    strcpy( info.mName, "GreenFlag" );
    info.mInput1 = 0.0f;
    info.mInput2 = 0.0f;
    info.mInput3 = 0.0f;
    info.mSkipChecks = true;
    return( true );
  }

  return( false );*/
}

