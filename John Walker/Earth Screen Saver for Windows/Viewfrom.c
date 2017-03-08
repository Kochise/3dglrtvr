/*

			Earth Screen Saver for Windows

		   View From Calculation and Display

*/

#include "sunclock.h"
#include "linalg.h"

#define TERMINC  100						// Circle segments for terminator
#define PROJINT  (60 * 10)					/* Frequency of seasonal recalculation
											   in seconds. */

static int lleft, lwid, lhgt;				// Logical drawing area upper corner and size
static long bslat, bslon, bsalt;			// View that saved bitmap represents

static DWORD lastDrawTime = 0;				// Ticks last update required to draw
static DWORD lastDrawEnd = 0;				// Time last update completed

static int onoon = -1;						// Previous pixel longitude of noon
static short *wtab = NULL, *wtab1 = NULL, *wtabs;
static int wtabsize = -1;					// Width table size
static long lctime = 0;						// Last full calculation time
int ImageWid = -1;				  			// Width of master image
int ImageHgt = -1;							// Height of master image
static HPALETTE imagePal = NULL;			// Custom colour palette

static struct {
	WORD	palVersion;
	WORD	palNumEntries;
	PALETTEENTRY palPalEntry[256];
} logpal;

static HGLOBAL svdib = NULL;				// Texture mapped DIB
static HGLOBAL earthBitmap = NULL;			// Copy of bitmap resource
static HGLOBAL pmBuf = NULL;

int satvterm = TRUE;						// Show terminator in texture mapped image ?

/*	VIEWINIT  --  Initialise view generation.  This re-initialises all
				  the static variables above to their compiled-in values,
				  permitting the screen saver to be restarted when run
				  under Scream Saver.  */

void viewInit(void)
{
	lastDrawTime = lastDrawEnd = 0;
	onoon = -1;
	wtab = wtab1 = NULL;
	wtabsize = -1;
	lctime = 0;
	ImageWid = ImageHgt = -1;
	imagePal = NULL;
	svdib = NULL;
	earthBitmap = NULL;
	pmBuf = NULL;
	satvterm = TRUE;
}

//	PALMAPSTART  --  Create a replacement palette bitmap header.

static LPBITMAPINFOHEADER palMapStart(LPBITMAPINFOHEADER bh, int *bmode)
{
	WORD i, ncol, offbits;
	LPBITMAPINFOHEADER bm = NULL;
	unsigned short *palidx;
    
    assert(pmBuf == NULL);
	ncol = (WORD) (bh->biClrUsed == 0 ? (1L << bh->biBitCount) : bh->biClrUsed);
	offbits = (WORD) (bh->biSize + ncol * sizeof(RGBQUAD));
	pmBuf = GlobalAlloc(GMEM_MOVEABLE, (DWORD) offbits);
	if (pmBuf != NULL) {
		bm = GlobalLock(pmBuf);
	}
	if (bm != NULL) {
		*bmode = DIB_PAL_COLORS;
		memcpy(bm, bh, offbits);
		bm->biClrUsed = bm->biClrImportant = 246;
		palidx = (unsigned short *) ((LPBITMAPINFO) bm)->bmiColors; 
		for (i = 0; i < bm->biClrUsed; i++) {
			*palidx++ = (unsigned short) i;
		}
		return bm;
	}
	*bmode = DIB_RGB_COLORS;
	return bh;	
}

//	PALMAPEND  --  Release palette map buffer, if any.

static void palMapEnd(void)
{
	if (pmBuf != NULL) {
		GlobalUnlock(pmBuf);
		GlobalFree(pmBuf);
		pmBuf = NULL;
	}
} 

//  NEEDIMAGEBITMAP  --  Make sure image bitmap and palette are loaded.

#define BackgroundColourIndex	117

