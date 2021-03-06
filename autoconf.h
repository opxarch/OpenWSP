#ifndef autoconf_h_
#define autoconf_h_

#define WEBAPIS_PATH "./apis"
#define WEBMOULE_DEFAULT "QingTing-FM"

#define HAVE_WINSOCK2 1
//#define HAVE_SYS_MMAN_H 1

#define HAVE_BIGENDIAN 0

/* set up max. outburst. use 65536 for ALSA 0.5, for others 16384 is enough */
#define MAX_OUTBURST 65536

//#define USES_SDL_AUDIO 1
#define USES_DSOUND_AUDIO 1

#define USES_FFMPEG_CODEC 1
#define USES_PCM_CODEC 1

#define COMPILER_SUP_INLINE_ASM_GNU_STYLE 1

#define ENABLE_ASSERTIONS 1

#define HAVE_PTHREADS 1


/*
 for libavutil
 */

/* assembler handling of .align */
#define ASMALIGN(ZEROBITS) ".align " #ZEROBITS "\n\t"

/* Define this if your system has pthreads */
//#define HAVE_PTHREADS 1

/* Define this if you enabled thread support for libavcodec */
#define HAVE_THREADS 1
#ifdef HAVE_THREADS
#define ENABLE_THREADS 1
#else
#define ENABLE_THREADS 0
#endif

/* Define if your processor can access unaligned data in a fast way */

#define ENABLE_X86 1
#define ENABLE_X86_32 0
#define ENABLE_X86_64 0
#define ENABLE_IA64 0
#define ENABLE_SPARC 0
#define ENABLE_ARM 0
#define ENABLE_ARMV4L 0
#define ENABLE_SH3 0
#define ENABLE_POWERPC 0
#define ENABLE_PPC 0
#define ENABLE_ALPHA 0
//#define ARCH_SGI_MIPS 1
#define ARCH_X86 1
#define ENABLE_SGI_MIPS 0
#define ENABLE_PA_RISC 0
#define ENABLE_S390 0
#define ENABLE_S390X 0
#define ENABLE_VAX 0
#define ENABLE_BFIN 0
#define ENABLE_GENERIC 0

/* Extension defines */
#define ENABLE_ALTIVEC 0
#define ENABLE_MMX 0
#define ENABLE_MMX2 0
#define ENABLE_3DNOW 0
#define ENABLE_3DNOWEX 0
#define ENABLE_SSE 0
#define ENABLE_SSE2 0
#define ENABLE_SSSE3 0
#define ENABLE_FAST_CMOV 0
#define ENABLE_CMOV 0
#define ENABLE_ARMV5TE 0
#define ENABLE_ARMV6 0
#define ENABLE_IWMMXT 0
#define ENABLE_MLIB 0
#define ENABLE_MMI 0
#define ENABLE_SH4 0
#define ENABLE_VIS 0
#define ENABLE_MVI 0


#define CONFIG_GPL 1
#define ENABLE_SMALL 0

/* Support for grayscale encoding/decoding in FFmpeg (makes color slower) */
#define ENABLE_GRAY 1
//#undef EBABLE_GRAY

/* Use AMR codecs from libavcodec. */

#undef CONFIG_LIBAMR_NB
#undef CONFIG_LIBAMR_WB

