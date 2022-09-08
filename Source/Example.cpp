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
//› Copyright (c) 1996-2014 Image Space Incorporated.  All rights reserved. ﬁ
//›                                                                         ﬁ
//›                                                                         ﬁ
//› Change history:                                                         ﬁ
//›   tag.2005.11.30: created                                               ﬁ
//›                                                                         ﬁ
//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ

#include "Example.hpp"          // corresponding header file
#include <math.h>               // for atan2, sqrt
#include <stdio.h>              // for sample output
// #include<dinput.h>

#include <limits>
#include <string>         // std::string
// socket
//#include <ws2tcpip.h>
// thread
#include <process.h>
#pragma comment(lib, "Ws2_32.lib")

#include <ctime>

// variables
// INI
long waitsec; // minimum camera change time
long interest; // interesting positions (1 to interest) more chance to view
double obtime; // difference to chance onboard camera (sec)
long interestsec; // interesting difference on race
long automatic; // auto camera
long walkthrough; // grid and finish walk through
char showinpit[256];
long showinpitl;
char camtest[10];
long obcam;
long rvcam;
long rearview; // chance onboard to rearview
std::string inifilename;
char filespath[256];
std::string sfilename;
char seged[256];
double lowinc; // low incident. show when everyone out of close gaps
double highinc; // high incident. show always


std::string sseged;
char* e;
MessageInfoV01 message;
long refreshcount;
long camvalthat; // randomized camera changed time
bool scoringrun; // updatescoring running
long needpos;
long needspos;
long needdpos;
long pitpos;
long needveh;
long rvveh;
long first;
long needcam;
long lastcam;
long aktveh;
long aktpos;
long sbs; // side by side
long needsbspos;
long maxsbs;
double minbehind; // legkisebb k¸lˆnbsÈg kerekitett
double pontosminbehind; // legkisebb k¸lˆnbsÈg pontosan
double aktbehind; // k¸lˆnbsÈg
double sessiontime;
double camvalttime;
bool allbox;
bool allfinished;
long finished; // last finished
short obchance;
bool inpit;

double bestlapT;
double best1T;
double best2T;
double curlapT;

int autokey;
bool autokeypressed;
int here;

// replay (incident) variables
bool needreplay;
bool stopreplay;
bool onreplay;
double inctime; // next to show incident time
double incsize;
long replayveh;
double pinctime; // possible to show incident time
double pincsize;
long preplayveh;
double replaystarted;
bool replayset;
char sincsize[10];

// info to files
std::string driverfname;
std::string timefname;
std::string listfname;
char aktname[32];
char replayname[32];
char elso[32];
long completedlaps;
long currentlap;
long numveh;



// plugin information

extern "C" __declspec( dllexport )
const char * __cdecl GetPluginName()                   { return( "rF2 autocam - 2015.09.20." ); }

extern "C" __declspec( dllexport )
PluginObjectType __cdecl GetPluginType()               { return( PO_INTERNALS ); }

extern "C" __declspec( dllexport )
int __cdecl GetPluginVersion()                         { return( 7 ); } // InternalsPluginV01 functionality (if you change this return value, you must derive from the appropriate class!)

extern "C" __declspec( dllexport )
PluginObject * __cdecl CreatePluginObject()            { return((PluginObject *) new rF2autocam); }

extern "C" __declspec( dllexport )
void __cdecl DestroyPluginObject(PluginObject *obj)  { delete((rF2autocam *)obj); }


// ExampleInternalsPlugin class

void rF2autocam::WritetoFileDrivername()
{
	FILE *fdriver = fopen(driverfname.c_str(), "w");
	if (fdriver != NULL)
	{
		if (onreplay)
		{
			fprintf(fdriver, "%s", replayname);
		}
		else
		{
			fprintf(fdriver, "%d. %s", aktpos, aktname);
		}
		fclose(fdriver);
	}
}

