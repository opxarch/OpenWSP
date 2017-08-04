## @file
# The makefile for avcodec
#

#
#  OpenWSP (an opensource web stream player)
#  Copyleft (C) 2016, The 1st Middle School in Yongsheng Lijiang China
#  please contact with <diyer175@hotmail.com> if you have any problems.
#
#  This project is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public License(GPL)
#  as published by the Free Software Foundation; either version 2.1
#  of the License, or (at your option) any later version.
#
#  This project is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#

SUB_DEPTH = ./../../..

include $(SUB_DEPTH)/autoconf.mk


#########################################################################
# Targets.

OBJS = allcodecs.c.o \
       audioconvert.c.o \
       bitstream.c.o \
       bitstream_filter.c.o \
       dsputil.c.o \
       error_resilience.c.o \
       eval.c.o \
       faandct.c.o \
       imgconvert.c.o \
       jrevdct.c.o \
       jfdctfst.c.o \
       jfdctint.c.o\
       mpeg12.c.o mpeg12data.c.o \
       mpegvideo.c.o \
       opt.c.o \
       parser.c.o \
       raw.c.o \
       resample.c.o \
       resample2.c.o \
       simple_idct.c.o \
       utils.c.o \


OBJS-$(CONFIG_AASC_DECODER)            += aasc.c.o
OBJS-$(CONFIG_AC3_DECODER)             += ac3dec.c.o ac3tab.c.o ac3.c.o mdct.c.o fft.c.o
OBJS-$(CONFIG_AC3_ENCODER)             += ac3enc.c.o ac3tab.c.o ac3.c.o
OBJS-$(CONFIG_ALAC_DECODER)            += alac.c.o
OBJS-$(CONFIG_AMV_DECODER)             += sp5xdec.c.o mjpegdec.c.o mjpeg.c.o
OBJS-$(CONFIG_APE_DECODER)             += apedec.c.o
OBJS-$(CONFIG_ASV1_DECODER)            += asv1.c.o
OBJS-$(CONFIG_ASV1_ENCODER)            += asv1.c.o
OBJS-$(CONFIG_ASV2_DECODER)            += asv1.c.o
OBJS-$(CONFIG_ASV2_ENCODER)            += asv1.c.o
OBJS-$(CONFIG_ATRAC3_DECODER)          += atrac3.c.o mdct.c.o fft.c.o
OBJS-$(CONFIG_AVS_DECODER)             += avs.c.o
OBJS-$(CONFIG_BETHSOFTVID_DECODER)     += bethsoftvideo.c.o
OBJS-$(CONFIG_BMP_DECODER)             += bmp.c.o
OBJS-$(CONFIG_BMP_ENCODER)             += bmpenc.c.o
OBJS-$(CONFIG_C93_DECODER)             += c93.c.o
OBJS-$(CONFIG_CAVS_DECODER)            += cavs.c.o cavsdec.c.o cavsdsp.c.o golomb.c.o
OBJS-$(CONFIG_CINEPAK_DECODER)         += cinepak.c.o
OBJS-$(CONFIG_CLJR_DECODER)            += cljr.c.o
OBJS-$(CONFIG_CLJR_ENCODER)            += cljr.c.o
OBJS-$(CONFIG_COOK_DECODER)            += cook.c.o mdct.c.o fft.c.o
OBJS-$(CONFIG_CSCD_DECODER)            += cscd.c.o
OBJS-$(CONFIG_CYUV_DECODER)            += cyuv.c.o
OBJS-$(CONFIG_DCA_DECODER)             += dca.c.o
OBJS-$(CONFIG_DNXHD_DECODER)           += dnxhddec.c.o dnxhddata.c.o
OBJS-$(CONFIG_DSICINVIDEO_DECODER)     += dsicinav.c.o
OBJS-$(CONFIG_DSICINAUDIO_DECODER)     += dsicinav.c.o
OBJS-$(CONFIG_DVBSUB_DECODER)          += dvbsubdec.c.o
OBJS-$(CONFIG_DVBSUB_ENCODER)          += dvbsub.c.o
OBJS-$(CONFIG_DVDSUB_DECODER)          += dvdsubdec.c.o
OBJS-$(CONFIG_DVDSUB_ENCODER)          += dvdsubenc.c.o
OBJS-$(CONFIG_DVVIDEO_DECODER)         += dv.c.o
OBJS-$(CONFIG_DVVIDEO_ENCODER)         += dv.c.o
OBJS-$(CONFIG_DXA_DECODER)             += dxa.c.o
OBJS-$(CONFIG_EIGHTBPS_DECODER)        += 8bps.c.o
OBJS-$(CONFIG_FFV1_DECODER)            += ffv1.c.o rangecoder.c.o golomb.c.o
OBJS-$(CONFIG_FFV1_ENCODER)            += ffv1.c.o rangecoder.c.o
OBJS-$(CONFIG_FFVHUFF_DECODER)         += huffyuv.c.o
OBJS-$(CONFIG_FFVHUFF_ENCODER)         += huffyuv.c.o
OBJS-$(CONFIG_FLAC_DECODER)            += flac.c.o golomb.c.o
OBJS-$(CONFIG_FLAC_ENCODER)            += flacenc.c.o golomb.c.o
OBJS-$(CONFIG_FLASHSV_DECODER)         += flashsv.c.o
OBJS-$(CONFIG_FLASHSV_ENCODER)         += flashsvenc.c.o
OBJS-$(CONFIG_FLIC_DECODER)            += flicvideo.c.o
OBJS-$(CONFIG_FLV_DECODER)             += h263dec.c.o h263.c.o
OBJS-$(CONFIG_FLV_ENCODER)             += mpegvideo_enc.c.o motion_est.c.o ratecontrol.c.o h263.c.o
OBJS-$(CONFIG_FOURXM_DECODER)          += 4xm.c.o
OBJS-$(CONFIG_FRAPS_DECODER)           += fraps.c.o
OBJS-$(CONFIG_GIF_DECODER)             += gifdec.c.o lzw.c.o
OBJS-$(CONFIG_GIF_ENCODER)             += gif.c.o
OBJS-$(CONFIG_H261_DECODER)            += h261dec.c.o h261.c.o
OBJS-$(CONFIG_H261_ENCODER)            += h261enc.c.o h261.c.o mpegvideo_enc.c.o motion_est.c.o ratecontrol.c.o
OBJS-$(CONFIG_H263_DECODER)            += h263dec.c.o h263.c.o
OBJS-$(CONFIG_H263I_DECODER)           += h263dec.c.o h263.c.o
OBJS-$(CONFIG_H263_ENCODER)            += mpegvideo_enc.c.o motion_est.c.o ratecontrol.c.o h263.c.o
OBJS-$(CONFIG_H263P_ENCODER)           += mpegvideo_enc.c.o motion_est.c.o ratecontrol.c.o h263.c.o
OBJS-$(CONFIG_H264_DECODER)            += h264.c.o h264idct.c.o h264pred.c.o cabac.c.o golomb.c.o
OBJS-$(CONFIG_H264_ENCODER)            += h264enc.c.o h264dspenc.c.o
OBJS-$(CONFIG_HUFFYUV_DECODER)         += huffyuv.c.o
OBJS-$(CONFIG_HUFFYUV_ENCODER)         += huffyuv.c.o
OBJS-$(CONFIG_IDCIN_DECODER)           += idcinvideo.c.o
OBJS-$(CONFIG_IMC_DECODER)             += imc.c.o mdct.c.o fft.c.o
OBJS-$(CONFIG_INDEO2_DECODER)          += indeo2.c.o
OBJS-$(CONFIG_INDEO3_DECODER)          += indeo3.c.o
OBJS-$(CONFIG_INTERPLAY_VIDEO_DECODER) += interplayvideo.c.o
OBJS-$(CONFIG_INTERPLAY_DPCM_DECODER)  += dpcm.c.o
OBJS-$(CONFIG_JPEGLS_DECODER)          += jpeglsdec.c.o jpegls.c.o mjpegdec.c.o mjpeg.c.o golomb.c.o
OBJS-$(CONFIG_JPEGLS_ENCODER)          += jpeglsenc.c.o jpegls.c.o golomb.c.o
OBJS-$(CONFIG_KMVC_DECODER)            += kmvc.c.o
OBJS-$(CONFIG_LJPEG_ENCODER)           += ljpegenc.c.o mjpegenc.c.o mjpeg.c.o mpegvideo_enc.c.o motion_est.c.o ratecontrol.c.o
OBJS-$(CONFIG_LOCO_DECODER)            += loco.c.o golomb.c.o
OBJS-$(CONFIG_MACE3_DECODER)           += mace.c.o
OBJS-$(CONFIG_MACE6_DECODER)           += mace.c.o
OBJS-$(CONFIG_MJPEG_DECODER)           += mjpegdec.c.o mjpeg.c.o
OBJS-$(CONFIG_MJPEG_ENCODER)           += mjpegenc.c.o mjpeg.c.o mpegvideo_enc.c.o motion_est.c.o ratecontrol.c.o
OBJS-$(CONFIG_MJPEGB_DECODER)          += mjpegbdec.c.o mjpegdec.c.o mjpeg.c.o
OBJS-$(CONFIG_MMVIDEO_DECODER)         += mmvideo.c.o
OBJS-$(CONFIG_MP2_DECODER)             += mpegaudiodec.c.o mpegaudiodecheader.c.o mpegaudio.c.o mpegaudiodata.c.o
OBJS-$(CONFIG_MP2_ENCODER)             += mpegaudioenc.c.o mpegaudio.c.o mpegaudiodata.c.o
OBJS-$(CONFIG_MP3_DECODER)             += mpegaudiodec.c.o mpegaudiodecheader.c.o mpegaudio.c.o mpegaudiodata.c.o
OBJS-$(CONFIG_MP3ADU_DECODER)          += mpegaudiodec.c.o mpegaudiodecheader.c.o mpegaudio.c.o mpegaudiodata.c.o
OBJS-$(CONFIG_MP3ON4_DECODER)          += mpegaudiodec.c.o mpegaudiodecheader.c.o mpegaudio.c.o mpegaudiodata.c.o
OBJS-$(CONFIG_MPC7_DECODER)            += mpc.c.o mpegaudiodec.c.o mpegaudiodecheader.c.o mpegaudio.c.o mpegaudiodata.c.o
OBJS-$(CONFIG_MPEG1VIDEO_ENCODER)      += mpeg12enc.c.o mpeg12data.c.o mpegvideo_enc.c.o motion_est.c.o ratecontrol.c.o
OBJS-$(CONFIG_MPEG2VIDEO_ENCODER)      += mpeg12enc.c.o mpeg12data.c.o mpegvideo_enc.c.o motion_est.c.o ratecontrol.c.o
OBJS-$(CONFIG_MPEG4_DECODER)           += h263dec.c.o h263.c.o
OBJS-$(CONFIG_MPEG4_ENCODER)           += mpegvideo_enc.c.o motion_est.c.o ratecontrol.c.o h263.c.o
OBJS-$(CONFIG_MSMPEG4V1_DECODER)       += msmpeg4.c.o msmpeg4data.c.o h263dec.c.o h263.c.o
OBJS-$(CONFIG_MSMPEG4V1_ENCODER)       += msmpeg4.c.o msmpeg4data.c.o mpegvideo_enc.c.o motion_est.c.o ratecontrol.c.o h263.c.o
OBJS-$(CONFIG_MSMPEG4V2_DECODER)       += msmpeg4.c.o msmpeg4data.c.o h263dec.c.o h263.c.o
OBJS-$(CONFIG_MSMPEG4V2_ENCODER)       += msmpeg4.c.o msmpeg4data.c.o mpegvideo_enc.c.o motion_est.c.o ratecontrol.c.o h263.c.o
OBJS-$(CONFIG_MSMPEG4V3_DECODER)       += msmpeg4.c.o msmpeg4data.c.o h263dec.c.o h263.c.o
OBJS-$(CONFIG_MSMPEG4V3_ENCODER)       += msmpeg4.c.o msmpeg4data.c.o mpegvideo_enc.c.o motion_est.c.o ratecontrol.c.o h263.c.o
OBJS-$(CONFIG_MSRLE_DECODER)           += msrle.c.o
OBJS-$(CONFIG_MSVIDEO1_DECODER)        += msvideo1.c.o
OBJS-$(CONFIG_MSZH_DECODER)            += lcldec.c.o
OBJS-$(CONFIG_NUV_DECODER)             += nuv.c.o rtjpeg.c.o
OBJS-$(CONFIG_PAM_ENCODER)             += pnmenc.c.o pnm.c.o
OBJS-$(CONFIG_PBM_ENCODER)             += pnmenc.c.o pnm.c.o
OBJS-$(CONFIG_PGM_ENCODER)             += pnmenc.c.o pnm.c.o
OBJS-$(CONFIG_PGMYUV_ENCODER)          += pnmenc.c.o pnm.c.o
OBJS-$(CONFIG_PNG_DECODER)             += png.c.o pngdec.c.o
OBJS-$(CONFIG_PNG_ENCODER)             += png.c.o pngenc.c.o
OBJS-$(CONFIG_PPM_ENCODER)             += pnmenc.c.o pnm.c.o
OBJS-$(CONFIG_PTX_DECODER)             += ptx.c.o
OBJS-$(CONFIG_QDM2_DECODER)            += qdm2.c.o mdct.c.o fft.c.o mpegaudiodec.c.o mpegaudiodecheader.c.o mpegaudio.c.o mpegaudiodata.c.o
OBJS-$(CONFIG_QDRAW_DECODER)           += qdrw.c.o
OBJS-$(CONFIG_QPEG_DECODER)            += qpeg.c.o
OBJS-$(CONFIG_QTRLE_DECODER)           += qtrle.c.o
OBJS-$(CONFIG_QTRLE_ENCODER)           += qtrleenc.c.o
OBJS-$(CONFIG_RA_144_DECODER)          += ra144.c.o
OBJS-$(CONFIG_RA_288_DECODER)          += ra288.c.o
OBJS-$(CONFIG_RAWVIDEO_DECODER)        += rawdec.c.o
OBJS-$(CONFIG_RAWVIDEO_ENCODER)        += rawenc.c.o
OBJS-$(CONFIG_ROQ_DECODER)             += roqvideodec.c.o roqvideo.c.o
OBJS-$(CONFIG_ROQ_ENCODER)             += roqvideoenc.c.o roqvideo.c.o elbg.c.o
OBJS-$(CONFIG_ROQ_DPCM_DECODER)        += dpcm.c.o
OBJS-$(CONFIG_ROQ_DPCM_ENCODER)        += roqaudioenc.c.o
OBJS-$(CONFIG_RPZA_DECODER)            += rpza.c.o
OBJS-$(CONFIG_RV10_DECODER)            += rv10.c.o h263.c.o
OBJS-$(CONFIG_RV10_ENCODER)            += rv10.c.o mpegvideo_enc.c.o motion_est.c.o ratecontrol.c.o h263.c.o
OBJS-$(CONFIG_RV20_DECODER)            += rv10.c.o h263.c.o
OBJS-$(CONFIG_RV20_ENCODER)            += rv10.c.o mpegvideo_enc.c.o motion_est.c.o ratecontrol.c.o h263.c.o
OBJS-$(CONFIG_SGI_DECODER)             += sgidec.c.o
OBJS-$(CONFIG_SGI_ENCODER)             += sgienc.c.o rle.c.o
OBJS-$(CONFIG_SHORTEN_DECODER)         += shorten.c.o golomb.c.o
OBJS-$(CONFIG_SMACKAUD_DECODER)        += smacker.c.o
OBJS-$(CONFIG_SMACKER_DECODER)         += smacker.c.o
OBJS-$(CONFIG_SMC_DECODER)             += smc.c.o
OBJS-$(CONFIG_SNOW_DECODER)            += snow.c.o rangecoder.c.o
OBJS-$(CONFIG_SNOW_ENCODER)            += snow.c.o rangecoder.c.o motion_est.c.o ratecontrol.c.o h263.c.o
OBJS-$(CONFIG_SOL_DPCM_DECODER)        += dpcm.c.o
OBJS-$(CONFIG_SONIC_DECODER)           += sonic.c.o golomb.c.o
OBJS-$(CONFIG_SONIC_ENCODER)           += sonic.c.o golomb.c.o
OBJS-$(CONFIG_SONIC_LS_ENCODER)        += sonic.c.o golomb.c.o
OBJS-$(CONFIG_SP5X_DECODER)            += sp5xdec.c.o mjpegdec.c.o mjpeg.c.o
OBJS-$(CONFIG_SVQ1_DECODER)            += svq1dec.c.o svq1.c.o h263.c.o
OBJS-$(CONFIG_SVQ1_ENCODER)            += svq1enc.c.o svq1.c.o motion_est.c.o h263.c.o
OBJS-$(CONFIG_SVQ3_DECODER)            += h264.c.o h264idct.c.o h264pred.c.o cabac.c.o golomb.c.o
OBJS-$(CONFIG_TARGA_DECODER)           += targa.c.o
OBJS-$(CONFIG_TARGA_ENCODER)           += targaenc.c.o rle.c.o
OBJS-$(CONFIG_THEORA_DECODER)          += vp3.c.o xiph.c.o vp3dsp.c.o
OBJS-$(CONFIG_THP_DECODER)             += mjpegdec.c.o mjpeg.c.o
OBJS-$(CONFIG_TIERTEXSEQVIDEO_DECODER) += tiertexseqv.c.o
OBJS-$(CONFIG_TIFF_DECODER)            += tiff.c.o lzw.c.o
OBJS-$(CONFIG_TIFF_ENCODER)            += tiffenc.c.o rle.c.o lzwenc.c.o
OBJS-$(CONFIG_TRUEMOTION1_DECODER)     += truemotion1.c.o
OBJS-$(CONFIG_TRUEMOTION2_DECODER)     += truemotion2.c.o
OBJS-$(CONFIG_TRUESPEECH_DECODER)      += truespeech.c.o
OBJS-$(CONFIG_TSCC_DECODER)            += tscc.c.o
OBJS-$(CONFIG_TTA_DECODER)             += tta.c.o
OBJS-$(CONFIG_TXD_DECODER)             += txd.c.o s3tc.c.o
OBJS-$(CONFIG_ULTI_DECODER)            += ulti.c.o
OBJS-$(CONFIG_VC1_DECODER)             += vc1.c.o vc1data.c.o vc1dsp.c.o msmpeg4data.c.o
OBJS-$(CONFIG_VCR1_DECODER)            += vcr1.c.o
OBJS-$(CONFIG_VCR1_ENCODER)            += vcr1.c.o
OBJS-$(CONFIG_VMDAUDIO_DECODER)        += vmdav.c.o
OBJS-$(CONFIG_VMDVIDEO_DECODER)        += vmdav.c.o
OBJS-$(CONFIG_VMNC_DECODER)            += vmnc.c.o
OBJS-$(CONFIG_VORBIS_DECODER)          += vorbis_dec.c.o vorbis.c.o vorbis_data.c.o xiph.c.o mdct.c.o fft.c.o
OBJS-$(CONFIG_VORBIS_ENCODER)          += vorbis_enc.c.o vorbis.c.o vorbis_data.c.o mdct.c.o fft.c.o
OBJS-$(CONFIG_VP3_DECODER)             += vp3.c.o vp3dsp.c.o
OBJS-$(CONFIG_VP5_DECODER)             += vp5.c.o vp56.c.o vp56data.c.o vp3dsp.c.o
OBJS-$(CONFIG_VP6_DECODER)             += vp6.c.o vp56.c.o vp56data.c.o vp3dsp.c.o
OBJS-$(CONFIG_VP6F_DECODER)            += vp6.c.o vp56.c.o vp56data.c.o vp3dsp.c.o
OBJS-$(CONFIG_VQA_DECODER)             += vqavideo.c.o
OBJS-$(CONFIG_WAVPACK_DECODER)         += wavpack.c.o
OBJS-$(CONFIG_WMAV1_DECODER)           += wmadec.c.o wma.c.o mdct.c.o fft.c.o
OBJS-$(CONFIG_WMAV2_DECODER)           += wmadec.c.o wma.c.o mdct.c.o fft.c.o
OBJS-$(CONFIG_WMAV1_ENCODER)           += wmaenc.c.o wma.c.o mdct.c.o fft.c.o
OBJS-$(CONFIG_WMAV2_ENCODER)           += wmaenc.c.o wma.c.o mdct.c.o fft.c.o
OBJS-$(CONFIG_WMV1_DECODER)            += h263dec.c.o h263.c.o
OBJS-$(CONFIG_WMV1_ENCODER)            += mpegvideo_enc.c.o motion_est.c.o ratecontrol.c.o h263.c.o
OBJS-$(CONFIG_WMV2_DECODER)            += msmpeg4.c.o msmpeg4data.c.o h263dec.c.o h263.c.o
OBJS-$(CONFIG_WMV2_ENCODER)            += msmpeg4.c.o msmpeg4data.c.o mpegvideo_enc.c.o motion_est.c.o ratecontrol.c.o h263.c.o
OBJS-$(CONFIG_WMV3_DECODER)            += vc1.c.o vc1data.c.o vc1dsp.c.o
OBJS-$(CONFIG_WNV1_DECODER)            += wnv1.c.o
OBJS-$(CONFIG_WS_SND1_DECODER)         += ws-snd1.c.o
OBJS-$(CONFIG_XAN_DPCM_DECODER)        += dpcm.c.o
OBJS-$(CONFIG_XAN_WC3_DECODER)         += xan.c.o
OBJS-$(CONFIG_XAN_WC4_DECODER)         += xan.c.o
OBJS-$(CONFIG_XL_DECODER)              += xl.c.o
OBJS-$(CONFIG_XSUB_DECODER)            += xsubdec.c.o
OBJS-$(CONFIG_ZLIB_DECODER)            += lcldec.c.o
OBJS-$(CONFIG_ZLIB_ENCODER)            += lclenc.c.o
OBJS-$(CONFIG_ZMBV_DECODER)            += zmbv.c.o
OBJS-$(CONFIG_ZMBV_ENCODER)            += zmbvenc.c.o

