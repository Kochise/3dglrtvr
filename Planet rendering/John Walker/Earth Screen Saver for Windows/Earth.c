/*

		Earth Screen Saver Window procedure
                             
*/

#include "sunclock.h"

int viewfrom = DAYVIEW;					// Chosen view
int imageSize = 640;					// Requested image size

static BOOL firstTick = FALSE;			// No repaint before first timer tick
static BOOL imageUp = FALSE;			// Is image painted on screen ?
static BOOL bailedOut = FALSE;			// Set when termination signal received

static RECT tickerRect;					// Rectangle for ticker time display
static long tickerLast;					// Last time ticker activated
static HDC hTickerDC;					// DC for ticker
static COLORREF skycolour = RGB(0, 0, 255); // Colour for legends

static TIME_ZONE_INFORMATION tzInfo;	// Time zone information
char *tzName[2] = { "", "" };			// Time zone names

#ifdef TRACY
void Blooie(char *s)
{
	RECT rc;
	HDC hDC;

	hDC = GetDC(hWndMain);
	GetClientRect(hWndMain, &rc);
	SetBkMode(hDC, OPAQUE);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkColor(hDC, RGB(255, 0, 0));
	SetTextAlign(hDC, TA_NOUPDATECP);
	TextOut(hDC, 10, 10, s, strlen(s));
	ReleaseDC(hWndMain, hDC);
}
#endif

//  SET_TM_TIME  --  Set time from Windows system or local time

void set_tm_time(struct tm *t, BOOL islocal)
{
#define CtF(tf, sf) t->tf = s.sf

	SYSTEMTIME s;

	if (islocal) {
		GetLocalTime(&s);
	} else {
		GetSystemTime(&s);
	}
	CtF(tm_sec, wSecond);
	CtF(tm_min, wMinute);
	CtF(tm_hour, wHour);
	CtF(tm_mday, wDay);
	CtF(tm_mon, wMonth - 1);
	CtF(tm_year, wYear - 1900);
	CtF(tm_wday, wDayOfWeek);
	//  tm_yday  never used
	t->tm_isdst = GetTimeZoneInformation(&tzInfo) == TIME_ZONE_ID_DAYLIGHT;
}

// TICKER  --  Periodically update seconds counter while calculating sky

void ticker(void)
{
	SYSTEMTIME s;
	static WORD tickerLast;

	GetSystemTime(&s);
	if (imageUp && s.wSecond != tickerLast && !fChildPreview) {
		struct tm ct, lt;
		
		tickerLast = s.wSecond;
		set_tm_time(&lt, TRUE);
		set_tm_time(&ct, FALSE);
		ssavetime(hTickerDC, lt, ct, tickerRect, skycolour);	// Update local and universal time
    }
}

/*	BAILOUT  --  See if we should bail out of calculation early.
				 Note that we don't actually process a pending
				 keyboard or mouse event, simply notify the caller
				 that one is queued.  This allows the screen saver
				 main program to actually receive the event and
				 perform its shut down. */

