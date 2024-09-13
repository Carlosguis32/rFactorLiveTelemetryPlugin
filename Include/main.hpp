//‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹
//›                                                                         ﬁ
//› Module: Internals Example Header File                                   ﬁ
//›                                                                         ﬁ
//› Description: Declarations for the Internals Example Plugin              ﬁ
//›                                                                         ﬁ
//›                                                                         ﬁ
//› This source code module, and all information, data, and algorithms      ﬁ
//› associated with it, are part of CUBE technology (tm).                   ﬁ
//›                 PROPRIETARY AND CONFIDENTIAL                            ﬁ
//› Copyright (c) 1996-2006 Image Space Incorporated.  All rights reserved. ﬁ
//›                                                                         ﬁ
//›                                                                         ﬁ
//› Change history:                                                         ﬁ
//›   tag.2005.11.30: created                                               ﬁ
//›                                                                         ﬁ
//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ

#ifndef _INTERNALS_EXAMPLE_H
#define _INTERNALS_EXAMPLE_H

#include "InternalsPlugin.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <regex>
#include <vector>
#include <sstream>


// This is used for app to find out information about the plugin
class InternalsPluginInfo : public PluginObjectInfo
{
 public:

  // Constructor/destructor
  InternalsPluginInfo();
  ~InternalsPluginInfo() {}

  // Derived from base class PluginObjectInfo
  virtual const char*    GetName()     const;
  virtual const char*    GetFullName() const;
  virtual const char*    GetDesc()     const;
  virtual const unsigned GetType()     const;
  virtual const char*    GetSubType()  const;
  virtual const unsigned GetVersion()  const;
  virtual void*          Create() const;

 private:

  char m_szFullName[128];
};


// This is used for the app to use the plugin for its intended purpose
class ExampleInternalsPlugin : public InternalsPlugin
{
 protected:
  
  const static char m_szName[];
  const static char m_szSubType[];
  const static unsigned m_uID;
  const static unsigned m_uVersion;

 public:

  // Called from class InternalsPluginInfo to return specific information about plugin
  static const char *   GetName()                     { return m_szName; }
  static const unsigned GetType()                     { return PO_INTERNALS; }
  static const char *   GetSubType()                  { return m_szSubType; }
  static const unsigned GetVersion()                  { return m_uVersion; }

  // Derived from base class PluginObject
  void                  Destroy()                     { Shutdown(); }  // poorly named ... doesn't destroy anything
  PluginObjectInfo *    GetInfo();
  unsigned              GetPropertyCount() const      { return 0; }
  PluginObjectProperty *GetProperty( const char * )   { return 0; }
  PluginObjectProperty *GetProperty( const unsigned ) { return 0; }

  // These are the functions derived from base class InternalsPlugin
  // that can be implemented.
  void Startup();         // game startup
  void Shutdown() {}      // game shutdown

  void EnterRealtime();    // entering realtime
  void ExitRealtime() {}  // exiting realtime

  // GAME OUTPUT
  bool WantsTelemetryUpdates() { return( true ); } // CHANGE TO TRUE TO ENABLE TELEMETRY EXAMPLE!
  void UpdateTelemetry( const TelemInfo &info );

  bool WantsGraphicsUpdates() { return( false ); } // CHANGE TO TRUE TO ENABLE GRAPHICS EXAMPLE!
  //void UpdateGraphics( const GraphicsInfo &info );

  // GAME INPUT
  bool HasHardwareInputs() { return( false ); } // CHANGE TO TRUE TO ENABLE HARDWARE EXAMPLE!
  void UpdateHardware( const float fDT ) { mET += fDT; } // update the hardware with the time between frames
  void EnableHardware() { mEnabled = true; }             // message from game to enable hardware
  void DisableHardware() { mEnabled = false; }           // message from game to disable hardware

  // See if the plugin wants to take over a hardware control.  If the plugin takes over the
  // control, this method returns true and sets the value of the float pointed to by the
  // second arg.  Otherwise, it returns false and leaves the float unmodified.
  //bool CheckHWControl( const char * const controlName, float &fRetVal );

  //bool ForceFeedback( float &forceValue );  // SEE FUNCTION BODY TO ENABLE FORCE EXAMPLE

 private:

  float mET;  // needed for the hardware example
  bool mEnabled; // needed for the hardware example
};

// This part of the code acts as a header for the file Example.cpp, and it defines some functions
// for the UDP socket configuration. If your code is not using a UDP socket, everything below, but the
// last line of code can be deleted

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <system_error>
#include <string>
#include <iostream>

#pragma once
#pragma comment (lib, "ws2_32")

#endif // _INTERNALS_EXAMPLE_H

