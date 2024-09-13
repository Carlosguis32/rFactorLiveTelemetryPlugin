//ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ
//İ                                                                         Ş
//İ Module: Header file for internals plugin                                Ş
//İ                                                                         Ş
//İ Description: Interface declarations for internals plugin                Ş
//İ                                                                         Ş
//İ This source code module, and all information, data, and algorithms      Ş
//İ associated with it, are part of isiMotor Technology (tm).               Ş
//İ                 PROPRIETARY AND CONFIDENTIAL                            Ş
//İ Copyright (c) 1996-2006 Image Space Incorporated.  All rights reserved. Ş
//İ                                                                         Ş
//İ Change history:                                                         Ş
//İ   tag.2005.11.29: created                                               Ş
//İ                                                                         Ş
//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

#ifndef _INTERNALS_PLUGIN_HPP_
#define _INTERNALS_PLUGIN_HPP_

#include "RFPluginObjects.hpp"

// change this variable whenever a change is made to the plugin interfaces
#define INTERNALS_PLUGIN_VERSION (0.0f)


//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//³ Structs to retrieve internal information (e.g. telemetry info)         ³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

struct TelemVect3
{
  float x, y, z;

  void Set( const float a, const float b, const float c )  { x = a; y = b; z = c; }
};


struct TelemWheel
{
  float mRotation;               // radians/sec
  float mSuspensionDeflection;   // meters
  float mRideHeight;             // meters
  float mTireLoad;               // Newtons
  float mLateralForce;           // Newtons
  float mGripFract;              // an approximation of what fraction of the contact patch is sliding
  float mBrakeTemp;              // Celsius
  float mPressure;               // kPa
  float mTemperature[3];         // Celsius, left/center/right (not to be confused with inside/center/outside!)
};


// Our world coordinate system is left-handed, with +y pointing up.
// The local vehicle coordinate system is as follows:
//   +x points out the left side of the car (from the driver's perspective)
//   +y points out the roof
//   +z points out the back of the car
// Rotations are as follows:
//   +x pitches up
//   +y yaws to the right
//   +z rolls to the right

struct TelemInfo
{
  // Time
  float mDeltaTime;              // time since last update (seconds)
  long mLapNumber;               // current lap number
  float mLapStartET;             // time this lap was started
  char mVehicleName[64];         // current vehicle name
  char mTrackName[64];           // current track name

  // Position and derivatives
  TelemVect3 mPos;               // world position in meters
  TelemVect3 mLocalVel;          // velocity (meters/sec) in local vehicle coordinates
  TelemVect3 mLocalAccel;        // acceleration (meters/sec^2) in local vehicle coordinates

  // Orientation and derivatives
  TelemVect3 mOriX;              // top row of orientation matrix (also converts local vehicle vectors into world X using dot product)
  TelemVect3 mOriY;              // mid row of orientation matrix (also converts local vehicle vectors into world Y using dot product)
  TelemVect3 mOriZ;              // bot row of orientation matrix (also converts local vehicle vectors into world Z using dot product)
  TelemVect3 mLocalRot;          // rotation (radians/sec) in local vehicle coordinates
  TelemVect3 mLocalRotAccel;     // rotational acceleration (radians/sec^2) in local vehicle coordinates

  // Vehicle status
  long mGear;                    // -1=reverse, 0=neutral, 1+=forward gears
  float mEngineRPM;              // engine RPM
  float mEngineWaterTemp;        // Celsius
  float mEngineOilTemp;          // Celsius
  float mClutchRPM;              // clutch RPM

  // Driver input
  float mUnfilteredThrottle;     // ranges  0.0-1.0
  float mUnfilteredBrake;        // ranges  0.0-1.0
  float mUnfilteredSteering;     // ranges -1.0-1.0 (left to right)
  float mUnfilteredClutch;       // ranges  0.0-1.0

  // Wheel info (front left, front right, rear left, rear right)
  float mSteeringArmForce;       // force on steering arms
  TelemWheel mWheel[4];
};


struct GraphicsInfo              // may be expanded in the future to provide interfaces for drawing onscreen
{
  TelemVect3 mCamPos;            // camera position
  TelemVect3 mCamOri;            // camera orientation
  HWND mHWND;                    // app handle
};


//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//³ Plugin used to access internals                                        ³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

class InternalsPlugin : public PluginObject
{
 public:

  // General internals methods
  InternalsPlugin() {}
  virtual ~InternalsPlugin() {}

  virtual void Startup() {}                                    // game startup
  virtual void Shutdown() {}                                   // game shutdown

  virtual void EnterRealtime() {}                              // entering realtime (where the vehicle can be driven)
  virtual void ExitRealtime() {}                               // exiting realtime

  // GAME OUTPUT
  virtual bool WantsTelemetryUpdates() { return( true ); }     // whether we want telemetry updates
  virtual void UpdateTelemetry( const TelemInfo &info ) {}     // update plugin with telemetry info

  virtual bool WantsGraphicsUpdates() { return( false ); }     // whether we want graphics updates
  virtual void UpdateGraphics( const GraphicsInfo &info ) {}   // update plugin with graphics info

  // GAME INPUT
  virtual bool HasHardwareInputs() { return( false ); }        // whether plugin has hardware plugins
  virtual void UpdateHardware( const float fDT ) {}            // update the hardware with the time between frames
  virtual void EnableHardware() {}                             // message from game to enable hardware
  virtual void DisableHardware() {}                            // message from game to disable hardware

  // See if the plugin wants to take over a hardware control.  If the plugin takes over the
  // control, this method returns true and sets the value of the float pointed to by the
  // second arg.  Otherwise, it returns false and leaves the float unmodified.
  virtual bool CheckHWControl( const char * const controlName, float &fRetVal ) { return false; }

  virtual bool ForceFeedback( float &forceValue ) { return( false ); } // alternate force feedback computation - return true if editing the value
};


//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

#endif // _INTERNALS_PLUGIN_HPP_

