#ifndef MUSIC_H
#define MUSIC_H

#include <windows.h>
#include "jamultypes.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mmsystem.h>

// these are CD audio modes for the CDPlayerUpdate
#define CD_OFF		  0
#define CD_LOOPTRACK  1	// continuously loop the current track
#define CD_INTROLOOP  2	// plays the chosen track, then loops the next one
#define CD_RANDOM	  3 // after current track, jump to any other at random
#define CD_NORMAL	  4 // just keep playing the tracks in order, loops at end of CD to beginning

byte MusicInit(void);
void MusicExit(void);

// CD audio functions
byte InitCDPlayer(void);
void ExitCDPlayer(void);
byte CDLoaded(void);
dword CDCurrentTrack(void);
dword CDTrackCount(void);
void CDPlay(int track);
void CDStop(void);
byte CDIsPlaying(void);
void CDPlayerUpdate(byte mode);
void CDNeedsUpdating(void);

// this kills the windows CD Player if it is on
void ShutoffWindowsCD(void);
BOOL CALLBACK FindCDPlayer( HWND hwnd, LPARAM lParam );

#endif