/*
 *  OpenWSP (an opensource webstream codec)
 *  Copyleft (C) 2016, The first Middle School in Yongsheng Lijiang China
 *  please contact with <diyer175@hotmail.com> if you have any problems.
 *
 *  This project is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License(GPL)
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#ifndef MS_HDR_H_
#define MS_HDR_H_

namespace openwsp {

#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_
typedef struct __attribute__((__packed__)) _WAVEFORMATEX {
  unsigned short  wFormatTag;
  unsigned short  nChannels;
  unsigned int    nSamplesPerSec;
  unsigned int    nAvgBytesPerSec;
  unsigned short  nBlockAlign;
  unsigned short  wBitsPerSample;
  unsigned short  cbSize;
} WAVEFORMATEX, *PWAVEFORMATEX, *NPWAVEFORMATEX, *LPWAVEFORMATEX;
#endif /* _WAVEFORMATEX_ */

#ifndef _MPEGLAYER3WAVEFORMAT_
#define _MPEGLAYER3WAVEFORMAT_
typedef struct __attribute__((__packed__)) mpeglayer3waveformat_tag {
  WAVEFORMATEX wf;
  unsigned short wID;
  unsigned int   fdwFlags;
  unsigned short nBlockSize;
  unsigned short nFramesPerBlock;
  unsigned short nCodecDelay;
} MPEGLAYER3WAVEFORMAT;
#endif /* _MPEGLAYER3WAVEFORMAT_ */

/* windows.h #includes wingdi.h on MinGW. */
#if !defined(_BITMAPINFOHEADER_) && !defined(_WINGDI_H)
#define _BITMAPINFOHEADER_
typedef struct __attribute__((__packed__))
{
    int 	biSize;
    int  	biWidth;
    int  	biHeight;
    short 	biPlanes;
    short 	biBitCount;
    int 	biCompression;
    int 	biSizeImage;
    int  	biXPelsPerMeter;
    int  	biYPelsPerMeter;
    int 	biClrUsed;
    int 	biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER, *LPBITMAPINFOHEADER;
typedef struct {
	BITMAPINFOHEADER bmiHeader;
	int	bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO;
#endif

#ifndef le2me_BITMAPINFOHEADER
#ifdef WORDS_BIGENDIAN
#define le2me_BITMAPINFOHEADER(h) {					\
    (h)->biSize = le2me_32((h)->biSize);				\
    (h)->biWidth = le2me_32((h)->biWidth);				\
    (h)->biHeight = le2me_32((h)->biHeight);				\
    (h)->biPlanes = le2me_16((h)->biPlanes);				\
    (h)->biBitCount = le2me_16((h)->biBitCount);			\
    (h)->biCompression = le2me_32((h)->biCompression);			\
    (h)->biSizeImage = le2me_32((h)->biSizeImage);			\
    (h)->biXPelsPerMeter = le2me_32((h)->biXPelsPerMeter);		\
    (h)->biYPelsPerMeter = le2me_32((h)->biYPelsPerMeter);		\
    (h)->biClrUsed = le2me_32((h)->biClrUsed);				\
    (h)->biClrImportant = le2me_32((h)->biClrImportant);		\
}
#define le2me_WAVEFORMATEX(h) {						\
    (h)->wFormatTag = le2me_16((h)->wFormatTag);			\
    (h)->nChannels = le2me_16((h)->nChannels);				\
    (h)->nSamplesPerSec = le2me_32((h)->nSamplesPerSec);		\
    (h)->nAvgBytesPerSec = le2me_32((h)->nAvgBytesPerSec);		\
    (h)->nBlockAlign = le2me_16((h)->nBlockAlign);			\
    (h)->wBitsPerSample = le2me_16((h)->wBitsPerSample);		\
    (h)->cbSize = le2me_16((h)->cbSize);				\
}
#else
#define le2me_BITMAPINFOHEADER(h)   /**/
#define le2me_WAVEFORMATEX(h)	    /**/
#endif
#endif

} // namespace openwsp


#endif //!defined(MS_HDR_H_)