void rF2autocam::WritetoInfohtml(long session)
{
	FILE *flist = fopen(listfname.c_str(), "w");
	if (flist != NULL)
	{
		fprintf(flist, "<!DOCTYPE html>");
		fprintf(flist, "<html>");
		fprintf(flist, "<head>");
		fprintf(flist, "<meta http-equiv=\"Content - Type\" content=\"text / html; charset = utf - 8\">");
		fprintf(flist, "<meta http-equiv=\"refresh\" content=\"0.5\">");
		fprintf(flist, "<style>");
		fprintf(flist, "	body {");
		fprintf(flist, "		font-size: 12pt;");
		fprintf(flist, "		font-family:Sans-serif;");
		fprintf(flist, "		font-weight: bold;");
		fprintf(flist, "	}");
		fprintf(flist, "	tr {		");
		fprintf(flist, "		background-size: 100% 100%;");
		fprintf(flist, "	}");
		fprintf(flist, "	td.head {");
		fprintf(flist, "		// background-image: url('Driver.png');");
		fprintf(flist, "		// background-repeat: no-repeat;");
		fprintf(flist, "		background-color: lightgray;");
		fprintf(flist, "		color: black;");
		fprintf(flist, "		text-align: center;			");
		fprintf(flist, "	}");
		fprintf(flist, "	td.value {");
		fprintf(flist, "		background-color: black;");
		fprintf(flist, "		color: white;");
		fprintf(flist, "		text-align: right;");
		fprintf(flist, "	}");
		fprintf(flist, "</style>");
		fprintf(flist, "</head>");
		fprintf(flist, "<body>");
		if (!onreplay)		
		{
			if (session < 10)
			{
				fprintf(flist, "<table>");
				fprintf(flist, "<tr>");
				fprintf(flist, "<td class=\"head\" width=\"45px\">Best:</td>");
				fprintf(flist, "<td class=\"value\" width=\"65px\">%.3f</td>", best1T);
				fprintf(flist, "<td class=\"value\" width=\"65px\">%.3f</td>", (best2T-best1T));
				fprintf(flist, "<td class=\"value\" width=\"65px\">%.3f</td>", (bestlapT-best2T));
				fprintf(flist, "<td class=\"value\" width=\"80px\">%.0f:%06.3f</td>", floor((bestlapT / 60)), fmod(bestlapT, 60));
				fprintf(flist, "</tr>");
				fprintf(flist, "<tr>");
				fprintf(flist, "<td class=\"head\" width=\"45px\">Dif:</td>");
				fprintf(flist, "<td class=\"value\" width=\"65px\">%.3f</td>");
				fprintf(flist, "<td class=\"value\" width=\"65px\">%.3f</td>");
				fprintf(flist, "<td class=\"value\" width=\"65px\">%.3f</td>");
				fprintf(flist, "<td class=\"value\" width=\"80px\">%.0f:%06.3f</td>");
				fprintf(flist, "</tr>");
				fprintf(flist, "<tr>");
				fprintf(flist, "<td class=\"head\" width=\"45px\">Curr:</td>");
				fprintf(flist, "<td class=\"value\" width=\"65px\">199.999</td>");
				fprintf(flist, "<td class=\"value\" width=\"65px\">199.999</td>");
				fprintf(flist, "<td class=\"value\" width=\"65px\">199.999</td>");
				fprintf(flist, "<td class=\"value\" width=\"80px\">%.0f:%06.3f</td>", floor(curlapT / 60), fmod(curlapT, 60));
				fprintf(flist, "</tr>");
				fprintf(flist, "</table>");
			}
		}
		fprintf(flist, "</body>");
		fprintf(flist, "</html>");
		fclose(flist);
	}
}

