/*

        Earth Screen Saver for Windows

				Definitions

*/

//#define TRACY				// Define to trace events with Blooie()

#include <windows.h>
#include <winbase.h>
#include <scrnsave.h>
#include <string.h>
#include <time.h>
#ifndef RC_INVOKED
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <sys/types.h>
#include <sys/timeb.h>
#endif

typedef RGBQUAD *LPRGBQUAD;
#define STRETCH_DELETESCANS COLORONCOLOR
#define MoveTo(h, x, y)	MoveToEx((h), (x), (y), NULL)

#include "resource.h"

//      String codes in resource file

#define IDS_APPNAME				103				// Application name/title

#define Format(n)   rstring(idsFormats + (n))

//      Resource ID numbers

//	Definitions for screen saver unique objects

#define ID_STARTUP					102
#define ID_HELP                     110

//	Chosen view

#define DAYVIEW				4000
#define NIGHTVIEW			4001
#define OBSLOCVIEW			4002
#define MOONVIEW			4003

#define PI 3.14159265358979323846  /* Assume not near black hole nor in
                                      Tennessee */

#define V       (void)

#define EOS     '\0'

//	Frequently used astronomical constants

#define J2000				2451545.0		// Julian day of J2000 epoch
#define JulianCentury		36525.0			// Days in Julian century
#define AstronomicalUnit	149597870.0		// Astronomical unit in kilometres
#define SunSMAX	 (AstronomicalUnit * 1.000001018) // Semi-major axis of Earth's orbit
#define EarthRad			6378.14			// Earth's equatorial radius, km (IAU 1976)
#define LunatBase			2423436.0		/* Base date for E. W. Brown's numbered
											   series of lunations (1923 January 16) */
#define SynMonth			29.53058868		// Synodic month (mean time from new Moon to new Moon)

/*  Handy mathematical functions  */

#ifdef abs
#undef abs
#endif

/* As an indication of how carefully Microsoft tests software prior
   to shipment, and their deep committment to numerical applications,
   Visual C++ 1.5 (C 8.0) shipped with a library in which tan(x)
   returns the wrong sign for arguments between -90 and 90 degrees
   but only if the math coprocessor is not installed.  Can you believe
   this?  So, define tan(x) as sin(x) / cos(x) to work around this
   unbelievable example of lack of regression testing. */
   
//#define MICROSOFT_CAN_T_COUNT_WORTH_BEANS
#ifdef MICROSOFT_CAN_T_COUNT_WORTH_BEANS
#define tan(x)	(sin(x) / cos(x))
#endif // MICROSOFT_CAN_T_COUNT_WORTH_BEANS 

#define sgn(x) (((x) < 0) ? -1 : ((x) > 0 ? 1 : 0))       /* Extract sign */
#define abs(x) ((x) < 0 ? (-(x)) : (x))                   /* Absolute val */
#define fixangle(a) ((a) - 360.0 * (floor((a) / 360.0)))  /* Fix angle    */
#define fixangr(a)  ((a) - (PI*2) * (floor((a) / (PI*2))))/* Fix angle in radians*/
#define dtr(x) ((x) * (PI / 180.0))                       /* Degree->Radian */
#define rtd(x) ((x) / (PI / 180.0))                       /* Radian->Degree */

#define RepositionTime	10		// Reposition image every RepositionTime minutes

//  Determine number of elements in an array

#define ELEMENTS(array) (sizeof(array)/sizeof((array)[0]))

//  Assertion checking for Windows

#ifndef NDEBUG
#define assert(x) { if (!(x)) { char erm[128]; wsprintf((LPSTR) erm, \
        (LPSTR) "Assertion %s failed at line %d of file %s.", (LPCSTR) #x, __LINE__,(LPSTR) __FILE__); \
        FatalAppExit(0, (LPCSTR) erm); }}
#else
#define assert(x)
#endif

//	Make sure no debug output remains in production builds: use Honk(x) for debug MessageBeep(x)

#ifdef NDEBUG
#ifdef OutputDebugString
#undef OutputDebugString
#endif
#define OutputDebugString(x) DebugOutputInProductionBuild()
#define Honk(x) DebugOutputInProductionBuild()
#else
#define Honk(x)	MessageBeep(x)
#endif

extern char szString[128];		// Variable to load resource strings
#define hInst	 hMainInstance  // Redefinitions for screen saver lib compatibility
#define hWndMain hMainWindow
extern HINSTANCE hInst;			// Class instance pointer
extern HWND hWndMain;			// Main window pointer
extern HICON micon;				// Moon icon calculated for correct phase
extern HICON planetIcons[10];	// Planet icons
extern double faketime;         // Time for simulation / testing
extern double siteLat;          // Observing site latitude
extern double siteLon;          // Observing site longitude
extern double siteAlt;			// View site from altitude
extern int ImageWid, ImageHgt;	// Image width and height
extern char *tzName[2];			// Time zone names
extern int viewfrom;			// Selected view
extern int imageSize;			// Image size

#define WindowProc(x) LONG FAR PASCAL x(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)

/* Dialogue service functions. */

#define DialogueProc(name) BOOL FAR PASCAL name(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)

//  Calculation functions from ASTRO.C

extern double jtime(struct tm *t);
extern void jyear(double td, long *yy, int *mm, int *dd),
            jhms(double j, int *h, int *m, int *s);
extern void sunpos(double jd, int apparent,
				   double *ra, double *dec, double *rv, double *slong);
extern double gmst(double jd);
extern double ucttoj(long year, int mon, int mday,
                     int hour, int min, int sec);
extern void highmoon(double jd, double *l, double *b, double *r);
extern double obliqeq(double jd);
extern void ecliptoeq(double jd, double Lambda, double Beta, double *Ra, double *Dec);

//	From EARTH.C

#ifdef TRACY
extern void Blooie(char *s);
#endif
extern void set_tm_time(struct tm *t, BOOL islocal);
extern BOOL bailout(void);
extern void ticker(void);

//	From SUNCALC.C

extern void ssavetime(HDC hDC, struct tm lt, struct tm ct, RECT tr, COLORREF tcol);
extern void localtimeformat(void);
extern char *rstring(int resid);

//	From SCRNSAVE.C

extern void GetIniEntries(void);
extern void GetIniSettings(void);
extern int latSgn, latDeg, latMin, latSec,
		   lonSgn, lonDeg, lonMin, lonSec;
extern BOOL nightLights;

//	From VIEWFROM.C

extern void viewInit(void);
extern void needImageBitmap(void);
extern void release_satview_bitmaps(void);
extern void release_image(void);
extern void updviewfrom(HWND hWnd, int isize, int offX, int offY,
						double satlat, double satlon, double satalt,
						BOOL repositioned);