void needImageBitmap(void)
{
	int i;
	LPBITMAPINFOHEADER bh, nh;
	DWORD bmpsize, nbmpsize;
	HANDLE earthImageDIB;			// Handle to image DIB in resource
	HANDLE nightImageDIB;			// Night image DIB

	ImageWid = ImageHgt = -1;

	earthImageDIB = LoadResource(hMainInstance,
					FindResource(hMainInstance, MAKEINTRESOURCE(IDB_NASA_DS), RT_BITMAP));
	if (earthImageDIB == NULL) {
		return;
	}

	bh = (LPBITMAPINFOHEADER) LockResource(earthImageDIB);
	if (bh == NULL) {
		return;
	}
	/*	Isn't it great what you have to do to figure out how long a bitmap
		is in memory, thanks to Monkeysoft's allowing the bitmap size and
		colours-used fields to be left as zero?  */
	bmpsize = bh->biSize +
		((bh->biClrUsed == 0 ? (1L << bh->biBitCount) : bh->biClrUsed) * sizeof(RGBQUAD)) +
		((((bh->biWidth + (sizeof(LONG) - 1)) / sizeof(LONG)) * sizeof(LONG)) *
		 bh->biHeight);

	earthBitmap = GlobalAlloc(GMEM_FIXED, bmpsize);
	if (earthBitmap == NULL) {
		UnlockResource(earthImageDIB);
		FreeResource(earthImageDIB);
		earthImageDIB = NULL;
		return;
	}
	memcpy((LPBYTE) earthBitmap, bh, bmpsize); 
	ImageWid = (int) bh->biWidth;
	ImageHgt = (int) bh->biHeight;

	/* If the user wants to see city lights at night, load the night
	   image bitmap and confirm that it is compatible with the day image.
	   If anything goes wrong locating or loading it, or it's found not
	   compatible, clear nightLights to disable night image generation.

	   If all goes well, nightLights will remain set and the night image
	   bitmap will remain locked with the pointer nh to its
	   LPBITMAPINFOHEADER structure. */

	if (nightLights) {
		nightLights = FALSE;				// Assume the worst at the outset
		nightImageDIB = LoadResource(hMainInstance,
						FindResource(hMainInstance, MAKEINTRESOURCE(IDB_NASA_NIGHT), RT_BITMAP));
		if (nightImageDIB != NULL) {
			nh = (LPBITMAPINFOHEADER) LockResource(nightImageDIB);
			if (nh != NULL) {
				nbmpsize = nh->biSize +
					((nh->biClrUsed == 0 ? (1L << nh->biBitCount) : nh->biClrUsed) * sizeof(RGBQUAD)) +
					((((nh->biWidth + (sizeof(LONG) - 1)) / sizeof(LONG)) * sizeof(LONG)) *
					 nh->biHeight);
				if ((nbmpsize != bmpsize) || (nh->biWidth != ImageWid) ||
					(nh->biHeight != ImageHgt)) {
					MessageBox(hWndMain, "Night image not compatible with day image.", NULL, MB_OK);
					UnlockResource(nightImageDIB);
					FreeResource(nightImageDIB);
					nightImageDIB = NULL;
				} else {
					nightLights = TRUE;		// Everything OK--confirm nightLights setting
				}
			}
		}
	}
    
    if (imagePal == NULL) {
	
		/* Extract the day and night colour palettes from the DIB file and
		   construct a memory logical palette embodying those colours.  Then
		   register the palette with the system to provide those colours on
		   the display. */
	
		logpal.palVersion = 0x300;
		logpal.palNumEntries = 246;

		/* Our conventional day and night images consist of pixels with
		   palette indices 128-145, representing at most 117 unique
		   colours in the day image.  Palette indices 0-116 are filled
		   with "subdued" shades of these colours intended to represent
		   the night side.  (The precise definition of "subdued" is left
		   to the producer of the image.  Typically, intensity and
		   contrast are reduced, perhaps with some admixture of blue.)

		   If nightLights is set, rather than flip night pixels of the
		   main image into the subdued colour gamut by clearing the
		   0x80 bit, we will replace them entirely with pixels from the
		   nightImageDIB bitmap.  To permit this pixel replacement, we
		   must construct a hybrid palette here where the first 117
		   colours come from the night image bitmap and the second from
		   the primary portion of the day image bitmap.  */

		for (i = 0; i < 117; i++) {
			RGBQUAD q;
			
			if (nightLights) {
				q = *((LPRGBQUAD) &((LPBITMAPINFO) nh)->bmiColors[i]);
			} else {
				q = *((LPRGBQUAD) &((LPBITMAPINFO) bh)->bmiColors[i]);
			}

			logpal.palPalEntry[i].peRed = q.rgbRed;
			logpal.palPalEntry[i].peGreen = q.rgbGreen;
			logpal.palPalEntry[i].peBlue = q.rgbBlue;
			logpal.palPalEntry[i].peFlags = PC_NOCOLLAPSE;
		}

		logpal.palPalEntry[BackgroundColourIndex].peRed =
			logpal.palPalEntry[BackgroundColourIndex].peGreen =
			logpal.palPalEntry[BackgroundColourIndex].peBlue = 0;
		logpal.palPalEntry[BackgroundColourIndex].peFlags = PC_NOCOLLAPSE;

		/* Now we simply transcribe the sub-palette for the illuminated side
		   from the day image bitmap. */

		for (i = 0; i < 117; i++) {
			RGBQUAD q = *((LPRGBQUAD) &((LPBITMAPINFO) bh)->bmiColors[i + 128]);
	
			logpal.palPalEntry[128 + i].peRed = q.rgbRed;
			logpal.palPalEntry[128 + i].peGreen = q.rgbGreen;
			logpal.palPalEntry[128 + i].peBlue = q.rgbBlue;
			logpal.palPalEntry[128 + i].peFlags = PC_NOCOLLAPSE;
		}
	
		imagePal = CreatePalette((LPLOGPALETTE) &logpal);
	}
	UnlockResource(earthImageDIB);
	FreeResource(earthImageDIB);
	earthImageDIB = NULL;

	// If we were using the night image DIB, unlock and free it now.

	if (nightLights) {
		UnlockResource(nightImageDIB);
		FreeResource(nightImageDIB);
		nightImageDIB = NULL;
	}
}