/* Use specific parts from FFmpeg. */
//#define CONFIG_AASC_DECODER 1
#define ENABLE_AASC_DECODER 0
//#define CONFIG_AMV_DECODER 1
#define ENABLE_AMV_DECODER 0
//#define CONFIG_ASV1_DECODER 1
#define ENABLE_ASV1_DECODER 0
//#define CONFIG_ASV2_DECODER 1
#define ENABLE_ASV2_DECODER 0
//#define CONFIG_AVS_DECODER 1
#define ENABLE_AVS_DECODER 0
//#define CONFIG_BETHSOFTVID_DECODER 1
#define ENABLE_BETHSOFTVID_DECODER 0
//#define CONFIG_BMP_DECODER 1
#define ENABLE_BMP_DECODER 0
//#define CONFIG_C93_DECODER 1
#define ENABLE_C93_DECODER 0
//#define CONFIG_CAVS_DECODER 1
#define ENABLE_CAVS_DECODER 0
//#define CONFIG_CINEPAK_DECODER 1
#define ENABLE_CINEPAK_DECODER 0
//#define CONFIG_CLJR_DECODER 1
#define ENABLE_CLJR_DECODER 0
//#define CONFIG_CSCD_DECODER 1
#define ENABLE_CSCD_DECODER 0
//#define CONFIG_CYUV_DECODER 1
#define ENABLE_CYUV_DECODER 0
//#define CONFIG_DNXHD_DECODER 1
#define ENABLE_DNXHD_DECODER 0
//#define CONFIG_DSICINVIDEO_DECODER 1
#define ENABLE_DSICINVIDEO_DECODER 0
//#define CONFIG_DVVIDEO_DECODER 1
#define ENABLE_DVVIDEO_DECODER 0
//#define CONFIG_DXA_DECODER 1
#define ENABLE_DXA_DECODER 0
//#define CONFIG_EIGHTBPS_DECODER 1
#define ENABLE_EIGHTBPS_DECODER 0
//#define CONFIG_FFV1_DECODER 1
#define ENABLE_FFV1_DECODER 0
//#define CONFIG_FFVHUFF_DECODER 1
#define ENABLE_FFVHUFF_DECODER 0
//#define CONFIG_FLASHSV_DECODER 1
#define ENABLE_FLASHSV_DECODER 0
//#define CONFIG_FLIC_DECODER 1
#define ENABLE_FLIC_DECODER 0
//#define CONFIG_FLV_DECODER 1
#define ENABLE_FLV_DECODER 0
//#define CONFIG_FOURXM_DECODER 1
#define ENABLE_FOURXM_DECODER 0
//#define CONFIG_FRAPS_DECODER 1
#define ENABLE_FRAPS_DECODER 0
//#define CONFIG_GIF_DECODER 1
#define ENABLE_GIF_DECODER 0
//#define CONFIG_H261_DECODER 1
#define ENABLE_H261_DECODER 0
//#define CONFIG_H263_DECODER 1
#define ENABLE_H263_DECODER 0
//#define CONFIG_H263I_DECODER 1
#define ENABLE_H263I_DECODER 0
//#define CONFIG_H264_DECODER 1
#define ENABLE_H264_DECODER 0
//#define CONFIG_HUFFYUV_DECODER 1
#define ENABLE_HUFFYUV_DECODER 0
//#define CONFIG_IDCIN_DECODER 1
#define ENABLE_IDCIN_DECODER 0
//#define CONFIG_INDEO2_DECODER 1
#define ENABLE_INDEO2_DECODER 0
//#define CONFIG_INDEO3_DECODER 1
#define ENABLE_INDEO3_DECODER 0
//#define CONFIG_INTERPLAY_VIDEO_DECODER 1
#define ENABLE_INTERPLAY_VIDEO_DECODER 0
//#define CONFIG_JPEGLS_DECODER 1
#define ENABLE_JPEGLS_DECODER 0
//#define CONFIG_KMVC_DECODER 1
#define ENABLE_KMVC_DECODER 0
//#define CONFIG_LOCO_DECODER 1
#define ENABLE_LOCO_DECODER 0
//#define CONFIG_MDEC_DECODER 1
#define ENABLE_MDEC_DECODER 0
//#define CONFIG_MJPEG_DECODER 1
#define ENABLE_MJPEG_DECODER 0
//#define CONFIG_MJPEGB_DECODER 1
#define ENABLE_MJPEGB_DECODER 0
//#define CONFIG_MMVIDEO_DECODER 1
#define ENABLE_MMVIDEO_DECODER 0
#define ENABLE_MPEG_XVMC_DECODER 0
//#define CONFIG_MPEG1VIDEO_DECODER 1
#define ENABLE_MPEG1VIDEO_DECODER 0
//#define CONFIG_MPEG2VIDEO_DECODER 1
#define ENABLE_MPEG2VIDEO_DECODER 0
//#define CONFIG_MPEG4_DECODER 1
#define ENABLE_MPEG4_DECODER 0
//#define CONFIG_MPEGVIDEO_DECODER 1
#define ENABLE_MPEGVIDEO_DECODER 0
//#define CONFIG_MSMPEG4V1_DECODER 1
#define ENABLE_MSMPEG4V1_DECODER 0
//#define CONFIG_MSMPEG4V2_DECODER 1
#define ENABLE_MSMPEG4V2_DECODER 0
//#define CONFIG_MSMPEG4V3_DECODER 1
#define ENABLE_MSMPEG4V3_DECODER 0
//#define CONFIG_MSRLE_DECODER 1
#define ENABLE_MSRLE_DECODER 0
//#define CONFIG_MSVIDEO1_DECODER 1
#define ENABLE_MSVIDEO1_DECODER 0
//#define CONFIG_MSZH_DECODER 1
#define ENABLE_MSZH_DECODER 0
//#define CONFIG_NUV_DECODER 1
#define ENABLE_NUV_DECODER 0
//#define CONFIG_PNG_DECODER 1
#define ENABLE_PNG_DECODER 0
//#define CONFIG_PTX_DECODER 1
#define ENABLE_PTX_DECODER 0
//#define CONFIG_QDRAW_DECODER 1
#define ENABLE_QDRAW_DECODER 0
//#define CONFIG_QPEG_DECODER 1
#define ENABLE_QPEG_DECODER 0
//#define CONFIG_QTRLE_DECODER 1
#define ENABLE_QTRLE_DECODER 0
//#define CONFIG_RAWVIDEO_DECODER 1
#define ENABLE_RAWVIDEO_DECODER 0
//#define CONFIG_ROQ_DECODER 1
#define ENABLE_ROQ_DECODER 0
//#define CONFIG_RPZA_DECODER 1
#define ENABLE_RPZA_DECODER 0
//#define CONFIG_RV10_DECODER 1
#define ENABLE_RV10_DECODER 0
//#define CONFIG_RV20_DECODER 1
#define ENABLE_RV20_DECODER 0
//#define CONFIG_SGI_DECODER 1
#define ENABLE_SGI_DECODER 0
//#define CONFIG_SMACKER_DECODER 1
#define ENABLE_SMACKER_DECODER 0
//#define CONFIG_SMC_DECODER 1
#define ENABLE_SMC_DECODER 0
//#define CONFIG_SNOW_DECODER 1
#define ENABLE_SNOW_DECODER 0
//#define CONFIG_SP5X_DECODER 1
#define ENABLE_SP5X_DECODER 0
//#define CONFIG_SVQ1_DECODER 1
#define ENABLE_SVQ1_DECODER 0
//#define CONFIG_SVQ3_DECODER 1
#define ENABLE_SVQ3_DECODER 0
//#define CONFIG_TARGA_DECODER 1
#define ENABLE_TARGA_DECODER 0
//#define CONFIG_THEORA_DECODER 1
#define ENABLE_THEORA_DECODER 0
//#define CONFIG_THP_DECODER 1
#define ENABLE_THP_DECODER 0
//#define CONFIG_TIERTEXSEQVIDEO_DECODER 1
#define ENABLE_TIERTEXSEQVIDEO_DECODER 0
//#define CONFIG_TIFF_DECODER 1
#define ENABLE_TIFF_DECODER 0
//#define CONFIG_TRUEMOTION1_DECODER 1
#define ENABLE_TRUEMOTION1_DECODER 0
//#define CONFIG_TRUEMOTION2_DECODER 1
#define ENABLE_TRUEMOTION2_DECODER 0
//#define CONFIG_TSCC_DECODER 1
#define ENABLE_TSCC_DECODER 0
//#define CONFIG_TXD_DECODER 1
#define ENABLE_TXD_DECODER 0
//#define CONFIG_ULTI_DECODER 1
#define ENABLE_ULTI_DECODER 0
//#define CONFIG_VC1_DECODER 1
#define ENABLE_VC1_DECODER 0
//#define CONFIG_VCR1_DECODER 1
#define ENABLE_VCR1_DECODER 0
//#define CONFIG_VMDVIDEO_DECODER 1
#define ENABLE_VMDVIDEO_DECODER 0
//#define CONFIG_VMNC_DECODER 1
#define ENABLE_VMNC_DECODER 0
//#define CONFIG_VP3_DECODER 1
#define ENABLE_VP3_DECODER 0
//#define CONFIG_VP5_DECODER 1
#define ENABLE_VP5_DECODER 0
//#define CONFIG_VP6_DECODER 1
#define ENABLE_VP6_DECODER 0
//#define CONFIG_VP6A_DECODER 1
#define ENABLE_VP6A_DECODER 0
//#define CONFIG_VP6F_DECODER 1
#define ENABLE_VP6F_DECODER 0
//#define CONFIG_VQA_DECODER 1
#define ENABLE_VQA_DECODER 0
//#define CONFIG_WMV1_DECODER 1
#define ENABLE_WMV1_DECODER 0
//#define CONFIG_WMV2_DECODER 1
#define ENABLE_WMV2_DECODER 0
//#define CONFIG_WMV3_DECODER 1
#define ENABLE_WMV3_DECODER 0
//#define CONFIG_WNV1_DECODER 1
#define ENABLE_WNV1_DECODER 0
//#define CONFIG_XAN_WC3_DECODER 1
#define ENABLE_XAN_WC3_DECODER 0
//#define CONFIG_XL_DECODER 1
#define ENABLE_XL_DECODER 0
//#define CONFIG_XSUB_DECODER 1
#define ENABLE_XSUB_DECODER 0
#define CONFIG_ZLIB_DECODER 1
#define ENABLE_ZLIB_DECODER 1
//#define CONFIG_ZMBV_DECODER 1
#define ENABLE_ZMBV_DECODER 0
#define ENABLE_MPEG4AAC_DECODER 0
//#define CONFIG_AC3_DECODER 1
#define ENABLE_AC3_DECODER 0
//#define CONFIG_ALAC_DECODER 1
#define ENABLE_ALAC_DECODER 0
//#define CONFIG_APE_DECODER 1
#define ENABLE_APE_DECODER 0
//#define CONFIG_ATRAC3_DECODER 1
#define ENABLE_ATRAC3_DECODER 0
//#define CONFIG_COOK_DECODER 1
#define ENABLE_COOK_DECODER 0
//#define CONFIG_DCA_DECODER 1
#define ENABLE_DCA_DECODER 0
//#define CONFIG_DSICINAUDIO_DECODER 1
#define ENABLE_DSICINAUDIO_DECODER 0
#define CONFIG_FLAC_DECODER 1
#define ENABLE_FLAC_DECODER 1
//#define CONFIG_IMC_DECODER 1
#define ENABLE_IMC_DECODER 0
#define ENABLE_LIBAMR_NB_DECODER 0
#define ENABLE_LIBAMR_WB_DECODER 0
#define ENABLE_LIBA52_DECODER 0
#define ENABLE_LIBFAAD_DECODER 0
#define ENABLE_LIBGSM_DECODER 0
#define ENABLE_LIBGSM_MS_DECODER 0
#define ENABLE_LIBVORBIS_DECODER 0
//#define CONFIG_MACE3_DECODER 1
#define ENABLE_MACE3_DECODER 0
//#define CONFIG_MACE6_DECODER 1
#define ENABLE_MACE6_DECODER 0
//#define CONFIG_MP2_DECODER 1
#define ENABLE_MP2_DECODER 0
#define CONFIG_MP3_DECODER 1
#define ENABLE_MP3_DECODER 1
//#define CONFIG_MP3ADU_DECODER 1
#define ENABLE_MP3ADU_DECODER 0
//#define CONFIG_MP3ON4_DECODER 1
#define ENABLE_MP3ON4_DECODER 0
//#define CONFIG_MPC7_DECODER 1
#define ENABLE_MPC7_DECODER 0
//#define CONFIG_QDM2_DECODER 1
#define ENABLE_QDM2_DECODER 0
//#define CONFIG_RA_144_DECODER 1
#define ENABLE_RA_144_DECODER 0
//#define CONFIG_RA_288_DECODER 1
#define ENABLE_RA_288_DECODER 0
//#define CONFIG_SHORTEN_DECODER 1
#define ENABLE_SHORTEN_DECODER 0
//#define CONFIG_SMACKAUD_DECODER 1
#define ENABLE_SMACKAUD_DECODER 0
//#define CONFIG_SONIC_DECODER 1
#define ENABLE_SONIC_DECODER 0
//#define CONFIG_TRUESPEECH_DECODER 1
#define ENABLE_TRUESPEECH_DECODER 0
//#define CONFIG_TTA_DECODER 1
#define ENABLE_TTA_DECODER 0
//#define CONFIG_VMDAUDIO_DECODER 1
#define ENABLE_VMDAUDIO_DECODER 0
//#define CONFIG_VORBIS_DECODER 1
#define ENABLE_VORBIS_DECODER 0
//#define CONFIG_WAVPACK_DECODER 1
#define ENABLE_WAVPACK_DECODER 0
//#define CONFIG_WMAV1_DECODER 1
#define ENABLE_WMAV1_DECODER 0
//#define CONFIG_WMAV2_DECODER 1
#define ENABLE_WMAV2_DECODER 0
//#define CONFIG_WS_SND1_DECODER 1
#define ENABLE_WS_SND1_DECODER 0
//#define CONFIG_PCM_ALAW_DECODER 1
#define ENABLE_PCM_ALAW_DECODER 0
//#define CONFIG_PCM_MULAW_DECODER 1
#define ENABLE_PCM_MULAW_DECODER 0
//#define CONFIG_PCM_S8_DECODER 1
#define ENABLE_PCM_S8_DECODER 0
//#define CONFIG_PCM_S16BE_DECODER 1
#define ENABLE_PCM_S16BE_DECODER 0
//#define CONFIG_PCM_S16LE_DECODER 1
#define ENABLE_PCM_S16LE_DECODER 0
//#define CONFIG_PCM_S24BE_DECODER 1
#define ENABLE_PCM_S24BE_DECODER 0
//#define CONFIG_PCM_S24DAUD_DECODER 1
#define ENABLE_PCM_S24DAUD_DECODER 0
//#define CONFIG_PCM_S24LE_DECODER 1
#define ENABLE_PCM_S24LE_DECODER 0
//#define CONFIG_PCM_S32BE_DECODER 1
#define ENABLE_PCM_S32BE_DECODER 0
//#define CONFIG_PCM_S32LE_DECODER 1
#define ENABLE_PCM_S32LE_DECODER 0
//#define CONFIG_PCM_U8_DECODER 1
#define ENABLE_PCM_U8_DECODER 0
//#define CONFIG_PCM_U16BE_DECODER 1
#define ENABLE_PCM_U16BE_DECODER 0
//#define CONFIG_PCM_U16LE_DECODER 1
#define ENABLE_PCM_U16LE_DECODER 0
//#define CONFIG_PCM_U24BE_DECODER 1
#define ENABLE_PCM_U24BE_DECODER 0
//#define CONFIG_PCM_U24LE_DECODER 1
#define ENABLE_PCM_U24LE_DECODER 0
//#define CONFIG_PCM_U32BE_DECODER 1
#define ENABLE_PCM_U32BE_DECODER 0
//#define CONFIG_PCM_U32LE_DECODER 1
#define ENABLE_PCM_U32LE_DECODER 0
//#define CONFIG_PCM_ZORK_DECODER 1
#define ENABLE_PCM_ZORK_DECODER 0
//#define CONFIG_INTERPLAY_DPCM_DECODER 1
#define ENABLE_INTERPLAY_DPCM_DECODER 0
//#define CONFIG_ROQ_DPCM_DECODER 1
#define ENABLE_ROQ_DPCM_DECODER 0
//#define CONFIG_SOL_DPCM_DECODER 1
#define ENABLE_SOL_DPCM_DECODER 0
//#define CONFIG_XAN_DPCM_DECODER 1
#define ENABLE_XAN_DPCM_DECODER 0
//#define CONFIG_ADPCM_4XM_DECODER 1
#define ENABLE_ADPCM_4XM_DECODER 0
//#define CONFIG_ADPCM_ADX_DECODER 1
#define ENABLE_ADPCM_ADX_DECODER 0
//#define CONFIG_ADPCM_CT_DECODER 1
#define ENABLE_ADPCM_CT_DECODER 0
//#define CONFIG_ADPCM_EA_DECODER 1
#define ENABLE_ADPCM_EA_DECODER 0
//#define CONFIG_ADPCM_G726_DECODER 1
#define ENABLE_ADPCM_G726_DECODER 0
//#define CONFIG_ADPCM_IMA_AMV_DECODER 1
#define ENABLE_ADPCM_IMA_AMV_DECODER 0
//#define CONFIG_ADPCM_IMA_DK3_DECODER 1
#define ENABLE_ADPCM_IMA_DK3_DECODER 0
//#define CONFIG_ADPCM_IMA_DK4_DECODER 1
#define ENABLE_ADPCM_IMA_DK4_DECODER 0
//#define CONFIG_ADPCM_IMA_QT_DECODER 1
#define ENABLE_ADPCM_IMA_QT_DECODER 0
//#define CONFIG_ADPCM_IMA_SMJPEG_DECODER 1
#define ENABLE_ADPCM_IMA_SMJPEG_DECODER 0
//#define CONFIG_ADPCM_IMA_WAV_DECODER 1
#define ENABLE_ADPCM_IMA_WAV_DECODER 0
//#define CONFIG_ADPCM_IMA_WS_DECODER 1
#define ENABLE_ADPCM_IMA_WS_DECODER 0
//#define CONFIG_ADPCM_MS_DECODER 1
#define ENABLE_ADPCM_MS_DECODER 0
//#define CONFIG_ADPCM_SBPRO_2_DECODER 1
#define ENABLE_ADPCM_SBPRO_2_DECODER 0
//#define CONFIG_ADPCM_SBPRO_3_DECODER 1
#define ENABLE_ADPCM_SBPRO_3_DECODER 0
//#define CONFIG_ADPCM_SBPRO_4_DECODER 1
#define ENABLE_ADPCM_SBPRO_4_DECODER 0
//#define CONFIG_ADPCM_SWF_DECODER 1
#define ENABLE_ADPCM_SWF_DECODER 0
//#define CONFIG_ADPCM_THP_DECODER 1
#define ENABLE_ADPCM_THP_DECODER 0
//#define CONFIG_ADPCM_XA_DECODER 1
#define ENABLE_ADPCM_XA_DECODER 0
//#define CONFIG_ADPCM_YAMAHA_DECODER 1
#define ENABLE_ADPCM_YAMAHA_DECODER 0
//#define CONFIG_DVBSUB_DECODER 1
#define ENABLE_DVBSUB_DECODER 0
//#define CONFIG_DVDSUB_DECODER 1
#define ENABLE_DVDSUB_DECODER 0
#define ENABLE_ASV1_ENCODER 0
#define ENABLE_ASV2_ENCODER 0
#define ENABLE_BMP_ENCODER 0
#define ENABLE_DVVIDEO_ENCODER 0
#define ENABLE_FFV1_ENCODER 0
#define ENABLE_FFVHUFF_ENCODER 0
#define ENABLE_FLASHSV_ENCODER 0
#define ENABLE_FLV_ENCODER 0
#define ENABLE_GIF_ENCODER 0
#define ENABLE_H261_ENCODER 0
#define ENABLE_H263_ENCODER 0
#define ENABLE_H263P_ENCODER 0
#define ENABLE_HUFFYUV_ENCODER 0
#define ENABLE_JPEGLS_ENCODER 0
#define ENABLE_LIBX264_ENCODER 0
#define ENABLE_LIBXVID_ENCODER 0
#define ENABLE_LJPEG_ENCODER 0
#define ENABLE_MJPEG_ENCODER 0
//#define CONFIG_MPEG1VIDEO_ENCODER 1
#define ENABLE_MPEG1VIDEO_ENCODER 0
#define ENABLE_MPEG2VIDEO_ENCODER 0
#define ENABLE_MPEG4_ENCODER 0
#define ENABLE_MSMPEG4V1_ENCODER 0
#define ENABLE_MSMPEG4V2_ENCODER 0
#define ENABLE_MSMPEG4V3_ENCODER 0
#define ENABLE_PAM_ENCODER 0
#define ENABLE_PBM_ENCODER 0
#define ENABLE_PGM_ENCODER 0
#define ENABLE_PGMYUV_ENCODER 0
#define ENABLE_PNG_ENCODER 0
#define ENABLE_PPM_ENCODER 0
#define ENABLE_QTRLE_ENCODER 0
#define ENABLE_RAWVIDEO_ENCODER 0
#define ENABLE_ROQ_ENCODER 0
#define ENABLE_RV10_ENCODER 0
#define ENABLE_RV20_ENCODER 0
#define ENABLE_SGI_ENCODER 0
//#define CONFIG_SNOW_ENCODER 1
#define ENABLE_SNOW_ENCODER 0
#define ENABLE_SVQ1_ENCODER 0
#define ENABLE_TARGA_ENCODER 0
#define ENABLE_TIFF_ENCODER 0
#define ENABLE_WMV1_ENCODER 0
#define ENABLE_WMV2_ENCODER 0
#define ENABLE_ZLIB_ENCODER 0
#define ENABLE_ZMBV_ENCODER 0
#define ENABLE_AC3_ENCODER 0
#define ENABLE_FLAC_ENCODER 0
#define ENABLE_LIBAMR_NB_ENCODER 0
#define ENABLE_LIBAMR_WB_ENCODER 0
#define ENABLE_LIBFAAC_ENCODER 0
#define ENABLE_LIBGSM_ENCODER 0
#define ENABLE_LIBGSM_MS_ENCODER 0
#define ENABLE_LIBMP3LAME_ENCODER 0
#define ENABLE_LIBTHEORA_ENCODER 0
#define ENABLE_LIBVORBIS_ENCODER 0
#define ENABLE_MP2_ENCODER 0
#define ENABLE_SONIC_ENCODER 0
#define ENABLE_SONIC_LS_ENCODER 0
#define ENABLE_VORBIS_ENCODER 0
#define ENABLE_WMAV1_ENCODER 0
#define ENABLE_WMAV2_ENCODER 0
#define ENABLE_PCM_ALAW_ENCODER 0
#define ENABLE_PCM_MULAW_ENCODER 0
#define ENABLE_PCM_S8_ENCODER 0
#define ENABLE_PCM_S16BE_ENCODER 0
#define ENABLE_PCM_S16LE_ENCODER 0
#define ENABLE_PCM_S24BE_ENCODER 0
#define ENABLE_PCM_S24DAUD_ENCODER 0
#define ENABLE_PCM_S24LE_ENCODER 0
#define ENABLE_PCM_S32BE_ENCODER 0
#define ENABLE_PCM_S32LE_ENCODER 0
#define ENABLE_PCM_U8_ENCODER 0
#define ENABLE_PCM_U16BE_ENCODER 0
#define ENABLE_PCM_U16LE_ENCODER 0
#define ENABLE_PCM_U24BE_ENCODER 0
#define ENABLE_PCM_U24LE_ENCODER 0
#define ENABLE_PCM_U32BE_ENCODER 0
#define ENABLE_PCM_U32LE_ENCODER 0
#define ENABLE_PCM_ZORK_ENCODER 0
#define ENABLE_ROQ_DPCM_ENCODER 0
#define ENABLE_ADPCM_4XM_ENCODER 0
#define ENABLE_ADPCM_ADX_ENCODER 0
#define ENABLE_ADPCM_CT_ENCODER 0
#define ENABLE_ADPCM_EA_ENCODER 0
#define ENABLE_ADPCM_G726_ENCODER 0
#define ENABLE_ADPCM_IMA_DK3_ENCODER 0
#define ENABLE_ADPCM_IMA_DK4_ENCODER 0
#define ENABLE_ADPCM_IMA_QT_ENCODER 0
#define ENABLE_ADPCM_IMA_SMJPEG_ENCODER 0
#define ENABLE_ADPCM_IMA_WAV_ENCODER 0
#define ENABLE_ADPCM_IMA_WS_ENCODER 0
#define ENABLE_ADPCM_MS_ENCODER 0
#define ENABLE_ADPCM_SBPRO_2_ENCODER 0
#define ENABLE_ADPCM_SBPRO_3_ENCODER 0
#define ENABLE_ADPCM_SBPRO_4_ENCODER 0
#define ENABLE_ADPCM_SWF_ENCODER 0
#define ENABLE_ADPCM_XA_ENCODER 0
#define ENABLE_ADPCM_YAMAHA_ENCODER 0
#define ENABLE_DVBSUB_ENCODER 0
#define ENABLE_DVDSUB_ENCODER 0
//#define CONFIG_AAC_PARSER 1
#define ENABLE_AAC_PARSER 0
//#define CONFIG_AC3_PARSER 1
#define ENABLE_AC3_PARSER 0
//#define CONFIG_CAVSVIDEO_PARSER 1
#define ENABLE_CAVSVIDEO_PARSER 0
//#define CONFIG_DCA_PARSER 1
#define ENABLE_DCA_PARSER 0
//#define CONFIG_DVBSUB_PARSER 1
#define ENABLE_DVBSUB_PARSER 0
//#define CONFIG_DVDSUB_PARSER 1
#define ENABLE_DVDSUB_PARSER 0
//#define CONFIG_H261_PARSER 1
#define ENABLE_H261_PARSER 0
//#define CONFIG_H263_PARSER 1
#define ENABLE_H263_PARSER 0
//#define CONFIG_H264_PARSER 1
#define ENABLE_H264_PARSER 0
//#define CONFIG_MJPEG_PARSER 1
#define ENABLE_MJPEG_PARSER 0
//#define CONFIG_MPEG4VIDEO_PARSER 1
#define ENABLE_MPEG4VIDEO_PARSER 0
//#define CONFIG_MPEGAUDIO_PARSER 1
#define ENABLE_MPEGAUDIO_PARSER 0
//#define CONFIG_MPEGVIDEO_PARSER 1
#define ENABLE_MPEGVIDEO_PARSER 0
//#define CONFIG_PNM_PARSER 1
#define ENABLE_PNM_PARSER 0
//#define CONFIG_VC1_PARSER 1
#define ENABLE_VC1_PARSER 0
//#define CONFIG_AAC_DEMUXER 1
#define ENABLE_AAC_DEMUXER 0
//#define CONFIG_AC3_DEMUXER 1
#define ENABLE_AC3_DEMUXER 0
//#define CONFIG_AIFF_DEMUXER 1
#define ENABLE_AIFF_DEMUXER 0
//#define CONFIG_AMR_DEMUXER 1
#define ENABLE_AMR_DEMUXER 0
//#define CONFIG_APC_DEMUXER 1
#define ENABLE_APC_DEMUXER 0
//#define CONFIG_APE_DEMUXER 1
#define ENABLE_APE_DEMUXER 0
//#define CONFIG_ASF_DEMUXER 1
#define ENABLE_ASF_DEMUXER 0
//#define CONFIG_AU_DEMUXER 1
#define ENABLE_AU_DEMUXER 0
#define ENABLE_AUDIO_BEOS_DEMUXER 0
//#define CONFIG_AVI_DEMUXER 1
#define ENABLE_AVI_DEMUXER 0
#define ENABLE_AVISYNTH_DEMUXER 0
//#define CONFIG_AVS_DEMUXER 1
#define ENABLE_AVS_DEMUXER 0
//#define CONFIG_BETHSOFTVID_DEMUXER 1
#define ENABLE_BETHSOFTVID_DEMUXER 0
#define ENABLE_BKTR_DEMUXER 0
//#define CONFIG_C93_DEMUXER 1
#define ENABLE_C93_DEMUXER 0
//#define CONFIG_DAUD_DEMUXER 1
#define ENABLE_DAUD_DEMUXER 0
//#define CONFIG_DSICIN_DEMUXER 1
#define ENABLE_DSICIN_DEMUXER 0
//#define CONFIG_DTS_DEMUXER 1
#define ENABLE_DTS_DEMUXER 0
//#define CONFIG_DV_DEMUXER 1
#define ENABLE_DV_DEMUXER 0
#define ENABLE_DV1394_DEMUXER 0
//#define CONFIG_DXA_DEMUXER 1
#define ENABLE_DXA_DEMUXER 0
//#define CONFIG_EA_DEMUXER 1
#define ENABLE_EA_DEMUXER 0
//#define CONFIG_FFM_DEMUXER 1
#define ENABLE_FFM_DEMUXER 0
//#define CONFIG_FLAC_DEMUXER 0
#define ENABLE_FLAC_DEMUXER 0
//#define CONFIG_FLIC_DEMUXER 1
#define ENABLE_FLIC_DEMUXER 0
//#define CONFIG_FLV_DEMUXER 1
#define ENABLE_FLV_DEMUXER 0
//#define CONFIG_FOURXM_DEMUXER 1
#define ENABLE_FOURXM_DEMUXER 0
//#define CONFIG_GIF_DEMUXER 1
#define ENABLE_GIF_DEMUXER 0
//#define CONFIG_GXF_DEMUXER 1
#define ENABLE_GXF_DEMUXER 0
//#define CONFIG_H261_DEMUXER 1
#define ENABLE_H261_DEMUXER 0
//#define CONFIG_H263_DEMUXER 1
#define ENABLE_H263_DEMUXER 0
//#define CONFIG_H264_DEMUXER 1
#define ENABLE_H264_DEMUXER 0
//#define CONFIG_IDCIN_DEMUXER 1
#define ENABLE_IDCIN_DEMUXER 0
//#define CONFIG_IMAGE2_DEMUXER 1
#define ENABLE_IMAGE2_DEMUXER 0
//#define CONFIG_IMAGE2PIPE_DEMUXER 1
#define ENABLE_IMAGE2PIPE_DEMUXER 0
//#define CONFIG_INGENIENT_DEMUXER 1
#define ENABLE_INGENIENT_DEMUXER 0
//#define CONFIG_IPMOVIE_DEMUXER 1
#define ENABLE_IPMOVIE_DEMUXER 0
#define ENABLE_LIBDC1394_DEMUXER 0
#define ENABLE_LIBNUT_DEMUXER 0
//#define CONFIG_M4V_DEMUXER 1
#define ENABLE_M4V_DEMUXER 0
//#define CONFIG_MATROSKA_DEMUXER 1
#define ENABLE_MATROSKA_DEMUXER 0
//#define CONFIG_MJPEG_DEMUXER 1
#define ENABLE_MJPEG_DEMUXER 0
//#define CONFIG_MM_DEMUXER 1
#define ENABLE_MM_DEMUXER 0
//#define CONFIG_MMF_DEMUXER 1
#define ENABLE_MMF_DEMUXER 0
//#define CONFIG_MOV_DEMUXER 1
#define ENABLE_MOV_DEMUXER 0
//#define CONFIG_MP3_DEMUXER 1
#define ENABLE_MP3_DEMUXER 0
//#define CONFIG_MPC_DEMUXER 1
#define ENABLE_MPC_DEMUXER 0
//#define CONFIG_MPEGPS_DEMUXER 1
#define ENABLE_MPEGPS_DEMUXER 0
//#define CONFIG_MPEGTS_DEMUXER 1
#define ENABLE_MPEGTS_DEMUXER 0
//#define CONFIG_MPEGTSRAW_DEMUXER 1
#define ENABLE_MPEGTSRAW_DEMUXER 0
//#define CONFIG_MPEGVIDEO_DEMUXER 1
#define ENABLE_MPEGVIDEO_DEMUXER 0
//#define CONFIG_MTV_DEMUXER 1
#define ENABLE_MTV_DEMUXER 0
//#define CONFIG_MXF_DEMUXER 1
#define ENABLE_MXF_DEMUXER 0
//#define CONFIG_NSV_DEMUXER 1
#define ENABLE_NSV_DEMUXER 0
//#define CONFIG_NUT_DEMUXER 1
#define ENABLE_NUT_DEMUXER 0
//#define CONFIG_NUV_DEMUXER 1
#define ENABLE_NUV_DEMUXER 0
//#define CONFIG_OGG_DEMUXER 1
#define ENABLE_OGG_DEMUXER 0
#define ENABLE_OSS_DEMUXER 0
//#define CONFIG_PCM_ALAW_DEMUXER 1
#define ENABLE_PCM_ALAW_DEMUXER 0
//#define CONFIG_PCM_MULAW_DEMUXER 1
#define ENABLE_PCM_MULAW_DEMUXER 0
//#define CONFIG_PCM_S16BE_DEMUXER 1
#define ENABLE_PCM_S16BE_DEMUXER 0
//#define CONFIG_PCM_S16LE_DEMUXER 1
#define ENABLE_PCM_S16LE_DEMUXER 0
//#define CONFIG_PCM_S8_DEMUXER 1
#define ENABLE_PCM_S8_DEMUXER 0
//#define CONFIG_PCM_U16BE_DEMUXER 1
#define ENABLE_PCM_U16BE_DEMUXER 0
//#define CONFIG_PCM_U16LE_DEMUXER 1
#define ENABLE_PCM_U16LE_DEMUXER 0
//#define CONFIG_PCM_U8_DEMUXER 1
#define ENABLE_PCM_U8_DEMUXER 0
//#define CONFIG_RAWVIDEO_DEMUXER 1
#define ENABLE_RAWVIDEO_DEMUXER 0
//#define CONFIG_RM_DEMUXER 1
#define ENABLE_RM_DEMUXER 0
//#define CONFIG_ROQ_DEMUXER 1
#define ENABLE_ROQ_DEMUXER 0
#define ENABLE_REDIR_DEMUXER 0
#define ENABLE_RTSP_DEMUXER 0
#define ENABLE_SDP_DEMUXER 0
//#define CONFIG_SEGAFILM_DEMUXER 1
#define ENABLE_SEGAFILM_DEMUXER 0
//#define CONFIG_SHORTEN_DEMUXER 1
#define ENABLE_SHORTEN_DEMUXER 0
//#define CONFIG_SMACKER_DEMUXER 1
#define ENABLE_SMACKER_DEMUXER 0
//#define CONFIG_SOL_DEMUXER 1
#define ENABLE_SOL_DEMUXER 0
//#define CONFIG_STR_DEMUXER 1
#define ENABLE_STR_DEMUXER 0
//#define CONFIG_SWF_DEMUXER 1
#define ENABLE_SWF_DEMUXER 0
//#define CONFIG_THP_DEMUXER 1
#define ENABLE_THP_DEMUXER 0
//#define CONFIG_TIERTEXSEQ_DEMUXER 1
#define ENABLE_TIERTEXSEQ_DEMUXER 0
//#define CONFIG_TTA_DEMUXER 1
#define ENABLE_TTA_DEMUXER 0
//#define CONFIG_TXD_DEMUXER 1
#define ENABLE_TXD_DEMUXER 0
#define ENABLE_V4L2_DEMUXER 0
#define ENABLE_V4L_DEMUXER 0
//#define CONFIG_VC1_DEMUXER 1
#define ENABLE_VC1_DEMUXER 0
//#define CONFIG_VMD_DEMUXER 1
#define ENABLE_VMD_DEMUXER 0
//#define CONFIG_VOC_DEMUXER 1
#define ENABLE_VOC_DEMUXER 0
//#define CONFIG_WAV_DEMUXER 1
#define ENABLE_WAV_DEMUXER 0
//#define CONFIG_WC3_DEMUXER 1
#define ENABLE_WC3_DEMUXER 0
//#define CONFIG_WSAUD_DEMUXER 1
#define ENABLE_WSAUD_DEMUXER 0
//#define CONFIG_WSVQA_DEMUXER 1
#define ENABLE_WSVQA_DEMUXER 0
//#define CONFIG_WV_DEMUXER 1
#define ENABLE_WV_DEMUXER 0
#define ENABLE_X11_GRAB_DEVICE_DEMUXER 0
//#define CONFIG_YUV4MPEGPIPE_DEMUXER 1
#define ENABLE_YUV4MPEGPIPE_DEMUXER 0
#define ENABLE_AC3_MUXER 0
#define ENABLE_ADTS_MUXER 0
#define ENABLE_AIFF_MUXER 0
#define ENABLE_AMR_MUXER 0
#define ENABLE_ASF_MUXER 0
#define ENABLE_ASF_STREAM_MUXER 0
#define ENABLE_AU_MUXER 0
#define ENABLE_AUDIO_BEOS_MUXER 0
#define ENABLE_AVI_MUXER 0
#define ENABLE_CRC_MUXER 0
#define ENABLE_DV_MUXER 0
#define ENABLE_FFM_MUXER 0
#define ENABLE_FLAC_MUXER 0
#define ENABLE_FLV_MUXER 0
#define ENABLE_FRAMECRC_MUXER 0
#define ENABLE_GIF_MUXER 0
#define ENABLE_GXF_MUXER 0
#define ENABLE_H261_MUXER 0
#define ENABLE_H263_MUXER 0
#define ENABLE_H264_MUXER 0
#define ENABLE_IMAGE2_MUXER 0
#define ENABLE_IMAGE2PIPE_MUXER 0
#define ENABLE_LIBNUT_MUXER 0
#define ENABLE_M4V_MUXER 0
#define ENABLE_MATROSKA_MUXER 0
#define ENABLE_MATROSKA_AUDIO_MUXER 0
#define ENABLE_MJPEG_MUXER 0
#define ENABLE_MMF_MUXER 0
#define ENABLE_MOV_MUXER 0
#define ENABLE_MP2_MUXER 0
#define ENABLE_MP3_MUXER 0
#define ENABLE_MP4_MUXER 0
#define ENABLE_MPEG1SYSTEM_MUXER 0
#define ENABLE_MPEG1VCD_MUXER 0
#define ENABLE_MPEG1VIDEO_MUXER 0
#define ENABLE_MPEG2DVD_MUXER 0
#define ENABLE_MPEG2SVCD_MUXER 0
#define ENABLE_MPEG2VIDEO_MUXER 0
#define ENABLE_MPEG2VOB_MUXER 0
#define ENABLE_MPEGTS_MUXER 0
#define ENABLE_MPJPEG_MUXER 0
#define ENABLE_NULL_MUXER 0
#define ENABLE_NUT_MUXER 0
#define ENABLE_OGG_MUXER 0
#define ENABLE_OSS_MUXER 0
#define ENABLE_PCM_ALAW_MUXER 0
#define ENABLE_PCM_MULAW_MUXER 0
#define ENABLE_PCM_S16BE_MUXER 0
#define ENABLE_PCM_S16LE_MUXER 0
#define ENABLE_PCM_S8_MUXER 0
#define ENABLE_PCM_U16BE_MUXER 0
#define ENABLE_PCM_U16LE_MUXER 0
#define ENABLE_PCM_U8_MUXER 0
#define ENABLE_PSP_MUXER 0
#define ENABLE_RAWVIDEO_MUXER 0
#define ENABLE_RM_MUXER 0
#define ENABLE_ROQ_MUXER 0
#define ENABLE_RTP_MUXER 0
#define ENABLE_SWF_MUXER 0
#define ENABLE_TG2_MUXER 0
#define ENABLE_TGP_MUXER 0
#define ENABLE_VOC_MUXER 0
#define ENABLE_WAV_MUXER 0
#define ENABLE_YUV4MPEGPIPE_MUXER 0
#define ENABLE_FILE_PROTOCOL 0
#define ENABLE_HTTP_PROTOCOL 0
#define ENABLE_PIPE_PROTOCOL 0
#define ENABLE_RTP_PROTOCOL 0
#define ENABLE_TCP_PROTOCOL 0
#define ENABLE_UDP_PROTOCOL 0
//#define CONFIG_DUMP_EXTRADATA_BSF 1
#define ENABLE_DUMP_EXTRADATA_BSF 0
//#define CONFIG_H264_MP4TOANNEXB_BSF 1
#define ENABLE_H264_MP4TOANNEXB_BSF 0
//#define CONFIG_IMX_DUMP_HEADER_BSF 1
#define ENABLE_IMX_DUMP_HEADER_BSF 0
//#define CONFIG_MJPEGA_DUMP_HEADER_BSF 1
#define ENABLE_MJPEGA_DUMP_HEADER_BSF 0
//#define CONFIG_MP3_HEADER_COMPRESS_BSF 1
#define ENABLE_MP3_HEADER_COMPRESS_BSF 0
//#define CONFIG_MP3_HEADER_DECOMPRESS_BSF 1
#define ENABLE_MP3_HEADER_DECOMPRESS_BSF 0
//#define CONFIG_NOISE_BSF 1
#define ENABLE_NOISE_BSF 0
//#define CONFIG_REMOVE_EXTRADATA_BSF 1
#define ENABLE_REMOVE_EXTRADATA_BSF 0

#endif