void rF2autocam::SetEnvironment(const EnvironmentInfoV01 &info)
{
	strcpy(seged, "");
	// as the SetEnvironment can be called several times at launch                  
	/* if (environmentAlreadySet) {
	return;
	} */
	// retrieve the ini filename (full path)
	std::string str = info.mPath[1];
	unsigned found = str.find_last_of("/\\");
	str = str.substr(0, found);
	str.append("\\rF2autocam.ini");
	inifilename = str;
	GetPrivateProfileString("AUTOCAM", "auto", "a", seged, 255, str.c_str());
	automatic = strtol(seged, &e, 0);
	if (0 == waitsec && seged == e) {
		automatic = 1;
		WritePrivateProfileString("AUTOCAM", "auto", "1", str.c_str());
	}
	GetPrivateProfileString("AUTOCAM", "autokey", "a", seged, 255, str.c_str());
	autokey = strtol(seged, &e, 0);
	if (0 == waitsec && seged == e) {
		autokey = 0x41;
		WritePrivateProfileString("AUTOCAM", "autokey", "0x41", str.c_str());
	}
	GetPrivateProfileString("AUTOCAM", "waitsec", "a", seged, 255, str.c_str());
	waitsec = strtol(seged, &e, 0);
	if (0 == waitsec && seged == e) {
		waitsec = 15;
		WritePrivateProfileString("AUTOCAM", "waitsec", "15", str.c_str());		
	}	
	GetPrivateProfileString("AUTOCAM", "interest", "a", seged, 255, str.c_str());
	interest = strtol(seged, &e, 0);
	if (0 == interest && seged == e) {
		interest = 12;		
		WritePrivateProfileString("AUTOCAM", "interest", "12", str.c_str());
	}
	GetPrivateProfileString("AUTOCAM", "interestdiff", "a", seged, 255, str.c_str());
	interestsec = strtol(seged, &e, 0);
	if (0 == interestsec && seged == e) {
		interestsec = 3;
		WritePrivateProfileString("AUTOCAM", "interestdiff", "3", str.c_str());
	}
	GetPrivateProfileString("AUTOCAM", "onboarddiff", "a", seged, 255, str.c_str());
	obtime = strtod(seged, &e);
	if (0 == obtime && seged == e) {
		obtime = 0.4;
		WritePrivateProfileString("AUTOCAM", "onboarddiff", "0.4", str.c_str());
	}
	GetPrivateProfileString("AUTOCAM", "onboardcam", "a", seged, 255, str.c_str());
	obcam = strtol(seged, &e, 0);
	if (0 == obcam && seged == e) {
		obcam = 0;
		WritePrivateProfileString("AUTOCAM", "onboardcam", "0", str.c_str());
	}
	GetPrivateProfileString("AUTOCAM", "rearview", "a", seged, 255, str.c_str());
	rearview = strtol(seged, &e, 0);
	if (0 == rearview && seged == e) {
		rearview = 0;
		WritePrivateProfileString("AUTOCAM", "rearview", "0", str.c_str());
	}
	GetPrivateProfileString("AUTOCAM", "rearviewcam", "a", seged, 255, str.c_str());
	rvcam = strtol(seged, &e, 0);
	if (0 == rvcam && seged == e) {
		rvcam = 6;
		WritePrivateProfileString("AUTOCAM", "rearviewcam", "6", str.c_str());
	}
	strcpy(camtest, "");
	GetPrivateProfileString("AUTOCAM", "camtest", "0", camtest, 255, str.c_str());
	strcpy(seged, "");
	if (_stricmp(camtest, "0") == 0) {
		strcpy(camtest, "no");
		WritePrivateProfileString("AUTOCAM", "camtest", "no", str.c_str());
	}
	GetPrivateProfileString("AUTOCAM", "walkthrough", "a", seged, 255, str.c_str());
	walkthrough = strtol(seged, &e, 0);
	if (0 == walkthrough && seged == e) {
		walkthrough = 1;
		WritePrivateProfileString("AUTOCAM", "walkthrough", "1", str.c_str());
	}
	strcpy(showinpit, "");
	GetPrivateProfileString("AUTOCAM", "showinpit", "0", showinpit, 255, str.c_str());
	strcpy(seged, "");
	if (_stricmp(showinpit,"0") == 0) {
		strcpy(showinpit,"interestdiff");
		WritePrivateProfileString("AUTOCAM", "showinpit", "interestdiff", str.c_str());
	}
	showinpitl = strtol(showinpit, &e, 0);
	GetPrivateProfileString("AUTOCAM", "lowinc", "a", seged, 255, str.c_str());
	lowinc = strtod(seged, &e);
	if (0 == lowinc && seged == e) {
		lowinc = 0.4;
		WritePrivateProfileString("AUTOCAM", "lowinc", "0.4", str.c_str());
	}
	GetPrivateProfileString("AUTOCAM", "highinc", "a", seged, 255, str.c_str());
	highinc = strtod(seged, &e);
	if (0 == highinc && seged == e) {
		highinc = 0.8;
		WritePrivateProfileString("AUTOCAM", "highinc", "0.8", str.c_str());
	}
	strcpy(filespath, "");
	GetPrivateProfileString("AUTOCAM", "filespath", "0", filespath, 255, str.c_str());
	strcpy(seged, "");
	if (_stricmp(filespath, "0") == 0) {
		strcpy(filespath, "c:\\rF2stream");
		WritePrivateProfileString("AUTOCAM", "filespath", "c:\\rF2stream", str.c_str());
	}
	// environmentAlreadySet = true;
}

void rF2autocam::Startup( long version )
{
  // Open ports, read configs, whatever you need to do.  For now, I'll just clear out the
  // example output data files.
	srand(time(NULL));

  // default HW control enabled to true
  mEnabled = true;
  message.mDestination = 0;
  message.mTranslate = 0;
  if (automatic) {
	  strcpy(message.mText, "rF2autocam 2015.09.20. Auto camera: on");	  
  }
  else {
	  strcpy(message.mText, "rF2autocam 2015.09.20. Auto camera: off");	  
  }
  camvalttime = 0;
  needpos = 0;
  needveh = 0;
  aktveh = -1;
  aktpos = 0;
  needcam = 4;
  refreshcount = 0;
  bestlapT = 2147483640;
  best1T = 2147483640;
  best2T = 2147483640;
  inpit = false;
  sbs = 0;
  maxsbs = 0;
  needreplay = false;
  onreplay = false;
  stopreplay = false;
  replaystarted = 0;
  replayset = false;
  replayveh = -1;
  inctime = 0;
  incsize = 0;
  preplayveh = -1;
  pinctime = 0;
  pincsize = 0;  
  completedlaps = 0;
  currentlap = 0;
  strcpy(aktname, "");
  strcpy(elso, "");

  // out files defaults  
  sfilename = "";
  sfilename.append(filespath);
  sfilename.append("\\time.txt");
  timefname = sfilename;
  FILE *ftime = fopen(timefname.c_str(), "w");
  if (ftime != NULL)
  {
	  fprintf(ftime, "-");
	  fclose(ftime);
  }
  sfilename = "";
  sfilename.append(filespath);
  sfilename.append("\\driver.txt");
  driverfname = sfilename;
  FILE *fdriver = fopen(driverfname.c_str(), "w");
  if (fdriver != NULL)
  {
	  fprintf(fdriver, "rF2autocam");
	  fclose(fdriver);
  }
  sfilename = "";
  sfilename.append(filespath);
  sfilename.append("\\info.html");
  listfname = sfilename;
  FILE *flist = fopen(listfname.c_str(), "w");
  if (flist != NULL)  
  {	  
	  fclose(flist);
  }
  WritetoInfohtml(0);
}