//  TEXTUREMAPIMAGE  --  Create texture mapped image of globe.

static void textureMapImage(int isize, int tmwidth, int tmheight,
							int diwidth, int diheight, int showterm,
							double satlat, double satlon, double satalt)
{
	Vector satpos, v, n, u, up, ippo, raydir, iv;
	double acslat = abs(cos(satlat)), a, l, alpha, vheight, zclip,
		   vl, ui, vi, b, disc, t, tmwo2pi, tmhopi,
		   normTemp;
	int x, y, tmx, tmy, iso2, tmwo2, tmho2, tmhm1;
	LPBITMAPINFOHEADER bh, obmap;
	WORD ncol, offbits, rowlen, tmrowlen;
	unsigned char *opixel;
	unsigned char *tpixel;
	unsigned char termmask;
	int tickTime = 0;
#define textureTickTimer	20				// Check bailout and ticker this many lines

#define FastVecNorm(a)		assert(VecLen(a) > 0.0); normTemp = 1.0 / VecLen(a); VecScale(normTemp, a)

	termmask = (unsigned char) (showterm ? 0x0 : 0x80);
	bh = (LPBITMAPINFOHEADER) earthBitmap;
	ncol = (WORD) (bh->biClrUsed == 0 ? (1L << bh->biBitCount) : bh->biClrUsed);
	offbits = (WORD) (bh->biSize + ncol * sizeof(RGBQUAD));
	tpixel = ((unsigned char *) (bh)) + offbits;
	tmrowlen = ((tmwidth + (sizeof(LONG) - 1)) / sizeof(LONG)) * sizeof(LONG);
	tmhm1 = tmheight - 1;

	if (svdib != NULL) {
		GlobalFree(svdib);
		svdib = NULL;
	}
	rowlen = ((diwidth + (sizeof(LONG) - 1)) / sizeof(LONG)) * sizeof(LONG);
	svdib = GlobalAlloc(GMEM_MOVEABLE, offbits + ((LONG) diheight) * rowlen);
	obmap = (LPBITMAPINFOHEADER) GlobalLock(svdib);
	memcpy(obmap, bh, offbits);
	obmap->biWidth = diwidth;
	obmap->biHeight = diheight;
	obmap->biSizeImage = ((DWORD) diheight) * rowlen;
	opixel = ((unsigned char *) (obmap)) + offbits;
	memset(opixel, BackgroundColourIndex, obmap->biSizeImage);  

#define PixA(x, y) ((tpixel + ((x) + (((DWORD) (tmhm1 - (y))) * (tmrowlen)))))
#define Opix(x, y) ((opixel + ((x) + (((DWORD) ((diheight - 1) - (y))) * (rowlen)))))

 	a = satalt + EarthRad;
 	assert(a > 0.0);
	MakeVector(a * cos(satlon) * acslat, a * sin(satlon) * acslat, -a * sin(satlat), satpos);
 	l = sqrt(a * a - EarthRad * EarthRad);
 	alpha = asin(EarthRad / a);
 	vheight = l * sin(alpha);
 	zclip = a - (l * cos(alpha));

 	VecCopy(satpos, v);
 	assert(VecLen(v) > 0.0);
 	vl = -1.0 / VecLen(v);
 	VecScale(vl, v);			// Invert and normalise at once

 	// n = Unit direction vector from eye position to North pole
 	MakeVector(0, 0, EarthRad, n);
 	VecSub(n, satpos, n);
 	FastVecNorm(n);

 	// u = v X n -- View right vector
 	VecCross(v, n, u);
 	FastVecNorm(u);

 	// up = u x v -- View up vector
 	VecCross(u, v, up);

 	VecCopy(satpos, ippo);
 	VecScale(zclip / a, ippo);

	// Precompute some values we'll need repeatedly inside the big loop

    iso2 = isize / 2;
    tmwo2 = tmwidth / 2;
    tmho2 = tmheight / 2;
    tmwo2pi = (tmwidth / 2) / PI;
    tmhopi = tmheight / PI;

 	for (y = 0; y < isize; y++) {
		unsigned char *opix = Opix(lleft, y);
		double viXupX, viXupY, viXupZ;
		double visq;

 		vi = -(((double) y) - iso2) / iso2;
 		visq = vi * vi;

 		viXupX = vi * up[X];
 		viXupY = vi * up[Y];
 		viXupZ = vi * up[Z];

 		for (x = 0; x < isize; x++) {
 			ui = (((double) x) - iso2) / iso2;

 			if (((ui * ui) + visq) <= 1.0) {

				raydir[X] = (ippo[X] - (ui * u[X] + viXupX) * vheight) - satpos[X];
				raydir[Y] = (ippo[Y] - (ui * u[Y] + viXupY) * vheight) - satpos[Y];
				raydir[Z] = (ippo[Z] - (ui * u[Z] + viXupZ) * vheight) - satpos[Z];
	 			FastVecNorm(raydir);

				iv[X] = -satpos[X];
				iv[Y] = -satpos[Y];
				iv[Z] = -satpos[Z];
	 			b = VecDot(iv, raydir);
	 			disc = (b * b - VecDot(iv, iv)) + EarthRad * EarthRad;
	 			if (disc >= 0) {			// If disc < 0, no intersection
	 				disc = sqrt(disc);
	 				t = ((b - disc) < 1e-6) ? b + disc : b - disc;

	 					tmx = (int) (tmwo2 - (tmwo2pi * atan2(t * raydir[Y] + satpos[Y], t * raydir[X] + satpos[X])));
	 					tmy = (int) (tmho2 + (tmhopi * asin((t * raydir[Z] + satpos[Z]) / EarthRad)));

						opix[x] = *(PixA(tmx, tmy)) | termmask;
	 			}
	 		}
 		}
		if (++tickTime > textureTickTimer) {
			if (bailout()) {
				GlobalUnlock(svdib);
				return;
			}
			ticker();
			tickTime = 0;
		}
 	}
	GlobalUnlock(svdib);
#undef PixA
#undef Opix
}