OBJS-$(CONFIG_PCM_S32LE_DECODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_S32LE_ENCODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_S32BE_DECODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_S32BE_ENCODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_U32LE_DECODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_U32LE_ENCODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_U32BE_DECODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_U32BE_ENCODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_S24LE_DECODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_S24LE_ENCODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_S24BE_DECODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_S24BE_ENCODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_U24LE_DECODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_U24LE_ENCODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_U24BE_DECODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_U24BE_ENCODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_S24DAUD_DECODER)     += pcm.c.o
OBJS-$(CONFIG_PCM_S24DAUD_ENCODER)     += pcm.c.o
OBJS-$(CONFIG_PCM_S16LE_DECODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_S16LE_ENCODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_S16BE_DECODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_S16BE_ENCODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_U16LE_DECODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_U16LE_ENCODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_U16BE_DECODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_U16BE_ENCODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_S8_DECODER)          += pcm.c.o
OBJS-$(CONFIG_PCM_S8_ENCODER)          += pcm.c.o
OBJS-$(CONFIG_PCM_U8_DECODER)          += pcm.c.o
OBJS-$(CONFIG_PCM_U8_ENCODER)          += pcm.c.o
OBJS-$(CONFIG_PCM_ALAW_DECODER)        += pcm.c.o
OBJS-$(CONFIG_PCM_ALAW_ENCODER)        += pcm.c.o
OBJS-$(CONFIG_PCM_MULAW_DECODER)       += pcm.c.o
OBJS-$(CONFIG_PCM_MULAW_ENCODER)       += pcm.c.o