void rF2autocam::Shutdown()
{
  
}


void rF2autocam::StartSession()
{
	camvalttime = 0;
	needpos = 0;
	needveh = 0;
	aktveh = -1;
	aktpos = 0;
	needcam = 4;
	refreshcount = 0;
	bestlapT = 2147483640;
	best1T = 2147483640;
	best2T = 2147483640;
	inpit = false;
	sbs = 0;
	maxsbs = 0;
	needreplay = false;
	onreplay = false;
	stopreplay = false;
	replaystarted = 0;
	replayset = false;
	replayveh = -1;
	inctime = 0;
	incsize = 0;
	preplayveh = -1;
	pinctime = 0;
	pincsize = 0;
	completedlaps = 0;
	currentlap = 0;
	strcpy(aktname, "");
	strcpy(elso, "");

	// out files defaults  	
	FILE *ftime = fopen(timefname.c_str(), "w");
	if (ftime != NULL)
	{
		fprintf(ftime, "-");
		fclose(ftime);
	}	
	FILE *fdriver = fopen(driverfname.c_str(), "w");
	if (fdriver != NULL)
	{
		fprintf(fdriver, "rF2autocam");
		fclose(fdriver);
	}	
	FILE *flist = fopen(listfname.c_str(), "w");
	if (flist != NULL)
	{		
		fclose(flist);
	}
	WritetoInfohtml(0);
}


void rF2autocam::EndSession()
{
  
}


void rF2autocam::EnterRealtime()
{
  // start up timer every time we enter realtime
  mET = 0.0;
  
}


void rF2autocam::ExitRealtime()
{
  
}


void rF2autocam::UpdateTelemetry( const TelemInfoV01 &info )
{
  // Use the incoming data, for now I'll just write some of it to a file to a) make sure it
  // is working, and b) explain the coordinate system a little bit (see header for more info) 
  
}


void rF2autocam::UpdateGraphics( const GraphicsInfoV01 &info )
{
  // Use the incoming data, for now I'll just write some of it to a file to a) make sure it
  // is working, and b) explain the coordinate system a little bit (see header for more info)
}

bool key_pressed(int pKeyCode)
{
	return (GetAsyncKeyState(pKeyCode) & 0x8000) != 0;
}

bool rF2autocam::CheckHWControl( const char * const controlName, double &fRetVal )
{
  // only if enabled, of course
  if( !mEnabled )
    return( false );

  // Note that incoming value is the game's computation, in case you're interested.

  // Sorry, no control allowed over actual vehicle inputs ... would be too easy to cheat!
  // However, you can still look at the values.

  // Note: since the game calls this function every frame for every available control, you might consider
  // doing a binary search if you are checking more than 7 or 8 strings, just to keep the speed up.
  if (key_pressed(VK_CONTROL))
  {
	  if (key_pressed(autokey)) {
		  message.mDestination = 0;
		  message.mTranslate = 0;
		  if (!autokeypressed) {
			  if (automatic) {
				  automatic = false;
				  strcpy(message.mText, "Auto camera: off");
			  }
			  else {
				  automatic = true;
				  strcpy(message.mText, "Auto camera: on");
			  }			  
		  }
		  autokeypressed = true;
//		  return (true);
	  }
	  else { autokeypressed = false; }
/*	  if (key_pressed(73)) // i - testing for replays
	  {
		  needreplay = true;
		  inctime = sessiontime;
		  replayveh = 5;
	  } */
  }
  if ((_stricmp(controlName, "InstantReplay") == 0) && (sessiontime > (inctime + 10)) && (needreplay && !onreplay))
  {
	  fRetVal = 1.0f;
	  onreplay = true;
	  needreplay = false;
	  replaystarted = sessiontime;
	  replayset = false;	  
	  return (true);
  }
  if ((_stricmp(controlName, "InstantReplay") == 0) && (onreplay && stopreplay))
  {
	  stopreplay = false;
	  onreplay = false;
	  replayset = false;
	  needreplay = false;
	  fRetVal = 1.0f;
	  return (true);
  }
  return(false);
}