//  PROJILLUM  --  Project illuminated area on the map.

static void projillum(short *wtab, int xdots, int ydots, double dec)
{
    int i, ftf = TRUE, ilon, ilat, lilon, lilat, xt;
    double m, x, y, z, th, lon, lat, s, c;

    // Clear unoccupied cells in width table.

    for (i = 0; i < ydots; i++) {
        wtab[i] = -1;
    }

    // Build transformation for declination.

    s = sin(-dtr(dec));
    c = cos(-dtr(dec));

    /* Increment over a semicircle of illumination */

    for (th = -(PI / 2); th <= PI / 2 + 0.001; th += PI / TERMINC) {

        // Transform the point through the declination rotation.

        x = -s * sin(th);
        y = cos(th);
        z = c * sin(th);

        /* Transform the resulting co-ordinate through the
           map projection to obtain screen co-ordinates. */

        lon = (y == 0 && x == 0) ? 0.0 : rtd(atan2(y, x));
        lat = rtd(asin(z));

        ilat = (int) (ydots - (lat + 90) * (ydots / 180.0));
        ilon = (int) (lon * (xdots / 360.0));

        if (ftf) {

            // First time.  Just save start co-ordinate.

            lilon = ilon;
            lilat = ilat;
            ftf = FALSE;
        } else {

            // Trace out the line and set the width table.

            if (lilat == ilat) {
                wtab[(ydots - 1) - ilat] = 2 * (ilon == 0 ? 1 : ilon);
            } else {
                m = ((double) (ilon - lilon)) / (ilat - lilat);
                for (i = lilat; i != ilat; i += sgn(ilat - lilat)) {
                    xt = (int) (lilon + floor((m * (i - lilat)) + 0.5));
                    wtab[(ydots - 1) - i] = 2 * (xt == 0 ? 1 : xt);
                }
            }
            lilon = ilon;
            lilat = ilat;
        }
    }

    /* Now tweak the widths to generate full illumination for
       the correct pole. */

    if (dec < 0.0) {
        ilat = ydots - 1;
        lilat = -1;
    } else {
        ilat = 0;
        lilat = 1;
    }

    for (i = ilat; i != ydots / 2; i += lilat) {
        if (wtab[i] != -1) {
            while (TRUE) {
                wtab[i] = xdots;
                if (i == ilat) {
                    break;
                }
                i -= lilat;
            }
            break;
        }
    }
}

