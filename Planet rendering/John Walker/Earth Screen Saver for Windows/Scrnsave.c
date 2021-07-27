/*

			Earth Screen Saver

*/

#include "sunclock.h"

/* Global used by SCRNSAVE.LIB. Required for all screen savers. */

char szAppName[40];

/* Globals exported. */

char szName[]="Earth Screen Saver";

/* Externals defined in SCRNSAVE.LIB. Required for all screen savers. */

HINSTANCE hMainInstance;
HWND hMainWindow;
char szName[TITLEBARNAMELEN];
char szScreenSaver[22];
char szHelpFile[MAXFILELEN];
char szNoHelpMemory[BUFFLEN];
UINT MyHelpMessage;
HOOKPROC fpMessageFilter;

//	Main configuration parameters and their .ini file names

static char szLatSgn[] = "LatitudeSgn";
int latSgn;
static char szLatDeg[] = "LatitudeDeg";
int latDeg;
static char szLatMin[] = "LatitudeMin";
int latMin;
static char szLatSec[] = "LatitudeSec";
int latSec;

static char szLonSgn[] = "LongitudeSgn";
int lonSgn;
static char szLonDeg[] = "LongitudeDeg";
int lonDeg;
static char szLonMin[] = "LongitudeMin";
int lonMin;
static char szLonSec[] = "LongitudeSec";
int lonSec;

static char szNightLights[] = "NightLights";
BOOL nightLights;

/* RegisterDialogClasses -- Entry point for registering window
							classes required by configuration dialog box. */

BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
    return TRUE;
}

// CREDITSDIALOGUE  --  Display Earth image credits

DialogueProc(creditsDialogue)
{
    switch (message) {
        case WM_COMMAND:
            switch (wParam) {
                case IDOK:
				case IDCANCEL:
                    EndDialog(hDlg, TRUE);
                    return TRUE;
		}
    }
    return FALSE;
}

/* ScreenSaverConfigureDialog -- Dialogue box function for
								 screen saver configuration dialogue. */