OBJS-$(CONFIG_ADPCM_4XM_DECODER)       += adpcm.c.o
OBJS-$(CONFIG_ADPCM_4XM_ENCODER)       += adpcm.c.o
OBJS-$(CONFIG_ADPCM_ADX_DECODER)       += adx.c.o
OBJS-$(CONFIG_ADPCM_ADX_ENCODER)       += adx.c.o
OBJS-$(CONFIG_ADPCM_CT_DECODER)        += adpcm.c.o
OBJS-$(CONFIG_ADPCM_CT_ENCODER)        += adpcm.c.o
OBJS-$(CONFIG_ADPCM_EA_DECODER)        += adpcm.c.o
OBJS-$(CONFIG_ADPCM_EA_ENCODER)        += adpcm.c.o
OBJS-$(CONFIG_ADPCM_G726_DECODER)      += g726.c.o
OBJS-$(CONFIG_ADPCM_G726_ENCODER)      += g726.c.o
OBJS-$(CONFIG_ADPCM_IMA_AMV_DECODER)   += adpcm.c.o
OBJS-$(CONFIG_ADPCM_IMA_DK3_DECODER)   += adpcm.c.o
OBJS-$(CONFIG_ADPCM_IMA_DK3_ENCODER)   += adpcm.c.o
OBJS-$(CONFIG_ADPCM_IMA_DK4_DECODER)   += adpcm.c.o
OBJS-$(CONFIG_ADPCM_IMA_DK4_ENCODER)   += adpcm.c.o
OBJS-$(CONFIG_ADPCM_IMA_QT_DECODER)    += adpcm.c.o
OBJS-$(CONFIG_ADPCM_IMA_QT_ENCODER)    += adpcm.c.o
OBJS-$(CONFIG_ADPCM_IMA_SMJPEG_DECODER) += adpcm.c.o
OBJS-$(CONFIG_ADPCM_IMA_SMJPEG_ENCODER) += adpcm.c.o
OBJS-$(CONFIG_ADPCM_IMA_WAV_DECODER)   += adpcm.c.o
OBJS-$(CONFIG_ADPCM_IMA_WAV_ENCODER)   += adpcm.c.o
OBJS-$(CONFIG_ADPCM_IMA_WS_DECODER)    += adpcm.c.o
OBJS-$(CONFIG_ADPCM_IMA_WS_ENCODER)    += adpcm.c.o
OBJS-$(CONFIG_ADPCM_MS_DECODER)        += adpcm.c.o
OBJS-$(CONFIG_ADPCM_MS_ENCODER)        += adpcm.c.o
OBJS-$(CONFIG_ADPCM_SBPRO_2_DECODER)   += adpcm.c.o
OBJS-$(CONFIG_ADPCM_SBPRO_2_ENCODER)   += adpcm.c.o
OBJS-$(CONFIG_ADPCM_SBPRO_3_DECODER)   += adpcm.c.o
OBJS-$(CONFIG_ADPCM_SBPRO_3_ENCODER)   += adpcm.c.o
OBJS-$(CONFIG_ADPCM_SBPRO_4_DECODER)   += adpcm.c.o
OBJS-$(CONFIG_ADPCM_SBPRO_4_ENCODER)   += adpcm.c.o
OBJS-$(CONFIG_ADPCM_SWF_DECODER)       += adpcm.c.o
OBJS-$(CONFIG_ADPCM_SWF_ENCODER)       += adpcm.c.o
OBJS-$(CONFIG_ADPCM_THP_DECODER)       += adpcm.c.o
OBJS-$(CONFIG_ADPCM_XA_DECODER)        += adpcm.c.o
OBJS-$(CONFIG_ADPCM_XA_ENCODER)        += adpcm.c.o
OBJS-$(CONFIG_ADPCM_YAMAHA_DECODER)    += adpcm.c.o
OBJS-$(CONFIG_ADPCM_YAMAHA_ENCODER)    += adpcm.c.o