/*  MOVETERM  --  Update illuminated portion of the globe.

    This code is responsible for building, in the in-memory bitmap
	earthBitmap, an image which shows the illuminated and night sides
	of the Earth as defined by the terminator width table wtab.

    The process by which this image is generated is quite different
	when separate night and day images are used as opposed to a single
	image with night and day sub-palettes.  In the former case, we must
	re-initialise the bitmap from the day image resource, then plug in
	pixels from the night image as appropriate.  In the single image
	case, we can flip a pixel from the day to night palette simply by
	clearing the 0x80 bit.  As this operation does not destroy the
	sub-palette index, there is no need to re-copy the bitmap from
	the resource each time.

	Because the cases are so different, we handle them in separate
	functions, with moveterm simply passing on the call to
	moveterm_dayNight when nightLights is set.

*/

static void moveterm_dayNight(short *wtab, int noon, short *otab)
{
    int i, oh, nl, nh;
	LPBITMAPINFOHEADER bh, dh, nbh;
	WORD ncol, offbits, noffbits;
	int wid, hgt, bmode = DIB_RGB_COLORS;
	unsigned char *pa, *dpa, *npa;
	HANDLE earthImageDIB;			// Handle to image DIB in resource
	HANDLE nightImageDIB;			// Night image DIB

//Blooie("Moveterm_dayNight");

	/* Re-copy earthBitmap from the master resource.  Actually, all we need
	   are the pixels, but since they're the vast majority of the resource,
	   we might as well copy the whole thing rather than flailing around
	   trying to extract just the pixels. */

	earthImageDIB = LoadResource(hMainInstance,
					FindResource(hMainInstance, MAKEINTRESOURCE(IDB_NASA_DS), RT_BITMAP));
	if (earthImageDIB == NULL) {
		return;
	}
	dh = (LPBITMAPINFOHEADER) LockResource(earthImageDIB);
	if (dh == NULL) {
		return;
	}

	nightImageDIB = LoadResource(hMainInstance,
					FindResource(hMainInstance, MAKEINTRESOURCE(IDB_NASA_NIGHT), RT_BITMAP));
	if (nightImageDIB == NULL) {
		return;
	}
	nbh = (LPBITMAPINFOHEADER) LockResource(nightImageDIB);
	ncol = (WORD) (nbh->biClrUsed == 0 ? (1L << nbh->biBitCount) : nbh->biClrUsed);
	noffbits = (WORD) (nbh->biSize + ncol * sizeof(RGBQUAD));

	bh = (LPBITMAPINFOHEADER) earthBitmap;
	ncol = (WORD) (bh->biClrUsed == 0 ? (1L << bh->biBitCount) : bh->biClrUsed);
	offbits = (WORD) (bh->biSize + ncol * sizeof(RGBQUAD));

	/* Note that the following code assumes the day and night images are
	   precisely the same size.  This has already been verified above in
	   needImageBitmap: if the images aren't compatible, we'll never get
	   here. */

	wid = ImageWid;
	hgt = ImageHgt;

#define PixA(x, y) ((((unsigned char *) (bh)) + offbits + ((x) + (((DWORD) ((ImageHgt - 1) - (y))) * (((wid + (sizeof(LONG) - 1)) & (~(sizeof(LONG) - 1))))))))
#define dPixA(x, y) ((((unsigned char *) (dh)) + offbits + ((x) + (((DWORD) ((ImageHgt - 1) - (y))) * (((wid + (sizeof(LONG) - 1)) & (~(sizeof(LONG) - 1))))))))
#define nPixA(x, y) ((((unsigned char *) (nbh)) + noffbits + ((x) + (((DWORD) ((ImageHgt - 1) - (y))) * (((wid + (sizeof(LONG) - 1)) & (~(sizeof(LONG) - 1))))))))

    for (i = 0; i < (int) hgt; i++) {
    	if (wtab[i] < 0) {
			/* Image is all night.  Copy entire line from night bitmap. */
    		pa = PixA(0, i);
			npa = nPixA(0, i);
			memcpy(pa, npa, wid);
    	} else {

			//	Fill line with night pixels

            pa = PixA(0, i);
			npa = nPixA(0, i);
			memcpy(pa, npa, wid);

            nl =  ((noon - (wtab[i] / 2)) + wid) % wid;
            nh = (nl + wtab[i]) - 1;

            oh = (nh - nl) + 1;
    		if ((nl + oh) > wid) {
				//	Two segments of illuminated pixels at the left and right
    			pa = PixA(nl, i);
				dpa = dPixA(nl, i);
				memcpy(pa, dpa, wid - nl);

				pa = PixA(0, i);
				dpa = dPixA(0, i);
				memcpy(pa, dpa, ((nl + oh) - wid) + 1);
    		} else {
				//	One segment of illuminated pixels within the image
    			pa = PixA(nl, i);
				dpa = dPixA(nl, i);
				memcpy(pa, dpa, oh + 1);
    		}
    	}
    	otab[i] = wtab[i];
    }
#undef PixA
#undef nPixA

	UnlockResource(earthImageDIB);
	FreeResource(earthImageDIB);
	earthImageDIB = NULL;

	/* Done with the night image.  Release the resource. */

	UnlockResource(nightImageDIB);
	FreeResource(nightImageDIB);
	nightImageDIB = NULL;
}

