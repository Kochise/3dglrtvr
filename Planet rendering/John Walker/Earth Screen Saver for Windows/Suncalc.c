/*

                     Earth Screen Saver for Windows

                    Astronomical Calculation Routines

*/

#include "sunclock.h"

double faketime = 0.0;                // Time increment for test mode

double siteLat = 47;		 		  // Observing site latitude
double siteLon = -7;		 		  // Observing site longitude
double siteAlt = 35775;				  // Altitude above observing site

static char dateSep[2], timeSep[2], amPM[2][5];
static int dateFormat, timeFormat;
static char format0[40], format1[40], format3[40];

/*  Forward functions.  */

static void updimage(HWND hWnd, RECT cRect, HDC hDC, int istimer, int copyCat);

/*  LOCALTIMEFORMAT  --  Retrieve localised date and time format from profile.  */

void localtimeformat(void)
{
	static char context[] = "intl";

	dateFormat = GetProfileInt(context, "iDate", 0);
	timeFormat = GetProfileInt(context, "iTime", 0);
	GetProfileString(context, "sDate", "/", dateSep, 2);
	GetProfileString(context, "sTime", ":", timeSep, 2);
	GetProfileString(context, "s1159", "AM", amPM[0], 5);
	GetProfileString(context, "s2359", "PM", amPM[1], 5);
	
	strcpy(format0, Format(0));
	strcpy(format1, Format(1));
	strcpy(format3, Format(3));
}

/*  EDTIME  --  Edit time in localised format.  */

static void edtime(char *str, int hh, int mm, int ss)
{
	if (timeFormat == 1) {
		wsprintf(str, format0, hh, (LPSTR) timeSep, mm, (LPSTR) timeSep, ss);
	} else {
		wsprintf(str, format1, (hh % 12) ? (hh % 12) : 12,
			(LPSTR) timeSep, mm, (LPSTR) timeSep, ss, (LPSTR) amPM[hh / 12]);
	}
}

/*  EDDATE  --  Edit date in localised format.  */

static void eddate(char *str, long yy, int mm, int dd)
{
	wsprintf(str, format3,
		dateFormat == 1 ? (long) dd : (dateFormat == 2 ? yy : (long) mm),
		(LPSTR) dateSep,
		dateFormat == 1 ? mm : (dateFormat == 2 ? mm : dd),
		(LPSTR) dateSep,
		dateFormat == 1 ? yy : (dateFormat == 2 ? (long) dd : yy));
}

/*  RSTRING  --  Retrieve a string from the resource file.  */

char *rstring(int resid)
{
#define maxCStrings 10              /* Maximum concurrently used strings */
    static char rstrings[maxCStrings][80];
    static int n = 0;
    int m = n;

    if (LoadString(hInst, resid, rstrings[m], 79) < 0) {
#ifndef NDEBUG
		{	char s[80];
		
			wsprintf(s, "String resource ID %d not found.\r\n", resid);
			OutputDebugString(s);
		}
#endif    
        strcpy(rstrings[m], "");
    }
    n = (n + 1) % maxCStrings;
    return rstrings[m];
}

/*  SSAVETIME  --  Edit and time for screen saver display.  */

void ssavetime(HDC hDC, struct tm lt, struct tm ct, RECT tr, COLORREF tcol)
{
	char tbuf[80];
	int obkm;
	COLORREF obkc, otxc;
	UINT otxa;

	edtime(tbuf, lt.tm_hour, lt.tm_min, lt.tm_sec);
	otxa = SetTextAlign(hDC, TA_NOUPDATECP);
    obkm = SetBkMode(hDC, OPAQUE);
    obkc = SetBkColor(hDC, RGB(0, 0, 0));
    otxc = SetTextColor(hDC, tcol);
	strcat(tbuf, " ");
	strcat(tbuf, rstring(idsWeekdays + lt.tm_wday));
	strcat(tbuf, " ");
	eddate(tbuf + strlen(tbuf), (long) (lt.tm_year + 1900), lt.tm_mon + 1, lt.tm_mday);
	DrawText(hDC, tbuf, -1, &tr, DT_CENTER | DT_TOP | DT_NOCLIP | DT_SINGLELINE);
	if (viewfrom == OBSLOCVIEW) {
		char slon[20], slat[20];

		wsprintf(slat, Format(4), abs(latDeg));
		if (latMin != 0) {
			wsprintf(slat + strlen(slat), Format(5), latMin);
		}
		if (latDeg != 0 || latMin != 0) {
			strcat(slat, rstring(latSgn ? IDS_BILLSHIT_SOUTH :
							IDS_BILLSHIT_NORTH));
		}
		wsprintf(slon, Format(4), abs(lonDeg));
		if (lonMin != 0) {
			wsprintf(slon + strlen(slon), Format(5), lonMin);
		}
		if (lonDeg != 0 || lonMin != 0) {
			strcat(slon, rstring(lonSgn ? IDS_BILLSHIT_EAST :
							IDS_BILLSHIT_WEST));
		}
		wsprintf(tbuf, rstring(viewfrom), (DWORD) siteAlt, slat, slon);
		DrawText(hDC, tbuf, -1, &tr,
				 DT_CENTER | DT_BOTTOM | DT_NOCLIP | DT_SINGLELINE);
	} else {
		DrawText(hDC, rstring(viewfrom), -1, &tr,
				 DT_CENTER | DT_BOTTOM | DT_NOCLIP | DT_SINGLELINE);
	}
	SetTextAlign(hDC, otxa);
	SetBkMode(hDC, obkm);
	SetBkColor(hDC, obkc);
	SetTextColor(hDC, otxc);
}


