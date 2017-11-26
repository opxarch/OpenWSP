/*
**********************************************************************
*
* Source file: VolumeIcon.bmp
* Dimensions:  31 * 25
* NumColors:   16
*
**********************************************************************
*/

#include "stdlib.h"

#include "GUI.h"

#include "bitmaps.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

/*   Palette
The following are the entries of the palette table.
Every entry is a 32-bit value (of which 24 bits are actually used)
the lower   8 bits represent the Red component,
the middle  8 bits represent the Green component,
the highest 8 bits (of the 24 bits used) represent the Blue component
as follows:   0xBBGGRR
*/

static GUI_CONST_STORAGE GUI_COLOR ColorsVolumeIcon[] = {
     0xC5C2C5,0x000000,0x808080,0xC0C0C0
    ,0xFFFFFF,0x00FFFF,0xFF0000,0x000000
    ,0x000000,0x000000,0x000000,0x000000
    ,0x000000,0x000000,0x000000,0x000000
};

static GUI_CONST_STORAGE GUI_LOGPALETTE PalVolumeIcon = {
  16,	/* number of entries */
  0, 	/* No transparency */
  &ColorsVolumeIcon[0]
};

static GUI_CONST_STORAGE unsigned char acVolumeIcon[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x13, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x13, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x23, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x14, 0x51, 0x33, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x45, 0x31, 0x33, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x14, 0x54, 0x12, 0x33, 0x32, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x45, 0x45, 0x12, 0x33, 0x33, 0x10, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x02, 0x11, 0x14, 0x54, 0x54, 0x12, 0x33, 0x33, 0x10, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
  0x21, 0x45, 0x45, 0x45, 0x45, 0x11, 0x13, 0x33, 0x10, 0x00, 0x10, 0x00, 0x00, 0x10, 0x00, 0x00,
  0x14, 0x54, 0x54, 0x54, 0x54, 0x10, 0x21, 0x33, 0x10, 0x00, 0x10, 0x00, 0x00, 0x10, 0x00, 0x00,
  0x15, 0x45, 0x45, 0x45, 0x45, 0x10, 0x21, 0x33, 0x10, 0x00, 0x10, 0x60, 0x00, 0x10, 0x00, 0x00,
  0x15, 0x55, 0x55, 0x55, 0x55, 0x13, 0x21, 0x30, 0x20, 0x60, 0x10, 0x00, 0x60, 0x10, 0x00, 0x60,
  0x15, 0x35, 0x35, 0x35, 0x35, 0x12, 0x21, 0x06, 0x20, 0x00, 0x10, 0x00, 0x00, 0x10, 0x60, 0x00,
  0x13, 0x53, 0x53, 0x53, 0x53, 0x12, 0x21, 0x30, 0x10, 0x00, 0x10, 0x00, 0x00, 0x10, 0x00, 0x00,
  0x21, 0x33, 0x35, 0x35, 0x35, 0x11, 0x13, 0x33, 0x10, 0x00, 0x10, 0x00, 0x00, 0x10, 0x00, 0x00,
  0x02, 0x11, 0x13, 0x33, 0x53, 0x12, 0x33, 0x33, 0x10, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x33, 0x35, 0x12, 0x33, 0x33, 0x10, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x13, 0x33, 0x12, 0x33, 0x32, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x33, 0x21, 0x33, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x13, 0x31, 0x33, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x31, 0x23, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x13, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x13, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

GUI_CONST_STORAGE GUI_BITMAP bmVolumeIcon = {
  31, /* XSize */
  25, /* YSize */
  16, /* BytesPerLine */
  4, /* BitsPerPixel */
  acVolumeIcon,  /* Pointer to picture data (indices) */
  &PalVolumeIcon  /* Pointer to palette */
};

/* *** End of file *** */