static void moveterm(short *wtab, int noon, short *otab)
{
    int i, j, oh, nl, nh;
	LPBITMAPINFOHEADER bh, bp;
	WORD ncol, offbits;
	int wid, hgt, bmode = DIB_RGB_COLORS;
	unsigned char *pa;

	if (nightLights) {
		moveterm_dayNight(wtab, noon, otab);
		return;
	}

	bp = bh = (LPBITMAPINFOHEADER) earthBitmap;
	ncol = (WORD) (bh->biClrUsed == 0 ? (1L << bh->biBitCount) : bh->biClrUsed);
	offbits = (WORD) (bh->biSize + ncol * sizeof(RGBQUAD));

	wid = ImageWid;
	hgt = ImageHgt;

#define PixA(x, y) ((((unsigned char *) (bh)) + offbits + ((x) + (((DWORD) ((ImageHgt - 1) - (y))) * (((wid + (sizeof(LONG) - 1)) & (~(sizeof(LONG) - 1))))))))
    for (i = 0; i < (int) hgt; i++) {
    	if (wtab[i] < 0) {
    		pa = PixA(0, i);
     		for (j = 0; j < wid; j++) {
				*pa++ &= 0x7F;
     		}
    	} else {

            /* If both the old and new spans were the entire
               screen, they're equivalent. */

           	if ((otab[i] == wtab[i]) && (wtab[i] == wid)) {
                continue;
            }

            pa = PixA(0, i);
			for (j = 0; j < wid; j++) {
				*pa++ &= 0x7F;
			}

            nl =  ((noon - (wtab[i] / 2)) + wid) % wid;
            nh = (nl + wtab[i]) - 1;

            oh = (nh - nl) + 1;
    		if ((nl + oh) > wid) {
    			pa = PixA(nl, i);
				for (j = nl; j < wid; j++) {
					*pa++ |= 0x80;;
				}
				pa = PixA(0, i);
				for (j = 0; j < (((nl + oh) - wid) + 1); j++) {
					*pa++ |= 0x80;
				}
    		} else {
    			pa = PixA(nl, i);
				for (j = nl; j < ((nl + oh) + 1); j++) {
					*pa++ |= 0x80;
				}
    		}
    	}
    	otab[i] = wtab[i];
    }
#undef PixA
}

