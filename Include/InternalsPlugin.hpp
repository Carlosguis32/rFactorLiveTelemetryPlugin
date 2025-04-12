#ifndef _INTERNALS_PLUGIN_HPP_
#define _INTERNALS_PLUGIN_HPP_

#include "RFPluginObjects.hpp"

// Structs to retrieve internal information (e.g. telemetry info)
struct TelemVect3 {
	float x, y, z;

	void Set( const float a, const float b, const float c )  { x = a; y = b; z = c; }
};


struct AdditionalSignals {
	float mRoll;				   // Roll angle (deg)
	float mPitch;				   // Pitch angle (deg)
	float mSpeed;				   // Speed (km/h)
};


struct TelemWheel {
	float mRotation;               // Radians/sec
	float mSuspensionDeflection;   // Meters
	float mRideHeight;             // Meters
	float mTireLoad;               // Newtons
	float mLateralForce;           // Newtons
	float mGripFract;              // An approximation of what fraction of the contact patch is sliding
	float mBrakeTemp;              // Celsius
	float mPressure;               // KPa
	float mTemperature[3];         // Celsius, left / center / right (not to be confused with inside / center / outside)
};


struct TelemWheelV2 : public TelemWheel {
	float mWear;                   // Wear (0.0 - 1.0, fraction of maximum) ... this is not necessarily proportional with grip loss
	char  mTerrainName[16];        // The material prefixes from the TDF file
	unsigned char mSurfaceType;    // 0 = dry, 1 = wet, 2 = grass, 3 = dirt, 4 = gravel, 5 = rumblestrip
	bool mFlat;                    // Whether tire is flat
	bool mDetached;                // Whether wheel is detached

	// Future use
	unsigned char mExpansion[32];
};


// Our world coordinate system is left-handed, with +y pointing up.
// The local vehicle coordinate system is as follows:
//	   +x points out the left side of the car (from the driver's perspective)
//     +y points out the roof
//     +z points out the back of the car
// 
// Rotations are as follows:
//     +x pitches up
//     +y yaws to the right
//     +z rolls to the right
struct TelemInfoBase {
	// Time
	float mDeltaTime;              // Time since last update (seconds)
	long mLapNumber;               // Current lap number
	float mLapStartET;             // Time this lap was started
	char mVehicleName[64];         // Current vehicle name
	char mTrackName[64];           // Current track name

	// Position and derivatives
	TelemVect3 mPos;               // World position in meters
	TelemVect3 mLocalVel;          // Velocity (meters / sec) in local vehicle coordinates
	TelemVect3 mLocalAccel;        // Acceleration (meters / sec^2) in local vehicle coordinates

	// Orientation and derivatives
	TelemVect3 mOriX;              // Top row of orientation matrix (also converts local vehicle vectors into world X using dot product)
	TelemVect3 mOriY;              // Mid row of orientation matrix (also converts local vehicle vectors into world Y using dot product)
	TelemVect3 mOriZ;              // Bot row of orientation matrix (also converts local vehicle vectors into world Z using dot product)
	TelemVect3 mLocalRot;          // Rotation (radians / sec) in local vehicle coordinates
	TelemVect3 mLocalRotAccel;     // Rotational acceleration (radians / sec^2) in local vehicle coordinates

	// Vehicle status
	long mGear;                    // -1 = reverse, 0 = neutral, 1+ = forward gears
	float mEngineRPM;              // Engine RPM
	float mEngineWaterTemp;        // Celsius
	float mEngineOilTemp;          // Celsius
	float mClutchRPM;              // Clutch RPM

	// Driver input
	float mUnfilteredThrottle;     // Ranges  0.0 - 1.0
	float mUnfilteredBrake;        // Ranges  0.0 - 1.0
	float mUnfilteredSteering;     // Ranges -1.0 - 1.0 (left to right)
	float mUnfilteredClutch;       // Ranges  0.0 - 1.0

	// Misc
	float mSteeringArmForce;       // Force on steering arms
};


// Re-arranged for expansion, but backwards-compatible
struct TelemInfo : public TelemInfoBase {
	TelemWheel mWheel[4];          // Wheel info (front left, front right, rear left, rear right)
};


// TelemInfoV2 contains everything in TelemInfoBase, plus the following:
struct TelemInfoV2 : public TelemInfoBase {
	// State / damage info
	float mFuel;                   // Amount of fuel (liters)
	float mEngineMaxRPM;           // Rev limit
	unsigned char mScheduledStops; // Number of scheduled pitstops
	bool  mOverheating;            // Whether overheating icon is shown
	bool  mDetached;               // Whether any parts (besides wheels) have been detached
	unsigned char mDentSeverity[8];// Dent severity at 8 locations around the car (0=none, 1=some, 2=more)
	float mLastImpactET;           // Time of last impact
	float mLastImpactMagnitude;    // Magnitude of last impact
	TelemVect3 mLastImpactPos;     // Location of last impact

	// Future use
	unsigned char mExpansion[64];