# external codec libraries
OBJS-$(CONFIG_LIBA52)                  += liba52.c.o
OBJS-$(CONFIG_LIBAMR)                  += libamr.c.o
OBJS-$(CONFIG_LIBFAAC)                 += libfaac.c.o
OBJS-$(CONFIG_LIBFAAD)                 += libfaad.c.o
OBJS-$(CONFIG_LIBGSM)                  += libgsm.c.o
OBJS-$(CONFIG_LIBMP3LAME)              += libmp3lame.c.o
OBJS-$(CONFIG_LIBTHEORA)               += libtheoraenc.c.o
OBJS-$(CONFIG_LIBVORBIS)               += libvorbis.c.o
OBJS-$(CONFIG_LIBX264)                 += libx264.c.o
OBJS-$(CONFIG_LIBXVID)                 += libxvidff.c.o libxvid_rc.c.o


OBJS-$(CONFIG_AAC_PARSER)              += aac_parser.c.o aac_ac3_parser.c.o
OBJS-$(CONFIG_AC3_PARSER)              += ac3_parser.c.o ac3tab.c.o aac_ac3_parser.c.o
OBJS-$(CONFIG_CAVSVIDEO_PARSER)        += cavs_parser.c.o
OBJS-$(CONFIG_DCA_PARSER)              += dca_parser.c.o
OBJS-$(CONFIG_DVBSUB_PARSER)           += dvbsub_parser.c.o
OBJS-$(CONFIG_DVDSUB_PARSER)           += dvdsub_parser.c.o
OBJS-$(CONFIG_H261_PARSER)             += h261_parser.c.o
OBJS-$(CONFIG_H263_PARSER)             += h263_parser.c.o
OBJS-$(CONFIG_H264_PARSER)             += h264_parser.c.o
OBJS-$(CONFIG_MJPEG_PARSER)            += mjpeg_parser.c.o
OBJS-$(CONFIG_MPEG4VIDEO_PARSER)       += mpeg4video_parser.c.o h263.c.o
OBJS-$(CONFIG_MPEGAUDIO_PARSER)        += mpegaudio_parser.c.o mpegaudiodecheader.c.o mpegaudiodata.c.o
OBJS-$(CONFIG_MPEGVIDEO_PARSER)        += mpegvideo_parser.c.o
OBJS-$(CONFIG_PNM_PARSER)              += pnm_parser.c.o pnm.c.o
OBJS-$(CONFIG_VC1_PARSER)              += vc1_parser.c.o