/*  PAINTDIB  --  Paint the saved texture mapped DIB into the device surface.  */

static void paintDIB(HDC hDC, int offx, int offy, int isize)
{
	LPBITMAPINFOHEADER obmap, bp;
	WORD ncol, offbits;
	int bmode = DIB_RGB_COLORS, n;
	unsigned char *opixel;
	HPALETTE opal;

	obmap = (LPBITMAPINFOHEADER) GlobalLock(svdib);
	ncol = (WORD) (obmap->biClrUsed == 0 ? (1L << obmap->biBitCount) : obmap->biClrUsed);
	bp = palMapStart(obmap, &bmode);
	offbits = (WORD) (obmap->biSize + ncol * sizeof(RGBQUAD));
	opixel = ((unsigned char *) (obmap)) + offbits;
	opal = SelectPalette(hDC, imagePal, FALSE);
	RealizePalette(hDC);

	n = SetDIBitsToDevice(hDC, offx, offy, isize, isize,
    		0, 0, 0, isize,
    		opixel, (LPBITMAPINFO) bp, bmode);
#ifndef NDEBUG
if (n == 0) MessageBeep(0xFFFFFFFF);
#endif
    palMapEnd();
	GlobalUnlock(svdib);
	SelectPalette(hDC, opal, FALSE);
}

//  UPDVIEWFROM  --  Update view from.

