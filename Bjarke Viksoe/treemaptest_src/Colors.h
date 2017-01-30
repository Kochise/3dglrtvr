
#pragma once

#pragma warning(disable : 4244)


#define  HSLMAX   240   /* H,L, and S vary over 0-HSLMAX */
#define  RGBMAX   255   /* R,G, and B vary over 0-RGBMAX */
                        /* HSLMAX BEST IF DIVISIBLE BY 6 */
                        /* RGBMAX, HSLMAX must each fit in a byte. */
#define UNDEFINED_HUE (HSLMAX*2/3)


class CColors
{
public:
   static COLORREF BlendRGB(COLORREF c1, COLORREF c2, WORD wPercent)
   {
      return RGB( GetRValue(c1) + ((GetRValue(c2) - GetRValue(c1)) * wPercent / 100L),
           GetGValue(c1) + ((GetGValue(c2) - GetGValue(c1)) * wPercent / 100L),
           GetBValue(c1) + ((GetBValue(c2) - GetBValue(c1)) * wPercent / 100L) ); 
   }

   static RGBQUAD RGB2RGBQUAD(COLORREF cr)
   {
      RGBQUAD c;
      c.rgbRed = GetRValue(cr);    // Get R, G, and B out of DWORD 
      c.rgbGreen = GetGValue(cr);
      c.rgbBlue = GetBValue(cr);
      c.rgbReserved = 0;
      return c;
   }

   static COLORREF RGBQUAD2RGB(RGBQUAD c)
   {
      return RGB(c.rgbRed, c.rgbGreen, c.rgbBlue);
   }

   static RGBQUAD RGBtoHSL(RGBQUAD lRGBColor)
   {
      BYTE H,L,S;                  // output HSL values
      BYTE cMax,cMin;              // max and min RGB values
      WORD Rdelta,Gdelta,Bdelta;   // intermediate value: % of spread from max
      BYTE R = lRGBColor.rgbRed;   // get R, G, and B out of DWORD
      BYTE G = lRGBColor.rgbGreen;
      BYTE B = lRGBColor.rgbBlue;
      cMax = max(max(R,G), B);     // calculate lightness
      cMin = min(min(R,G), B);
      L = (((cMax + cMin) * HSLMAX) + RGBMAX) / (2 * RGBMAX);
      if( cMax == cMin ) {         // r=g=b --> achromatic case
         S = 0;                    // saturation
         H = UNDEFINED_HUE;        // hue
      } 
      else {                        // chromatic case
         if( L <= (HSLMAX / 2) )    // saturation
            S = (((cMax - cMin)*HSLMAX) + ((cMax + cMin) / 2)) / (cMax + cMin);
         else
            S = (((cMax - cMin)*HSLMAX) + ((2 * RGBMAX - cMax - cMin) / 2)) / (2 * RGBMAX - cMax - cMin);
         /* hue */
         Rdelta = ( ((cMax - R) * (HSLMAX / 6)) + ((cMax - cMin) /2) ) / (cMax - cMin);
         Gdelta = ( ((cMax - G) * (HSLMAX / 6)) + ((cMax - cMin) /2) ) / (cMax - cMin);
         Bdelta = ( ((cMax - B) * (HSLMAX / 6)) + ((cMax - cMin) /2) ) / (cMax - cMin);

         if( R == cMax)
            H = Bdelta - Gdelta;
         else if( G == cMax)
            H = (HSLMAX/3) + Rdelta - Bdelta;
         else /* B == cMax */
            H = ((2*HSLMAX)/3) + Gdelta - Rdelta;

         if( H < 0) H += HSLMAX;
         if( H > HSLMAX) H -= HSLMAX;
      }
      RGBQUAD hsl={L,S,H,0};
      return hsl;
   }

   static WORD HueToRGB(WORD n1, WORD n2, WORD hue)
   { 
      // Range check: note values passed add/subtract thirds of range
      if( hue < 0) hue += HSLMAX;
      if( hue > HSLMAX) hue -= HSLMAX;
      // Return r,g, or b value from this tridrant
      if( hue < (HSLMAX / 6) ) return (WORD)( n1 + (((n2 - n1) * hue + (HSLMAX / 12)) / (HSLMAX / 6)) );
      if( hue < (HSLMAX / 2) ) return n2;
      if( hue < ((HSLMAX * 2) / 3) ) return (WORD)( n1 + (((n2 - n1) * (((HSLMAX * 2) / 3) - hue) + (HSLMAX / 12)) / (HSLMAX / 6))); 
      else return n1;
   } 

   static RGBQUAD HSLtoRGB(RGBQUAD lHSLColor)
   { 
      BYTE R,G,B;                 // RGB component values
      WORD Magic1,Magic2;         // calculated magic numbers (really!)
      WORD hue = lHSLColor.rgbRed;     // get H, S, and L out of DWORD
      WORD sat = lHSLColor.rgbGreen;
      WORD lum = lHSLColor.rgbBlue;
      if( sat == 0 ) {            // achromatic case
         R = G = B = (lum*RGBMAX)/HSLMAX;
      } 
      else {                      // chromatic case
         // Set up magic numbers
         if( lum <= (HSLMAX/2) ) Magic2 = (WORD)((lum * (HSLMAX + sat) + (HSLMAX / 2)) / HSLMAX);
         else Magic2 = (WORD)(lum + sat - ((lum * sat) + (HSLMAX / 2)) / HSLMAX);
         Magic1 = 2 * lum - Magic2;
         // Get RGB, change units from HSLMAX to RGBMAX
         R = (HueToRGB(Magic1, Magic2, (WORD)(hue+(HSLMAX / 3))) * RGBMAX + (HSLMAX / 2)) / HSLMAX; 
         G = (HueToRGB(Magic1, Magic2, hue) * RGBMAX + (HSLMAX / 2)) / HSLMAX;
         B = (HueToRGB(Magic1, Magic2, (WORD)(hue-(HSLMAX / 3))) * RGBMAX + (HSLMAX / 2)) / HSLMAX;
      }
      RGBQUAD rgb={ B, G, R, 0 };
      return rgb;
   }

   static RGBQUAD HSLtoRGB(COLORREF cHSLColor)
   {
      return HSLtoRGB(RGB2RGBQUAD(cHSLColor));
   }
};


#pragma warning(default: 4244)