OBJS-$(CONFIG_DUMP_EXTRADATA_BSF)      += dump_extradata_bsf.c.o
OBJS-$(CONFIG_H264_MP4TOANNEXB_BSF)    += h264_mp4toannexb_bsf.c.o
OBJS-$(CONFIG_IMX_DUMP_HEADER_BSF)     += imx_dump_header_bsf.c.o
OBJS-$(CONFIG_MJPEGA_DUMP_HEADER_BSF)  += mjpega_dump_header_bsf.c.o
OBJS-$(CONFIG_MP3_HEADER_COMPRESS_BSF) += mp3_header_compress_bsf.c.o
OBJS-$(CONFIG_MP3_HEADER_DECOMPRESS_BSF) += mp3_header_decompress_bsf.c.o mpegaudiodata.c.o
OBJS-$(CONFIG_NOISE_BSF)               += noise_bsf.c.o
OBJS-$(CONFIG_REMOVE_EXTRADATA_BSF)    += remove_extradata_bsf.c.o

OBJS-$(HAVE_BEOSTHREADS)               += beosthread.c.o
OBJS-$(HAVE_OS2THREADS)                += os2thread.c.o
OBJS-$(HAVE_PTHREADS)                  += pthread.c.o
OBJS-$(HAVE_W32THREADS)                += w32thread.c.o

