/*
**********************************************************************
*
* Source file: ListitemSelected.bmp
* Dimensions:  7 * 7
* NumColors:   0
*
**********************************************************************
*/

#include "stdlib.h"

#include "GUI.h"

#include "bitmaps.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif


static GUI_CONST_STORAGE unsigned short acListitemSelected[] = {
  0x3A39, 0x3A39, 0x3A39, 0x3A39, 0x3A39, 0x3A39, 0x3A39,
  0x3A19, 0x8C9E, 0x8C9E, 0x8C9E, 0x8C9E, 0x8C9E, 0x3A19,
  0x39F9, 0x8C9E, 0x7BFE, 0x7BFE, 0x7BFE, 0x8C9E, 0x39F9,
  0x3199, 0x83BE, 0x5A7E, 0x5A7E, 0x5A7E, 0x83BE, 0x3199,
  0x3159, 0x841D, 0x631C, 0x631C, 0x631C, 0x841D, 0x3159,
  0x28F9, 0x8C9C, 0x8C9C, 0x8C9C, 0x8C9C, 0x8C9C, 0x28F9,
  0x28D9, 0x28D9, 0x28D9, 0x28D9, 0x28D9, 0x28D9, 0x28D9
};

GUI_CONST_STORAGE GUI_BITMAP bmListitemSelected = {
  7, /* XSize */
  7, /* YSize */
  14, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)acListitemSelected,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMP565
};

/* *** End of file *** */
