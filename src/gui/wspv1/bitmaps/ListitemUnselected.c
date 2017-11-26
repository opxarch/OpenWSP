/*
**********************************************************************
*
* Source file: ListitemUnselected.bmp
* Dimensions:  8 * 8
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


static GUI_CONST_STORAGE unsigned short acListitemUnselected[] = {
  0xFFFF, 0xFFFF, 0x95AE, 0x4C00, 0x64A5, 0x95AE, 0xFFFF, 0xFFFF,
  0xFFFF, 0x7509, 0x754B, 0x9E96, 0x9633, 0x754B, 0x7509, 0xFFFF,
  0x8DAE, 0x752A, 0x9E75, 0x9E75, 0x9E75, 0x9E75, 0x752A, 0x8DAE,
  0x4C00, 0x85F0, 0x85F0, 0x85F0, 0x85F0, 0x85F0, 0x85F0, 0x4C00,
  0x5463, 0x754C, 0x758E, 0x758E, 0x758E, 0x758E, 0x754C, 0x5463,
  0x856C, 0x5CC7, 0x85D1, 0x85D1, 0x85D1, 0x85D1, 0x64A7, 0x856C,
  0xFFFF, 0x6CE8, 0x64C9, 0x8DF2, 0x7DB0, 0x64C9, 0x6CE8, 0xFFFF,
  0xFFFF, 0xFFFF, 0x8D8D, 0x4C00, 0x64A5, 0x8D8D, 0xFFFF, 0xFFFF
};

GUI_CONST_STORAGE GUI_BITMAP bmListitemUnselected = {
  8, /* XSize */
  8, /* YSize */
  16, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)acListitemUnselected,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMP565
};

/* *** End of file *** */