DialogueProc(ScreenSaverConfigureDialog)
{
	static HWND hIDOK;
	BOOL intok;
	int is;

    switch (message) {
        case WM_INITDIALOG:
            GetIniEntries();
            GetIniSettings();

			CheckDlgButton(hDlg, IDC_VF_SUN, viewfrom == DAYVIEW); 
			CheckDlgButton(hDlg, IDC_VF_MOON, viewfrom == MOONVIEW); 
			CheckDlgButton(hDlg, IDC_VF_NIGHT, viewfrom == NIGHTVIEW); 
			CheckDlgButton(hDlg, IDC_VF_LATLON, viewfrom == OBSLOCVIEW); 
			SetDlgItemInt(hDlg, IDC_CONF_ALTITUDE, (DWORD) siteAlt, FALSE);

			SetDlgItemInt(hDlg, IDC_CONF_LATDEG, abs(latDeg), FALSE);
			SetDlgItemInt(hDlg, IDC_CONF_LATMIN, latMin, FALSE);
			SetDlgItemInt(hDlg, IDC_CONF_LATSEC, latSec, FALSE);
			CheckDlgButton(hDlg, IDC_CONF_LAT_NORTH, !latSgn);
			CheckDlgButton(hDlg, IDC_CONF_LAT_SOUTH, latSgn);

			SetDlgItemInt(hDlg, IDC_CONF_LONGDEG, abs(lonDeg), FALSE);
			SetDlgItemInt(hDlg, IDC_CONF_LONGMIN, lonMin, FALSE);
			SetDlgItemInt(hDlg, IDC_CONF_LONGSEC, lonSec, FALSE);
			CheckDlgButton(hDlg, IDC_CONF_LON_WEST, !lonSgn);
			CheckDlgButton(hDlg, IDC_CONF_LON_EAST, lonSgn);

			CheckDlgButton(hDlg, IDC_CONF_NIGHT_LIGHTS, nightLights);

			if (imageSize == 0) {
				SetDlgItemText(hDlg, IDC_IMAGE_SIZE, rstring(IDS_AUTO));
			} else {
				SetDlgItemInt(hDlg, IDC_IMAGE_SIZE, imageSize, FALSE);
			}

            hIDOK = GetDlgItem(hDlg, IDOK);
            return TRUE;

        case WM_COMMAND:
            switch (wParam) {
                case IDOK:
					if (IsDlgButtonChecked(hDlg, IDC_VF_SUN)) {
						viewfrom = DAYVIEW;
					} else if (IsDlgButtonChecked(hDlg, IDC_VF_MOON)) {
						viewfrom = MOONVIEW;
					} else if (IsDlgButtonChecked(hDlg, IDC_VF_NIGHT)) {
						viewfrom = NIGHTVIEW;
					} else if (IsDlgButtonChecked(hDlg, IDC_VF_LATLON)) {
						viewfrom = OBSLOCVIEW;
					} 

					latDeg = GetDlgItemInt(hDlg, IDC_CONF_LATDEG, NULL, FALSE);
					latMin = GetDlgItemInt(hDlg, IDC_CONF_LATMIN, NULL, FALSE);
					latSec = GetDlgItemInt(hDlg, IDC_CONF_LATSEC, NULL, FALSE);
					latSgn = IsDlgButtonChecked(hDlg, IDC_CONF_LAT_SOUTH);
                    
					lonDeg = GetDlgItemInt(hDlg, IDC_CONF_LONGDEG, NULL, FALSE);
					lonMin = GetDlgItemInt(hDlg, IDC_CONF_LONGMIN, NULL, FALSE);
					lonSec = GetDlgItemInt(hDlg, IDC_CONF_LONGSEC, NULL, FALSE);
					lonSgn = IsDlgButtonChecked(hDlg, IDC_CONF_LON_EAST);

					nightLights = IsDlgButtonChecked(hDlg, IDC_CONF_NIGHT_LIGHTS);

					siteAlt = GetDlgItemInt(hDlg, IDC_CONF_ALTITUDE, NULL, FALSE);

					is = GetDlgItemInt(hDlg, IDC_IMAGE_SIZE, &intok, FALSE);
					if (intok) {
						imageSize = is;
					} else {
						imageSize = 0;
					}
					
					/* Write out all configuration parameters to the registry,
					   including those controlled by the detailed configuration
					   dialogue. */
                    
                    {
						HKEY keyh;
						DWORD dispos;
						TCHAR keyname[80];
						DWORD siteAltD = (DWORD) siteAlt;
						
						strcpy(keyname, "Software\\Fourmilab.ch\\");
						strcat(keyname, szAppName);
						if (RegCreateKeyEx(HKEY_CURRENT_USER,
									keyname,					// Subkey name
									0,							// Reserved
									"",							// Bogus class string
									REG_OPTION_NON_VOLATILE,	// Options
									KEY_ALL_ACCESS,				// Access rights			
									NULL,						// Security attributes
									&keyh,						// Returned handle to the key
									&dispos) == ERROR_SUCCESS) { // Returned disposition
#define regStore(name, value) RegSetValueEx(keyh, name, 0, REG_DWORD, (BYTE *) &value, sizeof(value))
								regStore("Viewpoint", viewfrom);
								regStore(szLatSgn, latSgn);
								regStore(szLatDeg, latDeg);  
								regStore(szLatMin, latMin);  
								regStore(szLatSec, latSec);
				                
								regStore(szLonSgn, lonSgn);  
								regStore(szLonDeg, lonDeg);  
								regStore(szLonMin, lonMin);  
								regStore(szLonSec, lonSec);

								regStore(szNightLights, nightLights);

								regStore("Altitude", siteAltD);

								regStore("ImageSize", imageSize);
								RegCloseKey(keyh);
						}
                   }
#undef regStore              
#undef Fp
                    EndDialog(hDlg, TRUE);
                    return TRUE;

                case IDCANCEL:
                    EndDialog(hDlg, FALSE);
                    return TRUE;

				case IDC_ABOUT_IMAGE:
					DialogBox(hInst, MAKEINTRESOURCE(DLG_CREDITS),
							  hDlg, creditsDialogue); 
					break;

                case ID_HELP:
DoHelp:
                    MessageBox(hDlg, "Earth screen saver.", szName, MB_OK);
                    break;
            }
            break;

        default:
            if (message == MyHelpMessage)     // Context sensitive help msg.
                goto DoHelp;
    }
    return FALSE;
}

