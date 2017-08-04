/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              u/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : F4x6.C
Purpose     : Implementation of 4x6 pixel font
Height      : 6
---------------------------END-OF-HEADER------------------------------
*/


#include "GUI_FontIntern.h"

GUI_CONST_STORAGE unsigned char acFont4x6[95][6] = {

  {
   ________,
   ________,
   ________,
   ________,
   ________,
   ________}

 ,{
   _X______,
   _X______,
   _X______,
   ________,
   _X______,
   ________}

 ,{
   X_X_____,
   X_X_____,
   ________,
   ________,
   ________,
   ________}

 ,{
   X_X_____,
   XXX_____,
   X_X_____,
   XXX_____,
   X_X_____,
   ________}

 ,{
   _XX_____,
   XX______,
   _X______,
   _XX_____,
   XX______,
   ________}

 ,{
   X_______,
   __X_____,
   _X______,
   X_______,
   __X_____,
   ________}

 ,{
   _X______,
   X_X_____,
   _X______,
   X_XX____,
   _X_X____,
   ________}

 ,{
   _X______,
   _X______,
   ________,
   ________,
   ________,
   ________}

 ,{
   __X_____,
   _X______,
   _X______,
   _X______,
   __X_____,
   ________}

 ,{
   _X______,
   __X_____,
   __X_____,
   __X_____,
   _X______,
   ________}

 ,{
   X_X_____,
   _X______,
   XXX_____,
   _X______,
   X_X_____,
   ________}

 ,{
   ________,
   _X______,
   XXX_____,
   _X______,
   ________,
   ________}

 ,{
   ________,
   ________,
   ________,
   _X______,
   X_______,
   ________}

 ,{
   ________,
   ________,
   XXX_____,
   ________,
   ________,
   ________}

 ,{
   ________,
   ________,
   ________,
   ________,
   _X______,
   ________}

 ,{
   ________,
   __X_____,
   _X______,
   X_______,
   ________,
   ________}

 ,{
   _X______,
   X_X_____,
   X_X_____,
   X_X_____,
   _X______,
   ________}

 ,{
   _X______,
   XX______,
   _X______,
   _X______,
   XXX_____,
   ________}

 ,{
   _X______,
   X_X_____,
   __X_____,
   _X______,
   XXX_____,
   ________}

 ,{
   XX______,
   __X_____,
   _X______,
   __X_____,
   XX______,
   ________}

 ,{
   __X_____,
   _XX_____,
   X_X_____,
   XXX_____,
   __X_____,
   ________}

 ,{
   XXX_____,
   X_______,
   XX______,
   __X_____,
   XX______,
   ________}

 ,{
   _XX_____,
   X_______,
   XXX_____,
   X_X_____,
   XXX_____,
   ________}

 ,{
   XXX_____,
   __X_____,
   _X______,
   X_______,
   X_______,
   ________}

 ,{
   _X______,
   X_X_____,
   _X______,
   X_X_____,
   _X______,
   ________}

 ,{
   XXX_____,
   X_X_____,
   XXX_____,
   __X_____,
   XX______,
   ________}

 ,{
   ________,
   ________,
   _X______,
   ________,
   _X______,
   ________}

 ,{
   ________,
   ________,
   _X______,
   ________,
   _X______,
   X_______}

 ,{
   __X_____,
   _X______,
   X_______,
   _X______,
   __X_____,
   ________}

 ,{
   ________,
   XXX_____,
   ________,
   XXX_____,
   ________,
   ________}

 ,{
   X_______,
   _X______,
   __X_____,
   _X______,
   X_______,
   ________}

 ,{
   XX______,
   __X_____,
   _X______,
   ________,
   _X______,
   ________}

 ,{
   _X______,
   X_X_____,
   __X_____,
   XXX_____,
   XXX_____,
   ________}

 ,{
   _X______,
   X_X_____,
   XXX_____,
   X_X_____,
   X_X_____,
   ________}

 ,{
   XX______,
   X_X_____,
   XX______,
   X_X_____,
   XX______,
   ________}

 ,{
   _X______,
   X_X_____,
   X_______,
   X_X_____,
   _X______,
   ________}

 ,{
   XX______,
   X_X_____,
   X_X_____,
   X_X_____,
   XX______,
   ________}

 ,{
   XXX_____,
   X_______,
   XX______,
   X_______,
   XXX_____,
   ________}

 ,{
   XXX_____,
   X_______,
   XX______,
   X_______,
   X_______,
   ________}

 ,{
   _XX_____,
   X_______,
   XXX_____,
   X_X_____,
   _XX_____,
   ________}

 ,{
   X_X_____,
   X_X_____,
   XXX_____,
   X_X_____,
   X_X_____,
   ________}

 ,{
   XXX_____,
   _X______,
   _X______,
   _X______,
   XXX_____,
   ________}

 ,{
   XXX_____,
   __X_____,
   __X_____,
   X_X_____,
   _X______,
   ________}

 ,{
   X_X_____,
   XX______,
   X_______,
   XX______,
   X_X_____,
   ________}

 ,{
   X_______,
   X_______,
   X_______,
   X_______,
   XXX_____,
   ________}

 ,{
   X_X_____,
   XXX_____,
   X_X_____,
   X_X_____,
   X_X_____,
   ________}

 ,{
   X_X_____,
   XXX_____,
   XXX_____,
   XXX_____,
   X_X_____,
   ________}

 ,{
   XXX_____,
   X_X_____,
   X_X_____,
   X_X_____,
   XXX_____,
   ________}

 ,{
   XXX_____,
   X_X_____,
   XXX_____,
   X_______,
   X_______,
   ________}

 ,{
   XXX_____,
   X_X_____,
   X_X_____,
   X_X_____,
   XXX_____,
   __X_____}

 ,{
   XX______,
   X_X_____,
   XX______,
   X_X_____,
   X_X_____,
   ________}

 ,{
   _XX_____,
   X_______,
   _X______,
   __X_____,
   XX______,
   ________}

 ,{
   XXX_____,
   _X______,
   _X______,
   _X______,
   _X______,
   ________}

 ,{
   X_X_____,
   X_X_____,
   X_X_____,
   X_X_____,
   XXX_____,
   ________}

 ,{
   X_X_____,
   X_X_____,
   X_X_____,
   X_X_____,
   _X______,
   ________}

 ,{
   X_X_____,
   X_X_____,
   X_X_____,
   XXX_____,
   X_X_____,
   ________}

 ,{
   X_X_____,
   X_X_____,
   _X______,
   X_X_____,
   X_X_____,
   ________}

 ,{
   X_X_____,
   X_X_____,
   X_X_____,
   _X______,
   _X______,
   ________}

 ,{
   XXX_____,
   __X_____,
   _X______,
   X_______,
   XXX_____,
   ________}

 ,{
   XXX_____,
   X_______,
   X_______,
   X_______,
   XXX_____,
   ________}


 ,{
   X_______,
   X_______,
   _X______,
   __X_____,
   __X_____,
   ________}


 ,{
   XXX_____,
   __X_____,
   __X_____,
   __X_____,
   XXX_____,
   ________}


 ,{
   _X______,
   X_X_____,
   ________,
   ________,
   ________,
   ________}


 ,{
   ________,
   ________,
   ________,
   ________,
   ________,
   XXX_____}


 ,{
   X_______,
   _X______,
   ________,
   ________,
   ________,
   ________}


 ,{
   ________,
   ________,
   _XX_____,
   X_X_____,
   _XX_____,
   ________}

 ,{
   X_______,
   X_______,
   XX______,
   X_X_____,
   XX______,
   ________}

 ,{
   ________,
   ________,
   _XX_____,
   X_______,
   _XX_____,
   ________}

 ,{
   __X_____,
   __X_____,
   _XX_____,
   X_X_____,
   _XX_____,
   ________}

 ,{
   _X______,
   X_X_____,
   XXX_____,
   X_______,
   _XX_____,
   ________}

 ,{
   _XX_____,
   X_______,
   XX______,
   X_______,
   X_______,
   ________}

 ,{
   ________,
   _XX_____,
   X_X_____,
   _XX_____,
   __X_____,
   XX______}

 ,{
   X_______,
   X_______,
   XX______,
   X_X_____,
   X_X_____,
   ________}

 ,{
   _X______,
   ________,
   XX______,
   _X______,
   XXX_____,
   ________}

 ,{
   __X_____,
   ________,
   __X_____,
   __X_____,
   X_X_____,
   _X______}

 ,{
   X_______,
   X_______,
   X_X_____,
   XX______,
   X_X_____,
   ________}

 ,{
   XX______,
   _X______,
   _X______,
   _X______,
   XXX_____,
   ________}

 ,{
   ________,
   X_X_____,
   XXX_____,
   X_X_____,
   X_X_____,
   ________}

 ,{
   ________,
   ________,
   XXX_____,
   X_X_____,
   X_X_____,
   ________}

 ,{
   ________,
   ________,
   XXX_____,
   X_X_____,
   XXX_____,
   ________}

 ,{
   ________,
   ________,
   XX______,
   X_X_____,
   XX______,
   X_______}

 ,{
   ________,
   ________,
   XXX_____,
   X_X_____,
   XXX_____,
   __X_____}

 ,{
   ________,
   ________,
   X_X_____,
   XX______,
   X_______,
   ________}

 ,{
   _XX_____,
   X_______,
   _X______,
   __X_____,
   XX______,
   ________}

 ,{
   _X______,
   XXX_____,
   _X______,
   _X______,
   __X_____,
   ________}

 ,{
   ________,
   ________,
   X_X_____,
   X_X_____,
   XXX_____,
   ________}

 ,{
   ________,
   ________,
   X_X_____,
   X_X_____,
   _X______,
   ________}

 ,{
   ________,
   ________,
   X_X_____,
   XXX_____,
   X_X_____,
   ________}

 ,{
   ________,
   ________,
   X_X_____,
   _X______,
   X_X_____,
   ________}

 ,{
   ________,
   ________,
   X_X_____,
   XXX_____,
   __X_____,
   XX______}

 ,{
   ________,
   XXX_____,
   _X______,
   X_______,
   XXX_____,
   ________}

 ,{
   X_______,
   _X______,
   _XX_____,
   _X______,
   X_______,
   ________}

 ,{
   _X______,
   _X______,
   _X______,
   _X______,
   _X______,
   ________}

 ,{
   __X_____,
   _X______,
   XX______,
   _X______,
   __X_____,
   ________}

 ,{
   _X_X____,
   X_X_____,
   ________,
   ________,
   ________,
   ________}

};



GUI_CONST_STORAGE GUI_FONT_MONO FontMono4x6 = {
 acFont4x6[0],
 (const U8 *)0,
 (const GUI_FONT_TRANSINFO *)0,     /* ASCII only, no translation table */
 32, 126, 4, 4, 1
};


GUI_CONST_STORAGE GUI_FONT GUI_Font4x6 = {
  GUI_FONTTYPE_MONO,
  6, 6, 1, 1
  ,{&FontMono4x6}
  , 5, 3, 5
};
 GUI_FONT GUI_Font4x6_2 = {
  GUI_FONTTYPE_MONO,
  6, 6*2, 2, 2
  ,{&FontMono4x6}
  , 5, 3, 5
};