BOOL bailout(void)
{
	MSG msg;

#define PEEKERWOOD(x) PeekMessage(&msg, hWndMain, x, x, PM_NOREMOVE | PM_NOYIELD)

	/* Why not use WM_KEYFIRST / WM_KEYLAST and WM_MOUSEFIRST / WM_MOUSELAST
	   instead of all this code-barf?  Because Redmond Kiddies Kode Club
	   saw fit to make the set of events which terminate a screen saver
	   different from all those within the defined ranges.  So, you can get
	   into a state where you think you're about to terminate, but in fact
	   go on running.  To get around this inanity, we have to explicitly
	   test for each of the events documented in scrnsave.h as causing the
	   screen saver to terminate, and then hope that is, in fact, the actual
	   set of events which the library tests for.
	   
	   Well, on further investigation, the comment in scrnsave.h does, in fact,
	   turn out to be a lie.  For example, pressing the "Sys Req" key
	   generates a WM_KEYDOWN but does *not* terminate the screen saver.  So,
	   to try to survive this Monkeysoft muddle, let's try eliminating the
	   redundant bailout entirely. */ 
	
	if (bailedOut ||
		PEEKERWOOD(WM_LBUTTONDOWN) || PEEKERWOOD(WM_MBUTTONDOWN) ||
		PEEKERWOOD(WM_RBUTTONDOWN) || PEEKERWOOD(WM_KEYDOWN) ||
		PEEKERWOOD(WM_KEYUP) || PEEKERWOOD(WM_MOUSEMOVE)
	   ) {		
#ifdef Mdiag
		char s[120];
		
		if (!bailedOut) {
			wsprintf(s, "Bailed on %X, hWnd = %x (oWnd = %x) wParam = %X, lParam = %lX\r\n", msg.message, msg.hwnd, hMainWindow, msg.wParam, msg.lParam);
			OutputDebugString(s);
		} else {
			OutputDebugString("Redundant bailout.\r\n");	
		}
#endif
#ifdef TRACY
		char s[120];
		
		if (!bailedOut) {
			wsprintf(s, "Bailed on %X, hWnd = %x (oWnd = %x) wParam = %X, lParam = %lX",
				msg.message, msg.hwnd, hMainWindow, msg.wParam, msg.lParam);
			Blooie(s);
/*{ long l = GetTickCount();

  while ((GetTickCount() - l) < 10000) Yield();
}*/
		} else {
			Blooie("Rbail: ");	
		}
#endif		
		return bailedOut = TRUE;
	}
	return FALSE; 
}

/*  SCREENSAVERPROC  --  Screen saver window procedure.  */