OBJS-$(HAVE_XVMC_ACCEL)                += xvmcvideo.c.o

ifneq ($(CONFIG_SWSCALER),yes)
OBJS += imgresample.c.o
endif

# processor-specific code
ifeq ($(HAVE_MMX),yes)
OBJS += i386/fdct_mmx.c.o \
        i386/cputest.c.o \
        i386/dsputil_mmx.c.o \
        i386/mpegvideo_mmx.c.o \
        i386/motion_est_mmx.c.o \
        i386/simple_idct_mmx.c.o \
        i386/idct_mmx_xvid.c.o \
        i386/fft_sse.c.o \
        i386/fft_3dn.c.o \
        i386/fft_3dn2.c.o \

OBJS-$(CONFIG_GPL)                     += i386/idct_mmx.c.o

OBJS-$(CONFIG_CAVS_DECODER)            += i386/cavsdsp_mmx.c.o
OBJS-$(CONFIG_SNOW_DECODER)            += i386/snowdsp_mmx.c.o
OBJS-$(CONFIG_VP3_DECODER)             += i386/vp3dsp_mmx.c.o i386/vp3dsp_sse2.c.o
OBJS-$(CONFIG_VP5_DECODER)             += i386/vp3dsp_mmx.c.o i386/vp3dsp_sse2.c.o
OBJS-$(CONFIG_VP6_DECODER)             += i386/vp3dsp_mmx.c.o i386/vp3dsp_sse2.c.o
OBJS-$(CONFIG_VP6F_DECODER)            += i386/vp3dsp_mmx.c.o i386/vp3dsp_sse2.c.o
endif