bool rF2autocam::ForceFeedback( double &forceValue )
{
  // Note that incoming value is the game's computation, in case you're interested.
#if 0 // enable to log it out (note that this is a very very slow implementation)
  FILE *fo = fopen( "FFB.txt", "a" );
  if( fo != NULL )
  {
    fprintf( fo, "\nFFB=%.4f", forceValue );
    fclose( fo );
  }
#endif

  // CHANGE COMMENTS TO ENABLE FORCE EXAMPLE
  return( false );

  // I think the bounds are -11500 to 11500 ...
//  forceValue = 11500.0 * sinf( mET );
//  return( true );
}


void rF2autocam::UpdateScoring(const ScoringInfoV01 &info)
{	
	strcpy(seged, "");
	char* e;
	here = 0;
	// Note: function is called twice per second now (instead of once per second in previous versions)
	if (automatic != 0) // auto mode on
	{		
		scoringrun = true;
		sessiontime = info.mCurrentET;
		finished = 0;
		allbox = true;
		allfinished = true;		
		camvalthat = waitsec + (rand() % 5);
		if (info.mNumVehicles > 0) {
			minbehind = 99999;
			pontosminbehind = obtime + 1;
			numveh = info.mNumVehicles;
			needpos = 0;
			needspos = 0;
			needdpos = 0;
			needsbspos = 0;
			inpit = false;
			maxsbs = 0;
			for (long i = 0; i < info.mNumVehicles; ++i) // prelist: bestlap, bestsector, first, allfinished etc...
			{
				VehicleScoringInfoV01 &vinfo = info.mVehicle[i];
				if (vinfo.mPlace == 1)
				{
					first = vinfo.mID;
					strcpy(elso, vinfo.mDriverName);
					completedlaps = vinfo.mTotalLaps;
				}				
				if (vinfo.mFinishStatus == 0) { allfinished = false; }
				if ((vinfo.mBestLapTime < bestlapT) && (vinfo.mBestLapTime > 0))
				{
					bestlapT = vinfo.mBestLapTime;
					best1T = vinfo.mBestSector1;
					best2T = vinfo.mBestSector2;					
				}				
			}
			if (info.mSession < 10) { //Practie, qualy, warmup etc
				for (long i = 0; i < info.mNumVehicles; ++i) // searching who is better lap
				{
					VehicleScoringInfoV01 &vinfo = info.mVehicle[i];					
					if ((vinfo.mCurSector2 > 0) && ((vinfo.mCurSector2+vinfo.mCurSector1) < (best1T+best2T)) && ((needdpos > vinfo.mPlace) || (needdpos == 0)))
					{
						if ((sessiontime - camvalttime) >= camvalthat) {
							needdpos = vinfo.mPlace;
						}
						pontosminbehind = (float) 0.01; // hogy valthasson onboardra
					}
					if ((vinfo.mCurSector1 > 0) && (vinfo.mCurSector2 == 0) && (vinfo.mCurSector1 < best1T) && ((needspos > vinfo.mPlace) || (needspos == 0)))
					{
						if ((sessiontime - camvalttime) >= camvalthat) {
							needspos = vinfo.mPlace;
						}
						pontosminbehind = (float) 0.01; // hogy valthasson onboardra
					}
					if ((vinfo.mCurSector1 > 0) && ((needpos > vinfo.mPlace) || (needpos == 0)))
					{
						if ((sessiontime - camvalttime) >= camvalthat) {
							needpos = vinfo.mPlace;
						}
						pontosminbehind = (float) 0.01; // hogy valthasson onboardra
					}
				}
				if (needspos != 0) { needpos = needspos; }
				if (needdpos != 0) { needpos = needdpos; }
				if (needpos == 0) {
					for (long i = 0; i < info.mNumVehicles; ++i) // searching who is on track
					{
						VehicleScoringInfoV01 &vinfo = info.mVehicle[i];
						if (((vinfo.mPitState == 0) || (vinfo.mPitState == 4)) && ((needpos > vinfo.mPlace) || (needpos == 0))) {
							needpos = vinfo.mPlace; 
						}
					}
				}
			} // Time attack end
			if (info.mSession > 9) { // race
				for (long i = 0; i < info.mNumVehicles; ++i) // searching minimum difference
				{
					VehicleScoringInfoV01 &vinfo = info.mVehicle[i];
					if ((vinfo.mFinishStatus == 1) && (finished < vinfo.mPlace)) { finished = vinfo.mPlace; }					
					if ((vinfo.mPitState == 0) && (vinfo.mFinishStatus == 0))
					{
						if ((vinfo.mPlace > interest) && (vinfo.mTimeBehindNext > obtime))
						{
							aktbehind = round((abs(vinfo.mTimeBehindNext + 0.5)/3) * 10);
						}
						else
						{
							aktbehind = round((abs(vinfo.mTimeBehindNext)/3) * 10);
						}
						sbs = 0;
						for (long j = 0; j < info.mNumVehicles; ++j) // searching side by side
						{
							VehicleScoringInfoV01 &vinfosbs = info.mVehicle[j];
							if (abs(vinfo.mLapDist - vinfosbs.mLapDist) < 0.2) { sbs++;	}
						}
						if ((info.mGamePhase == 5) || (info.mGamePhase == 4)) // Green Flag
						{
							if ((aktbehind <= minbehind) && (vinfo.mTimeBehindNext != 0))
							{
								if (aktbehind == minbehind)
								{
									if ((vinfo.mPlace < needpos) || (needpos == 0))
									{
										minbehind = aktbehind;
										pontosminbehind = abs(vinfo.mTimeBehindNext);
										needpos = vinfo.mPlace;
									}
								}
								else
								{
									minbehind = aktbehind;
									pontosminbehind = abs(vinfo.mTimeBehindNext);
									needpos = vinfo.mPlace;
								}
							}
							if ((sbs > maxsbs) && ( sbs > 1) && ((needsbspos == 0) || (vinfo.mPlace < needsbspos)))
							{
								maxsbs = sbs;
								needsbspos = vinfo.mPlace;								
							}
						}
						else if (info.mGamePhase == 3) { // cicling on formation lap							
							if ((aktpos < info.mNumVehicles) && (walkthrough == 1)) { needpos = aktpos + 1; }
							else { needpos = 1; }
						}
						else { 
							camvalthat = 2;
							needpos = 1;							
						}
					}
				} // difference check end
				if (maxsbs > 2) { // cars side by side
					needpos = needsbspos;
					pontosminbehind = obtime + 1; // disable onboard
				}
				// SHOWINPIT
				inpit = false;
				if (((_stricmp(showinpit,"interestdiff") == 0) && (pontosminbehind > interestsec)) // showinpit interestdiff
					|| ((_stricmp(showinpit,"onboarddiff") == 0) && (pontosminbehind > obtime)) // showinpit onboard					
					|| (_stricmp(showinpit,"always") == 0)) // showinpit always
				{
					pitpos = 0;					
					for (long i = 0; i < info.mNumVehicles; ++i)
					{
						VehicleScoringInfoV01 &vinfo = info.mVehicle[i];
						if (((vinfo.mPitState == 2) || (vinfo.mPitState == 4)) && (vinfo.mFinishStatus == 0) && ((pitpos == 0) || (pitpos > vinfo.mPlace))) {
							inpit = true;
							pitpos = vinfo.mPlace;							
						}
					}
				} else
				if (showinpitl > 0) { // show top<n>
					pitpos = 0;
					for (long i = 0; i < info.mNumVehicles; ++i)
					{
						VehicleScoringInfoV01 &vinfo = info.mVehicle[i];
						if ((vinfo.mPitState > 1) && (vinfo.mFinishStatus == 0) && ((pitpos == 0) || (pitpos > vinfo.mPlace)) && (vinfo.mPlace <= showinpitl)) {
							inpit = true;
							pitpos = vinfo.mPlace;							
						}
					}
				}
				if (inpit && (pitpos != 0)) {
					needpos = pitpos;
					pontosminbehind = obtime + 1; // disable onboard
				}
				// SHOWINPIT END
				if (pontosminbehind > interestsec) {					
					if (((sessiontime - camvalttime) >= camvalthat) && (!inpit))
					{
						if ((rand() % 1) == 1) { needpos = rand() % info.mNumVehicles; } //random vehichle
					}
				}				
				// LastLap+last+sector cam to last not finished
				for (long i = 0; i < info.mNumVehicles; ++i)
				{
					VehicleScoringInfoV01 &vinfo = info.mVehicle[i];
					if (vinfo.mFinishStatus == 0)
					{
						if ((((info.mMaxLaps - 1) == (vinfo.mTotalLaps)) || (((info.mEndET - sessiontime) < 20) && (info.mEndET > 0))) && (vinfo.mSector == 0) && (vinfo.mPlace == (finished + 1)))
						{
							needpos = vinfo.mPlace;
							pontosminbehind = obtime + 1; // disable onboard
							camvalthat = 2;
						}
						allfinished = false;
					}
				}				
			} // race end			
			if (allfinished) {
				camvalthat = waitsec + (rand() % 10);				
				if ((aktpos < 3) && (walkthrough == 1)) { // all racers finished cicling first 3
					needpos = aktpos + 1;
					pontosminbehind = obtime + 1; // disable onboard
				}
				else {					
					needpos = 1;
					pontosminbehind = obtime + 1; // disable onboard
				}
			}
			if ((needpos == 0)) { // not find anyone (eg.: everyone in the pit)
				camvalthat = 2;
				needpos = 1;
			}
			// INCIDENTS DETECTION FROM mResultsStream
			sseged = "";
			const char *ptr = info.mResultsStream;
			while (*ptr != NULL)
			{				
				sseged += *ptr++;
			}
			std::size_t ifound = sseged.find("Incident");
			std::size_t imfound = sseged.find("Immovable");
			std::size_t vfound = sseged.find("vehicle");
			if ((ifound != std::string::npos) && ((imfound != std::string::npos) || (vfound != std::string::npos)))
			{
				*seged = NULL;
				*sincsize = NULL;
				std::size_t sbfound = sseged.find("(");
				std::size_t sefound = sseged.find(")");				
				// sseged.substr((sbfound + 1), sefound - (sbfound + 1)).copy(seged, 256);
				std::size_t sibfound = sseged.find("contact (");
				std::size_t siefound = sseged.find(") with");				
				sseged.substr(sibfound + 9, siefound - sibfound - 9).copy(sincsize, 10);
				if ((strtod(sincsize, &e) > pincsize) && (strtod(sincsize, &e) > lowinc))
				{
					// preplayveh = strtol(seged, &e, 0);
					preplayveh = strtol(sseged.substr((sbfound + 1), sefound - (sbfound + 1)).c_str(), &e, 0);
					pincsize = strtod(sincsize, &e);
					pinctime = sessiontime;
					// strcpy(message.mText, sseged.substr((sbfound + 1), sefound - (sbfound + 1)).c_str());
				}
			}
			if ((!needreplay) && (!onreplay) && (sessiontime <(pinctime + 180)))
			{
				if ((pincsize >= highinc) ||
					((pincsize >= lowinc) && (info.mSession < 10)))
				{
					incsize = pincsize;
					replayveh = preplayveh;
					inctime = pinctime;
					needreplay = true;
					pincsize = 0;
				}
				if ((pincsize >= lowinc) && (info.mSession < 10) && ((pontosminbehind <= obtime) && (pontosminbehind >= 0.04)))
				{
					incsize = pincsize;
					replayveh = preplayveh;
					inctime = pinctime;
					needreplay = true;
					pincsize = 0;
				}
			}
			// INCIDENTS END
			if ((sessiontime - camvalttime) > camvalthat)
			{
				for (long i = 0; i < info.mNumVehicles; ++i) // position to slotid
				{
					VehicleScoringInfoV01 &vinfo = info.mVehicle[i];
					if (vinfo.mPlace == needpos) // onboard
					{
						if (((vinfo.mPitState == 0) && (vinfo.mFinishStatus == 0)) || (allfinished) || (inpit))
						{
							needveh = vinfo.mID;							
						}
						else { needveh = first; }
					}
					if (needpos > 1) // rearview
					{ 
						if (vinfo.mPlace == needpos - 1)
						{
							if (((vinfo.mPitState == 0) && (vinfo.mFinishStatus == 0)) || (allfinished) || (inpit))
							{
								rvveh = vinfo.mID;								
							}
						}
					}
					else { rvveh = first; }
				}				
				if ((lastcam == 0) && (needveh == aktveh)) { obchance = 3; }
				else { obchance = 2; }
				if ((pontosminbehind >= 0.05) && (pontosminbehind <= obtime) && ((rand() % 9 + 1) <= obchance)) //onboard
				{
					if ((rand() % 99 + 1) <= rearview)
					{
						needcam = rvcam;
						needveh = rvveh;
					}
					else
					{
						needcam = obcam;
					}
				}
				else {
					needcam = 4;
				}				
			}
			else { needveh = aktveh; }
		}
		for (long i = 0; i < info.mNumVehicles; ++i) // position to slotid
		{
			VehicleScoringInfoV01 &vinfo = info.mVehicle[i];
			if (vinfo.mID == replayveh)
			{
				strcpy(replayname, vinfo.mDriverName);
			}
			if (vinfo.mID == needveh)
			{
				strcpy(aktname, vinfo.mDriverName);
				curlapT = info.mCurrentET - vinfo.mLapStartET;
			}			
		}
		// FILE OUTPUTS
		// session time to file
		FILE *ftime = fopen(timefname.c_str(), "w");
		if (ftime != NULL)
		{
			if (onreplay)
			{
				fprintf(ftime, "REPLAY");
			}
			else
			{
				if (info.mSession > 9) {
					if ((info.mGamePhase > 4) && (info.mGamePhase < 8)) {
						currentlap = completedlaps + 1;
						if (completedlaps == 0) { completedlaps = 1; }
						if (((info.mEndET - info.mCurrentET) > 0) && ((((info.mCurrentET / completedlaps) * (info.mMaxLaps - 1)) > info.mEndET) || (info.mMaxLaps == 0)))
						{
							fprintf(ftime, "%02.0f:%02.0f:%02.0f", floor((info.mEndET - info.mCurrentET) / 3600.0), floor(fmod((info.mEndET - info.mCurrentET), 3600.0) / 60.0), fmod((info.mEndET - info.mCurrentET), 60.0));
						}
						else
						{
							if ((currentlap == info.mMaxLaps) || ((info.mMaxLaps > 2147483640) && ((info.mEndET - info.mCurrentET) < 0))) {
								fprintf(ftime, "Last lap");
							}
							else { fprintf(ftime, "%d / %d", currentlap, info.mMaxLaps); }
						}
					}
					else { if (info.mGamePhase == 8) { fprintf(ftime, "Futam vÈge"); } else { fprintf(ftime, ""); } }
				}
				else {
					if ((info.mEndET - info.mCurrentET) > 0) {
						fprintf(ftime, "%02.0f:%02.0f:%02.0f", floor((info.mEndET - info.mCurrentET) / 3600.0), floor(fmod((info.mEndET - info.mCurrentET), 3600.0) / 60.0), fmod((info.mEndET - info.mCurrentET), 60.0));
					}
					else { fprintf(ftime, "Session end"); }
				}
			}
			fclose(ftime);
		}
		WritetoInfohtml(info.mSession);
		/* LOG */
/*		FILE *fo = fopen("SCOUT.txt", "a");
		if (fo != NULL)
		{
			fprintf(fo, "here=%d time=%.3f session=%d gameph=%d showinpit=%s ", here, sessiontime, info.mSession, info.mGamePhase, showinpit);
			fprintf(fo, "intsec=%.3f intpos=%d ", interestsec, interest);
			fprintf(fo, "onboard=%.3f wait=%d ", obtime, waitsec);
			fprintf(fo, "pmbehind=%.3f mbehind=%.3f ", pontosminbehind, minbehind);
			fprintf(fo, "needveh=%d aktveh=%d needpos=%d aktpos=%d ", needveh, aktveh, needpos, aktpos);
			fprintf(fo, "replayveh=%d\n", replayveh);					
			// Close file
			fclose(fo);
		} */
		scoringrun = false;
	}
}