WindowProc(ScreenSaverProc)
{
    HDC hDC;
    PAINTSTRUCT ps;         			// holds PAINT information
    RECT cr;							// Client rectangle
    int minsize;						// Short direction in window
    static int offX, offY;				// Offset of image within window
    static long cctime;					// Time to display (real or simulated) */
    static long rptime = 0;				// Last time image shifted
    static DWORD calctime;				// Calculation time in ticks
    static WORD minlapse = 0;			// Minutes elapsed since last calculation
    struct tm ct, lt;
    double jt;
    static BOOL repositioned = FALSE;
    static BOOL busy = FALSE;
	static char stdname[128], dstname[128];	// Standard and Daylight time zone names
	double lastslat, lastslon, lastsalt;
	double ijdate, moonclong, moonclat, mooncrv, moonlong, moonra, moondec, igmst,
		   sunra, sundec, sunrv, sunlong, subslong;
	static int isize = -1;					// Image size
	static TEXTMETRIC tmx;

    switch (Message) {

        case WM_CREATE:
			firstTick = FALSE;			// Waiting for first timer tick
			imageUp = FALSE;			// Image not yet painted
			bailedOut = FALSE;			// No termination signal received
			rptime = 0;
			minlapse = 0;
			repositioned = FALSE;
			busy = FALSE;
			isize = -1;
			viewInit();					// Initialise image generator
            hDC = GetDC(hWnd);
			SetCursor(NULL);			// Zap cursor before first draw
            GetIniEntries();
            GetIniSettings();			// Get setup defaults
            siteLat = (latSgn ? -1 : 1) * (latDeg + (latMin / 60.0) + (latSec / (60.0 * 60.0)));
            siteLon = (lonSgn ? -1 : 1) * (lonDeg + (lonMin / 60.0) + (lonSec / (60.0 * 60.0)));
			GetTimeZoneInformation(&tzInfo);
			wcstombs(stdname, tzInfo.StandardName, sizeof stdname);
			tzName[FALSE] = stdname;
			wcstombs(dstname, tzInfo.DaylightName, sizeof dstname);
			tzName[TRUE] = dstname;
            localtimeformat();
			GetTextMetrics(hDC, &tmx);
            srand((unsigned) (GetTickCount() & 0xFFFF));
            ReleaseDC(hWnd, hDC);
			needImageBitmap();
            SetTimer(hWnd, 1, 1000u, NULL);
            break;

        case WM_COMPACTING:
#ifdef TRACY
			Blooie("Yaaar, bro!  We're gonna compact!");
#endif
			release_satview_bitmaps();
        	return 0;

        case WM_DESTROY:
            KillTimer(hWnd, 1);
			release_satview_bitmaps();
			release_image();
        	break;

#ifdef CLIPPO

		/*	If CLIPPO is defined, right mouse button copies screen
			to the clipboard.  Why?  So we can make screen shots to
			illustrate the program.  */

		case WM_RBUTTONDOWN:
            {
                HDC hdcMem;
                HBITMAP hBitmap;

                hDC = GetDC(hWnd);
                hdcMem = CreateCompatibleDC(hDC);
				GetClientRect(hWnd, &cr);
                hBitmap = CreateCompatibleBitmap(hDC,
                            cr.right, cr.bottom);
                if (hBitmap) {
                    SelectObject(hdcMem, hBitmap);
                    BitBlt(hdcMem, 0, 0, cr.right, cr.bottom,
                            hDC, 0, 0, SRCCOPY);
                    OpenClipboard(hWnd);
                    EmptyClipboard();
                    SetClipboardData(CF_BITMAP, hBitmap);
                    CloseClipboard();
                    DeleteDC(hdcMem);
                    ReleaseDC(hWnd, hDC);
                }
            }
			return 0;
#endif
        
        case WM_TIMER:
			if (!busy) {
				busy = TRUE;

				/* Monkeysoft madness makes it impossible to determine
				   precisely which events will actually cause the screen
				   saver to terminate, resulting in our having no way to
				   know when a bailout() is actually valid.  So, we reset
				   bailedOut if we receive a timer tick after a bogus event
				   which caused us to bail out. */

				bailedOut = FALSE;
		        hDC = GetDC(hWnd);
				if (imageUp && !fChildPreview && (isize > 0)) {
		        	cr.left = offX;
		        	cr.top = offY - (tmx.tmHeight + 1);
			        cr.right = cr.left + isize;
			        cr.bottom = cr.top + isize + (tmx.tmHeight + 1) * 2;
					set_tm_time(&lt, TRUE);
					set_tm_time(&ct, FALSE);
		       		ssavetime(hDC, lt, ct, cr, skycolour);	// Update local and universal time
				}
		        ReleaseDC(hWnd, hDC);
				firstTick = TRUE;
				InvalidateRect(hWnd, NULL, FALSE);
#ifdef TRACY_FORCE_COMPACT
				{	static int toop = 0;

					if (++toop == 137) {
						PostMessage(hWnd, WM_COMPACTING, 0, 0);
						toop = 0; 
					}
				}
#endif
			}
        	break;

		case WM_ERASEBKGND:
			hDC = (HDC) wParam;
		    GetClientRect(hWnd, &cr);
		    FillRect(hDC, &cr, (HBRUSH) GetStockObject(BLACK_BRUSH));
			repositioned = TRUE;
			return 0;

        case WM_PAINT:
			time(&cctime); 
            memset(&ps, 0, sizeof(PAINTSTRUCT));
            hDC = BeginPaint(hWnd, &ps);
            if (!firstTick || (ImageWid < 0)) {
				if (ImageWid < 0) {
					char s[132];
					int tx, ty;
					
					if (cctime % 10 == 0) {
						RECT rc;
						SIZE sz;

		    			GetClientRect(hWnd, &rc);
	  					LoadString(hMainInstance, idsInsuffMemory, s, sizeof s);
	  					GetTextExtentPoint32(hDC, s, lstrlen(s), &sz);
	  					SetBkMode(hDC, TRANSPARENT);
	  					tx = (int) (((rc.right - sz.cx) * ((long) rand())) / RAND_MAX); 
	  					ty = sz.cy + (int) (((rc.bottom - (2 * sz.cy)) * ((long) rand())) / RAND_MAX);
	  					rc.top = sz.cy;
		    			FillRect(hDC, &rc, (HBRUSH) GetStockObject(BLACK_BRUSH));
	  					skycolour = RGB(255, 255, 255);  
	  					SetTextColor(hDC, skycolour);
	  					SetTextAlign(hDC, TA_NOUPDATECP);
	  					TextOut(hDC, tx, ty, s, lstrlen(s));
					}
				}
	            ValidateRgn(hWnd, NULL);
	            EndPaint(hWnd, &ps);
				busy = FALSE;
				imageUp = TRUE;
#ifdef TRACY
				if (ImageWid < 0) Blooie("ImageWid < 0");
#endif
	            return 0;
            }
			imageUp = FALSE;			// Disable timer updates until image initially drawn
			hTickerDC = hDC;        
			set_tm_time(&lt, TRUE);
			set_tm_time(&ct, FALSE);
			jt = faketime = jtime(&ct);
            GetClientRect(hWnd, &cr);
            minsize = min(cr.right, cr.bottom);
			isize = (imageSize < 64) ? ((2 * minsize) / 3) : imageSize;
            minsize = min(cr.right, (cr.bottom - (2 * (tmx.tmHeight + 1))));
			if (isize > minsize) {
				isize = minsize;
			}
            
            //	Save time and position to update ticker during calculation
            
            tickerLast = cctime;
        	tickerRect.left = offX;
        	tickerRect.top = offY - (tmx.tmHeight + 1);
	        tickerRect.right = tickerRect.left + isize;
	        tickerRect.bottom = tickerRect.top + isize + (tmx.tmHeight + 1) * 2;					      
            
            /* If it's time, randomly reposition the image within the
               rectangular window.  This prevents burning in the phosphor by
               the constant parts of the map. */ 
            
            if ((cctime - rptime) > (RepositionTime * 60L)) {
            	offX = (int) (((cr.right - isize) * ((long) rand())) / RAND_MAX);
            	offY = (int) ((((cr.bottom - (tmx.tmHeight + 1)) - isize) * ((long) rand())) / RAND_MAX);
				offY = max(offY, tmx.tmHeight + 1);
            	
            	/* Sigh.  If this is the first time, we must set the ticker
            	   location to the window where we're going to paint the initial
            	   sky map. */
            	   
            	if (rptime == 0) {
		        	tickerRect.left = offX;
		        	tickerRect.top = offY - (tmx.tmHeight + 1);
			        tickerRect.right = tickerRect.left + isize;
			        tickerRect.bottom = tickerRect.top + isize + (tmx.tmHeight + 1) * 2;					      
		        }
            	rptime = cctime;
            	repositioned = TRUE;
            }
            cr.right = cr.bottom = isize;		// Now restrict to actual draw window

			ijdate = faketime;
			igmst = gmst(ijdate);

			if (viewfrom == DAYVIEW || viewfrom == NIGHTVIEW) {
				sunpos(ijdate, TRUE, &sunra, &sundec, &sunrv, &sunlong);
				subslong = (igmst * 15) - sunra;
				if (viewfrom == NIGHTVIEW) {
					subslong = fixangle(subslong + 180);
					sundec = -sundec;
				}
				if (subslong > 180) {
					subslong = -(360 - subslong);
				} else if (subslong < -180) {
					subslong += 360;
				}
				lastslat = dtr(sundec);
				lastslon = dtr(subslong);
				lastsalt = sunrv * SunSMAX;
			} else if (viewfrom == OBSLOCVIEW) {
				lastslat = dtr(siteLat);
				lastslon = dtr(siteLon);
				lastsalt = siteAlt;							
			} else {
				highmoon(ijdate, &moonclong, &moonclat, &mooncrv);
				ecliptoeq(ijdate, moonclong, moonclat, &moonra, &moondec);
				moonlong = (igmst * 15) - moonra;
				if (moonlong > 180) {
					moonlong = -(360 - moonlong);
				} else if (moonlong < -180) {
					moonlong += 360;
				}
				lastslat = dtr(moondec);
				lastslon = dtr(moonlong);
				lastsalt = mooncrv;
			}

			updviewfrom(hWnd, isize, offX, offY, lastslat, lastslon, lastsalt,
						repositioned);
			repositioned = FALSE;

            ValidateRgn(hWnd, NULL);

            /* Inform Windows painting is complete */
            EndPaint(hWnd, &ps);
			busy = FALSE;
			imageUp = TRUE;
            return 0;
    }
	return DefScreenSaverProc(hWnd, Message, wParam, lParam);
}