ASM_OBJS-$(ARCH_ARMV4L)                += armv4l/jrevdct_arm.s.o     \
                                          armv4l/simple_idct_arm.s.o \
                                          armv4l/dsputil_arm_s.s.o   \

OBJS-$(ARCH_ARMV4L)                    += armv4l/dsputil_arm.c.o   \
                                          armv4l/mpegvideo_arm.c.o \

OBJS-$(HAVE_IWMMXT)                    += armv4l/dsputil_iwmmxt.c.o   \
                                          armv4l/mpegvideo_iwmmxt.c.o \

ASM_OBJS-$(HAVE_ARMV5TE)               += armv4l/simple_idct_armv5te.s.o \
                                          armv4l/mpegvideo_armv5te.s.o \

ASM_OBJS-$(HAVE_ARMV6)                 += armv4l/simple_idct_armv6.s.o \

OBJS-$(HAVE_VIS)                       += sparc/dsputil_vis.c.o \
                                          sparc/simple_idct_vis.c.o \

OBJS-$(HAVE_MLIB)                      += mlib/dsputil_mlib.c.o \

OBJS-$(ARCH_ALPHA)                     += alpha/dsputil_alpha.c.o     \
                                          alpha/motion_est_alpha.c.o  \
                                          alpha/mpegvideo_alpha.c.o   \
                                          alpha/simple_idct_alpha.c.o \