/* GetIniSettings -- Get initial settings from the registry

	Params:  hWnd -- Handle to window

	Return:  None
	
	Note that initial installation defaults are set by the default
	arguments to the retrievals of the profile variables.
*/

void GetIniSettings(void)
{
	HKEY keyh;
	TCHAR keyname[80];
	DWORD siteAltd;
	TIME_ZONE_INFORMATION tzone;
	int longshot, lateral, latsg, lonsg;
	BOOL isdst = GetTimeZoneInformation(&tzone) == TIME_ZONE_ID_DAYLIGHT;

	/* Guess initial latitude and longitude based on the
	   selected time zone.  We displace 15 degrees for every 60
	   minutes of bias from UTC in the time zone.  If the switch to
	   daylight time occurs in the first 6 months of the year, we
	   guess 45 North latitude; if in the latter half of the year
	   we guess 45 South.  If daylight time is not observed, see
	   if the time zone name begins with "SA" (as South American
	   time zones do) or contains the word "South"; if so guess
	   -45, otherwise +45). */

	longshot = (tzone.Bias * 15) / 60;
	lateral = (tzone.DaylightDate.wMonth > 0 &&
			   tzone.DaylightDate.wMonth <= 6) ? 45: -45;
			   
	if (tzone.StandardDate.wMonth == 0) {
		if ((wcsncmp(tzone.StandardName, L"SA ", 2) == 0) ||
			(wcsstr(tzone.StandardName, L"South ") != NULL)) {
			lateral = -45;
		} else {
			lateral = 45;
		}
	}
	latsg = lateral < 0;
	lateral = abs(lateral);
	lonsg = longshot < 0;
	longshot = abs(longshot);
	
	strcpy(keyname, "Software\\Fourmilab.ch\\");
	strcat(keyname, szAppName);

	if (RegOpenKeyEx(HKEY_CURRENT_USER,
			keyname,				// Subkey name
			0,						// Reserved
			KEY_QUERY_VALUE,		// Security access mask
			&keyh) == ERROR_SUCCESS) {
		DWORD vsize;
		DWORD vtype =  REG_DWORD;

#define regGet(name, value) vsize = sizeof(value); RegQueryValueEx(keyh, name, NULL, &vtype, (BYTE *) &value, &vsize)
		regGet("Viewpoint", viewfrom);
		
		regGet(szLatSgn, latSgn);
		regGet(szLatDeg, latDeg);
		regGet(szLatMin, latMin);
		regGet(szLatSec, latSec);
		
		regGet(szLonSgn, lonSgn);
		regGet(szLonDeg, lonDeg);
		regGet(szLonMin, lonMin);
		regGet(szLonSec, lonSec);
		
		regGet(szNightLights, nightLights);
		regGet("Altitude", siteAltd);
		siteAlt = siteAltd;
		if (siteAlt <= 10) {
			siteAlt = 35775;			// Reset ridiculous altitudes to default
		}
		regGet("ImageSize", imageSize);

		RegCloseKey(keyh);
#undef regGet
	} else {
		viewfrom = DAYVIEW;

		latSgn = latsg; 
		latDeg = lateral; 
		latMin = 0; 
		latSec = 0;
		 
		lonSgn = lonsg; 
		lonDeg = longshot; 
		lonMin = 0; 
		lonSec = 0;

		nightLights = 1;

		siteAlt = 35775; 
		imageSize = 0; 
	}
}

void GetIniEntries(void)
{
	// Load common strings from stringtable...
    LoadString(hMainInstance, idsAppName, szAppName, 40); 
	LoadString(hMainInstance, idsScreenSaver, szScreenSaver, 22);
	LoadString(hMainInstance, idsHelpFile, szHelpFile, MAXFILELEN);
	LoadString(hMainInstance, idsNoHelpMemory, szNoHelpMemory, BUFFLEN);
}