bool rF2autocam::RequestCommentary( CommentaryRequestInfoV01 &info )
{
  // COMMENT OUT TO ENABLE EXAMPLE
  return( false );

  // only if enabled, of course
  if( !mEnabled )
    return( false );

  // Note: function is called twice per second

  // Say green flag event for no particular reason every 20 seconds ...
  const double timeMod20 = fmod( mET, 20.0 );
  if( timeMod20 > 19.0 )
  {
    strcpy( info.mName, "GreenFlag" );
    info.mInput1 = 0.0;
    info.mInput2 = 0.0;
    info.mInput3 = 0.0;
    info.mSkipChecks = true;
    return( true );
  }

  return( false );
}

unsigned char rF2autocam::WantsToViewVehicle(CameraControlInfoV01 &camControl)
{
	if ((!scoringrun) && (automatic != 0)) {		
		if (camControl.mReplayActive)
		{
			if (sessiontime > replaystarted + 20)
			{
				stopreplay = true;
				replayset = false;
				needreplay = false;				
			}
			if (!replayset && !stopreplay && onreplay)
			{
				camControl.mID = replayveh;
				camControl.mCameraType = 4;
				camControl.mReplaySetTime = true;
				camControl.mReplaySeconds = inctime-5;
				aktveh = replayveh;				
				replayset = true;
				WritetoFileDrivername();
				return{ 3 };
			}
			if ((sessiontime > replaystarted + 4) && (sessiontime < replaystarted + 13))
			{
				camControl.mReplayCommand = 8;
				return{ 2 };
			}
			if (sessiontime > replaystarted + 13)
			{
				camControl.mReplayCommand = 9;
				return{ 2 };
			}						
		}
		if (!camControl.mReplayActive)
		{
			if (needveh != aktveh)
			{
				camControl.mID = needveh;
				if (_stricmp(camtest, "ob") == 0){ needcam = obcam; }
				else if (_stricmp(camtest, "rv") == 0) { needcam = rvcam; }
				camControl.mCameraType = needcam;
				camvalttime = sessiontime;
				aktveh = needveh;
				aktpos = needpos;
				lastcam = needcam;
				WritetoFileDrivername();
				return{ 1 };
			}
			if (needcam != lastcam)
			{
				camControl.mID = needveh;
				if (_stricmp(camtest, "ob") == 0){ needcam = obcam; }
				else if (_stricmp(camtest, "rv") == 0) { needcam = rvcam; }
				camControl.mCameraType = needcam;
				camvalttime = sessiontime;
				aktveh = needveh;
				aktpos = needpos;
				lastcam = needcam;
				WritetoFileDrivername();
				return{ 1 };
			}
		}
	}
	return{ 0 };

}

bool rF2autocam::WantsToDisplayMessage(MessageInfoV01 &msgInfo)
{
	if (message.mText == "")
	{
		return (false);
	}
	else {
		msgInfo = message;
		strcpy(message.mText, "");
		return (true);
	}
}