ASM_OBJS-$(ARCH_ALPHA)                 += alpha/dsputil_alpha_asm.s.o  \
                                          alpha/motion_est_mvi_asm.s.o \

OBJS-$(ARCH_POWERPC)                   += ppc/dsputil_ppc.c.o   \

OBJS-$(HAVE_MMI)                       += ps2/dsputil_mmi.c.o   \
                                          ps2/idct_mmi.c.o      \
                                          ps2/mpegvideo_mmi.c.o \

OBJS-$(ARCH_SH4)                       += sh4/idct_sh4.c.o      \
                                          sh4/dsputil_align.c.o \
                                          sh4/dsputil_sh4.c.o   \

ALTIVEC-OBJS-yes                       += ppc/dsputil_altivec.c.o      \
                                          ppc/fdct_altivec.c.o         \
                                          ppc/fft_altivec.c.o          \
                                          ppc/float_altivec.c.o        \
                                          ppc/gmc_altivec.c.o          \
                                          ppc/idct_altivec.c.o         \
                                          ppc/int_altivec.c.o          \
                                          ppc/mpegvideo_altivec.c.o    \

ALTIVEC-OBJS-$(CONFIG_H264_DECODER)    += ppc/h264_altivec.c.o
ALTIVEC-OBJS-$(CONFIG_SNOW_DECODER)    += ppc/snow_altivec.c.o
ALTIVEC-OBJS-$(CONFIG_VC1_DECODER)     += ppc/vc1dsp_altivec.c.o
ALTIVEC-OBJS-$(CONFIG_WMV3_DECODER)    += ppc/vc1dsp_altivec.c.o

# -maltivec is needed in order to build AltiVec code.
$(ALTIVEC-OBJS-yes): CFLAGS += -maltivec -mabi=altivec

# check_altivec must be built without -maltivec
OBJS-$(HAVE_ALTIVEC)                   += $(ALTIVEC-OBJS-yes)       \
                                          ppc/check_altivec.c.o

OBJS-$(ARCH_BFIN)                      += bfin/dsputil_bfin.c.o \
                                          bfin/mpegvideo_bfin.c.o \
                                          bfin/vp3_bfin.c.o \

ASM_OBJS-$(ARCH_BFIN)                  += bfin/pixels_bfin.s.o \
                                          bfin/fdct_bfin.s.o   \
                                          bfin/idct_bfin.s.o   \
                                          bfin/vp3_idct_bfin.s.o   \

#

CFLAGS += --std=gnu99
INCS += ../libswscale ../libavutil
DEFS += HAVE_AV_CONFIG_H

.PHONY: all clean avcodec

all: avcodec

clean: generic_clean

#########################################################################
# Standard rules.

include $(SUB_DEPTH)/rules.generic.mk

#########################################################################
# Main rules.

avcodec: avcodec.$(.LIB)

avcodec.$(.LIB): $(OBJS) $(OBJS-y) $(OBJS-y) $(ALTIVEC-OBJS-y) $(ASM_OBJS-y)