	// Keeping this at the end of the structure to make it easier to replace in future versions
	TelemWheelV2 mWheel[4];        // Wheel info (front left, front right, rear left, rear right)
};


struct GraphicsInfo {
	TelemVect3 mCamPos;            // Camera position
	TelemVect3 mCamOri;            // Camera orientation
	HWND mHWND;                    // App handle
};


// GraphicsInfoV2 contains everything in GraphicsInfo, plus the following:
struct GraphicsInfoV2 : public GraphicsInfo {
	float mAmbientRed;
	float mAmbientGreen;
	float mAmbientBlue;
};


struct VehicleScoringInfo {
	char mDriverName[32];          // Driver name
	char mVehicleName[64];         // Vehicle name
	short mTotalLaps;              // Laps completed
	signed char mSector;           // 0 = sector3, 1 = sector1, 2 = sector2
	signed char mFinishStatus;     // 0 = none, 1 = finished, 2 = dnf, 3 = dq
	float mLapDist;                // Current distance around track
	float mPathLateral;            // Lateral position with respect to *very approximate* "center" path
	float mTrackEdge;              // Track edge (w.r.t. "center" path) on same side of track as vehicle

	float mBestSector1;            // Best sector 1
	float mBestSector2;            // Best sector 2 (plus sector 1)
	float mBestLapTime;            // Best lap time
	float mLastSector1;            // Last sector 1
	float mLastSector2;            // Last sector 2 (plus sector 1)
	float mLastLapTime;            // Last lap time
	float mCurSector1;             // Current sector 1 if valid
	float mCurSector2;             // Current sector 2 (plus sector 1) if valid
	// No current laptime because it instantly becomes "last"

	short mNumPitstops;            // Number of pitstops made
	short mNumPenalties;           // Number of outstanding penalties
};


// VehicleScoringInfoV2 contains everything in VehicleScoringInfo, plus the following:
struct VehicleScoringInfoV2 : public VehicleScoringInfo {
	bool mIsPlayer;                // Is this the player's vehicle
	signed char mControl;          // Who's in control: -1 = nobody (shouldn't get this), 0 = local player, 1 = local AI, 2 = remote, 3 = replay (shouldn't get this)
	bool mInPits;                  // Between pit entrance and pit exit (not always accurate for remote vehicles)
	unsigned char mPlace;          // 1-based position
	char mVehicleClass[32];        // Vehicle class

	// Dash Indicators
	float mTimeBehindNext;         // Time behind vehicle in next higher place
	long mLapsBehindNext;          // Laps behind vehicle in next higher place
	float mTimeBehindLeader;       // Time behind leader
	long mLapsBehindLeader;        // Laps behind leader
	float mLapStartET;             // Time this lap was started

	// Position and derivatives
	TelemVect3 mPos;               // World position in meters
	TelemVect3 mLocalVel;          // Velocity (meters / sec) in local vehicle coordinates
	TelemVect3 mLocalAccel;        // Acceleration (meters / sec^2) in local vehicle coordinates

	// Orientation and derivatives
	TelemVect3 mOriX;              // Top row of orientation matrix (also converts local vehicle vectors into world X using dot product)
	TelemVect3 mOriY;              // Mid row of orientation matrix (also converts local vehicle vectors into world Y using dot product)
	TelemVect3 mOriZ;              // Bot row of orientation matrix (also converts local vehicle vectors into world Z using dot product)
	TelemVect3 mLocalRot;          // Rotation (radians / sec) in local vehicle coordinates
	TelemVect3 mLocalRotAccel;     // Rotational acceleration (radians / sec^2) in local vehicle coordinates

	// Future use
	unsigned char mExpansion[128];
};


struct ScoringInfoBase {
	char mTrackName[64];           // Current track name
	long mSession;                 // Current session
	float mCurrentET;              // Current time
	float mEndET;                  // Ending time
	long  mMaxLaps;                // Maximum laps
	float mLapDist;                // Distance around track
	char *mResultsStream;          // Results stream additions since last update (newline-delimited and NULL-terminated)

	long mNumVehicles;             // Current number of vehicles
};


// Re-arranged for expansion, but backwards-compatible
struct ScoringInfo : public ScoringInfoBase {
	VehicleScoringInfo *mVehicle;  // Array of vehicle scoring info's
};


// ScoringInfoV2 contains everything in ScoringInfoBase, plus the following:
struct ScoringInfoV2 : public ScoringInfoBase {
	// Game phases:
	// 0 Before session has begun
	// 1 Reconnaissance laps (race only)
	// 2 Grid walk-through (race only)
	// 3 Formation lap (race only)
	// 4 Starting-light countdown has begun (race only)
	// 5 Green flag
	// 6 Full course yellow / safety car
	// 7 Session stopped
	// 8 Session over
	unsigned char mGamePhase;   

	// Yellow flag states (applies to full-course only)
	// -1 Invalid
	//  0 None
	//  1 Pending
	//  2 Pits closed
	//  3 Pit lead lap
	//  4 Pits open
	//  5 Last lap
	//  6 Resume
	//  7 Race halt (not currently used)
	signed char mYellowFlagState;