void updviewfrom(HWND hWnd, int isize, int offX, int offY,
				 double satlat, double satlon, double satalt,
				 BOOL repositioned)
{
#ifndef NDEBUG
	HCURSOR oldcur;
#endif
	HWND panel;
	HDC hDC;
    double jt, sunra, sundec, sunrv, sunlong, gt;
	double a, l, alpha, vheight, zclip;
    struct tm ct, lt;
	DWORD drawStart;							// Time this draw began
	int timeYet;								// Time to update ?
	long cctime;
    int xl;

#define Bres(x) ((long) ((x) * (1800 / PI)))	// Fixed point latitude and longitude for tests

	if (earthBitmap == NULL || imagePal == NULL) {
#ifdef TRACY
		Blooie("Wavy!  Could not load bitmap");
#endif
		return;									// Couldn't load bitmap
	}

	/* To avoid hogging the machine, don't update any more frequently than
	   twice the time required to paint the screen.  */
	
	drawStart = GetTickCount();					// Current tick count
	timeYet = drawStart >= (lastDrawEnd + (2 * lastDrawTime));

	/* One more trick--if this is a pure repaint to repair screen damage
	   (e.g. the satellite hasn't changed), and we have a backing bitmap,
	   and it isn't time to recalculate, just repair with the bitmap.
	   Also, if the satellite position hasn't changed measurably, there's
	   no need to recalculate. */

	panel = hWnd;
	if ((svdib != NULL) && !repositioned &&
		(!timeYet || ((bslat == Bres(satlat)) &&
					  (bslon == Bres(satlon)) &&
					  (bsalt == ((long) satalt))
					 )
		)
	   ) {
		return;
	}

    if (wtabsize != ImageHgt) {
    	int i;

    	/* Allocate the projected illumination width tables. */

	    if (wtab != NULL) {
	        free(wtab);
	        free(wtab1);
	    }
	    wtabsize = ImageHgt;
	    wtab = (short *) malloc((unsigned int) wtabsize * sizeof(short));
	    wtab1 = (short *) malloc((unsigned int) wtabsize * sizeof(short));
	    for (i = 0; i < wtabsize; i++) {
	        wtab[i] = wtab1[i] = -1;
	    }

        lctime = 0;
        onoon = -1;
    }

	time(&cctime);				// Only used for recalculation tests
	set_tm_time(&lt, TRUE);
	set_tm_time(&ct, FALSE);

    jt = faketime = jtime(&ct);
    sunpos(jt, FALSE, &sunra, &sundec, &sunrv, &sunlong);
    gt = gmst(jt);
    sunlong = fixangle(180.0 + (sunra - (gt * 15)));
    xl = (int) (sunlong * (ImageWid / 360.0));

    /* Projecting the illumination curve  for the current seasonal
       instant is costly.  If we're running in real time, only  do
       it every PROJINT seconds.  */

    if ((cctime < lctime) || ((cctime - lctime) > PROJINT)) {
        projillum(wtab, ImageWid, ImageHgt, sundec);
        wtabs = wtab;
        wtab = wtab1;
        wtab1 = wtabs;
        lctime = cctime;
		if (bailout()) {
			return;
		}
		ticker();
	}

	if (onoon != xl) {
 		moveterm(wtab1, xl, wtab);
		onoon = xl;
		if (bailout()) {
			return;
		}
		ticker();
	}
#ifndef NDEBUG
    oldcur = SetCursor(LoadCursor(NULL, IDC_WAIT));
    ShowCursor(TRUE);
#endif
	a = satalt + EarthRad;						// Geocentric altitude of satellite
	l = sqrt(a * a - EarthRad * EarthRad);		// Distance to visible limb of Earth
	alpha = asin(EarthRad / a);					// Half-angle subtended by Earth from satellite
	vheight = l * sin(alpha);					// Diameter of visible disc from satellite, km
	zclip = a - (l * cos(alpha));				// Z-clipping plane distance from Earth's centre

	hDC = GetDC(panel);
	lwid = lhgt = isize;
	lleft = 0;

	/* If the image was loaded already or we just succeeded in loading it, generate
	   the texture mapped image. */


	textureMapImage(lwid, ImageWid, ImageHgt, isize, isize, satvterm,
					satlat, satlon, satalt);
	if (bailout()) {
		return;
	}
	lastDrawEnd = GetTickCount();
	lastDrawTime = lastDrawEnd - drawStart;
	bslat = Bres(satlat);
	bslon = Bres(satlon);
	bsalt = (long) satalt;
	if (svdib != NULL) {
		if (repositioned) {
			RECT rc;

		    GetClientRect(hWnd, &rc);
			FillRect(hDC, &rc, (HBRUSH) GetStockObject(BLACK_BRUSH));
		}
		paintDIB(hDC, offX, offY, isize);
	}
#ifdef TRACY
		 else Blooie("Stuzzadelic!  Svdib == NULL!");
#endif

	ReleaseDC(panel, hDC);
#ifndef NDEBUG
	ShowCursor(FALSE);
	SetCursor(oldcur);
#endif
#undef Bres
}

//  RELEASE_SATVIEW_BITMAPS  --  Release view from backing bitmap.

void release_satview_bitmaps(void)
{
	if (svdib != NULL) {
		GlobalFree(svdib);
		svdib = NULL;
	}
}

//	RELEASE_IMAGE  --  Release storage associated with the Earth image

void release_image(void)
{
	if (earthBitmap != NULL) {
		GlobalFree(earthBitmap);
		earthBitmap = NULL;
	}
	if (imagePal != NULL) {
		DeleteObject(imagePal);
		imagePal = NULL;
	}
	if (wtab != NULL) {
	    free(wtab);
	    free(wtab1);
		wtab = wtab1 = NULL;
	}
}