	signed char mSectorFlag[3];      // Whether there are any local yellows at the moment in each sector (not sure if sector 0 is first or last, so test)
	unsigned char mStartLight;       // Start light frame (number depends on track)
	unsigned char mNumRedLights;     // Number of red lights in start sequence
	bool mInRealtime;                // In realtime as opposed to at the monitor
	char mPlayerName[32];            // Player name (including possible multiplayer override)
	char mPlrFileName[64];           // May be encoded to be a legal filename

	// Weather
	float mDarkCloud;                // Cloud darkness? 0.0 - 1.0
	float mRaining;                  // Raining severity 0.0 - 1.0
	float mAmbientTemp;              // Temperature (Celsius)
	float mTrackTemp;                // Temperature (Celsius)
	TelemVect3 mWind;                // Wind speed
	float mOnPathWetness;            // On main path 0.0 - 1.0
	float mOffPathWetness;           // On main path 0.0 - 1.0

	// Future use
	unsigned char mExpansion[256];

	VehicleScoringInfoV2 *mVehicle;  // Array of vehicle scoring info's
};


struct CommentaryRequestInfo {
	char mName[32];                  // One of the event names in the commentary INI file
	double mInput1;                  // First value to pass in (if any)
	double mInput2;                  // First value to pass in (if any)
	double mInput3;                  // First value to pass in (if any)
	bool mSkipChecks;                // Ignores commentary detail and random probability of event

	CommentaryRequestInfo() { mName[0] = 0; mInput1 = 0.0; mInput2 = 0.0; mInput3 = 0.0; mSkipChecks = false; }
};


// Plugin classes used to access internals

// Note: use class InternalsPlugin   for GetVersion()==1, or
//       use class InternalsPluginV2 for GetVersion()==2, or
//       use class InternalsPluginV3 for GetVersion()==3
class InternalsPlugin : public PluginObject {
	public:
		// General internals methods
		InternalsPlugin() {}
		virtual ~InternalsPlugin() {}

		virtual void Startup() {}                                    // Game startup
		virtual void Shutdown() {}                                   // Game shutdown

		virtual void EnterRealtime() {}                              // Entering realtime (where the vehicle can be driven)
		virtual void ExitRealtime() {}                               // Exiting realtime

		// GAME OUTPUT
		virtual bool WantsTelemetryUpdates() { return( false ); }    // Whether we want telemetry updates
		virtual void UpdateTelemetry( const TelemInfo &info ) {}     // Update plugin with telemetry info

		virtual bool WantsGraphicsUpdates() { return( false ); }     // Whether we want graphics updates
		virtual void UpdateGraphics( const GraphicsInfo &info ) {}   // Update plugin with graphics info

		// GAME INPUT
		virtual bool HasHardwareInputs() { return( false ); }        // Whether plugin has hardware plugins
		virtual void UpdateHardware( const float fDT ) {}            // Update the hardware with the time between frames
		virtual void EnableHardware() {}                             // Message from game to enable hardware
		virtual void DisableHardware() {}                            // Message from game to disable hardware

		// See if the plugin wants to take over a hardware control. If the plugin takes over the
		// control, this method returns true and sets the value of the float pointed to by the
		// second arg. Otherwise, it returns false and leaves the float unmodified
		virtual bool CheckHWControl( const char * const controlName, float &fRetVal ) { return false; }

		virtual bool ForceFeedback( float &forceValue ) { return( false ); } // Alternate force feedback computation - return true if editing the value
};


// InternalsPluginV2 contains everything in InternalsPlugin, plus the following:
class InternalsPluginV2 : public InternalsPlugin {
	public:
		// SCORING OUTPUT
		virtual bool WantsScoringUpdates() { return( false); }       // Whether we want scoring updates
		virtual void UpdateScoring( const ScoringInfo &info ) {}     // Update plugin with scoring info (approximately once per second)
};


// InternalsPluginV3 contains everything in InternalsPluginV2 (and InternalsPlugin), plus the following:
class InternalsPluginV3 : public InternalsPluginV2 {
	public:
		// SESSION NOTIFICATIONS
		virtual void StartSession() {}                               // Session started
		virtual void EndSession() {}                                 // Session ended

		// GAME OUTPUT
		virtual void UpdateTelemetry( const TelemInfoV2 &info ) {}   // Update plugin with telemetry info

		// GRAPHICS OUTPUT
		virtual void UpdateGraphics( const GraphicsInfoV2 &info ) {} // Update plugin with graphics info

		// SCORING OUTPUT
		virtual void UpdateScoring( const ScoringInfoV2 &info ) {}   // Update plugin with scoring info (approximately once per second)

		// COMMENTARY INPUT
		virtual bool RequestCommentary( CommentaryRequestInfo &info ) { return( false ); } // To use our commentary event system, fill in data and return true
};

#endif

