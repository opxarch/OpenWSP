
/* ========================= stream =========================== */

/* stream.cpp: */
#define MSGTR_UnableOpenURL "Unable to open URL: %s\n"
#define MSGTR_ConnToServer "Connected to server: %s\n"
#define MSGTR_FileNotFound "File not found: '%s'\n"

/* demuxer.cpp, demux_*.cpp: */
#define MSGTR_AudioStreamRedefined "WARNING: Audio stream header %d redefined.\n"
#define MSGTR_VideoStreamRedefined "WARNING: Video stream header %d redefined.\n"
#define MSGTR_TooManyAudioInBuffer "\nToo many audio packets in the buffer: (%d in %d bytes).\n"
#define MSGTR_TooManyVideoInBuffer "\nToo many video packets in the buffer: (%d in %d bytes).\n"
#define MSGTR_MaybeNI "Maybe you are playing a non-interleaved stream/file or the codec failed?\n" \
		      "For AVI files, try to force non-interleaved mode with the -ni option.\n"
#define MSGTR_WorkAroundBlockAlignHeaderBug "AVI: Working around CBR-MP3 nBlockAlign header bug!\n"
#define MSGTR_SwitchToNi "\nBadly interleaved AVI file detected - switching to -ni mode...\n"
#define MSGTR_InvalidAudioStreamNosound "AVI: invalid audio stream ID: %d - ignoring (nosound)\n"
#define MSGTR_InvalidAudioStreamUsingDefault "AVI: invalid video stream ID: %d - ignoring (using default)\n"
#define MSGTR_ON2AviFormat "ON2 AVI format"
#define MSGTR_Detected_XXX_FileFormat "%s file format detected.\n"
#define MSGTR_DetectedAudiofile "Audio file detected.\n"
#define MSGTR_NotSystemStream "Not MPEG System Stream format... (maybe Transport Stream?)\n"
#define MSGTR_InvalidMPEGES "Invalid MPEG-ES stream??? Contact the author, it may be a bug :(\n"
#define MSGTR_FormatNotRecognized "============ Sorry, this file format is not recognized/supported =============\n"\
				  "=== If this file is an AVI, ASF or MPEG stream, please contact the author! ===\n"
#define MSGTR_SettingProcessPriority "Setting process priority: %s\n"
#define MSGTR_FilefmtFourccSizeFpsFtime "[V] filefmt:%d  fourcc:0x%X  size:%dx%d  fps:%5.2f  ftime:=%6.4f\n"
#define MSGTR_CannotInitializeMuxer "Cannot initialize muxer."
#define MSGTR_MissingVideoStream "No video stream found.\n"
#define MSGTR_MissingAudioStream "No audio stream found -> no sound.\n"
#define MSGTR_MissingVideoStreamBug "Missing video stream!? Contact the author, it may be a bug :(\n"

#define MSGTR_DoesntContainSelectedStream "demux: File doesn't contain the selected audio or video stream.\n"

#define MSGTR_NI_Forced "Forced"
#define MSGTR_NI_Detected "Detected"
#define MSGTR_NI_Message "%s NON-INTERLEAVED AVI file format.\n"

#define MSGTR_UsingNINI "Using NON-INTERLEAVED broken AVI file format.\n"
#define MSGTR_CouldntDetFNo "Could not determine number of frames (for absolute seek).\n"
#define MSGTR_CantSeekRawAVI "Cannot seek in raw AVI streams. (Index required, try with the -idx switch.)\n"
#define MSGTR_CantSeekFile "Cannot seek in this file.\n"

#define MSGTR_MOVcomprhdr "MOV: Compressed headers support requires ZLIB!\n"
#define MSGTR_MOVvariableFourCC "MOV: WARNING: Variable FourCC detected!?\n"
#define MSGTR_MOVtooManyTrk "MOV: WARNING: too many tracks"
#define MSGTR_FoundAudioStream "==> Found audio stream: %d\n"
#define MSGTR_FoundVideoStream "==> Found video stream: %d\n"
#define MSGTR_DetectedTV "TV detected! ;-)\n"
#define MSGTR_ErrorOpeningOGGDemuxer "Unable to open the Ogg demuxer.\n"
#define MSGTR_ASFSearchingForAudioStream "ASF: Searching for audio stream (id:%d).\n"
#define MSGTR_CannotOpenAudioStream "Cannot open audio stream: %s\n"
#define MSGTR_CannotOpenSubtitlesStream "Cannot open subtitle stream: %s\n"
#define MSGTR_OpeningAudioDemuxerFailed "Failed to open audio demuxer: %s\n"
#define MSGTR_OpeningSubtitlesDemuxerFailed "Failed to open subtitle demuxer: %s\n"
#define MSGTR_DemuxerInfoChanged "Demuxer info %s changed to %s\n"
#define MSGTR_ClipInfo "Clip info:\n"

#define MSGTR_LeaveTelecineMode "\ndemux_mpg: 30000/1001fps NTSC content detected, switching framerate.\n"
#define MSGTR_EnterTelecineMode "\ndemux_mpg: 24000/1001fps progressive NTSC content detected, switching framerate.\n"

#define MSGTR_CacheFill "\rCache fill: %5.2f%% (%"PRId64" bytes)   "
#define MSGTR_NoBindFound "No bind found for key '%s'."
#define MSGTR_FailedToOpen "Failed to open %s.\n"

#define MSGTR_VideoID "[%s] Video stream found, -vid %d\n"
#define MSGTR_AudioID "[%s] Audio stream found, -aid %d\n"
#define MSGTR_SubtitleID "[%s] Subtitle stream found, -sid %d\n"

/* audcodec.cpp: */
#define MSGTR_CantOpenCodec "Could not open codec.\n"
#define MSGTR_CantCloseCodec "Could not close codec.\n"

#define MSGTR_MissingDLLcodec "ERROR: Could not open required DirectShow codec %s.\n"
#define MSGTR_ACMiniterror "Could not load/initialize Win32/ACM audio codec (missing DLL file?).\n"
#define MSGTR_MissingLAVCcodec "Cannot find codec '%s' in libavcodec...\n"

#define MSGTR_MpegNoSequHdr "MPEG: FATAL: EOF while searching for sequence header.\n"
#define MSGTR_CannotReadMpegSequHdr "FATAL: Cannot read sequence header.\n"
#define MSGTR_CannotReadMpegSequHdrEx "FATAL: Cannot read sequence header extension.\n"
#define MSGTR_BadMpegSequHdr "MPEG: bad sequence header\n"
#define MSGTR_BadMpegSequHdrEx "MPEG: bad sequence header extension\n"

#define MSGTR_ShMemAllocFail "Cannot allocate shared memory.\n"
#define MSGTR_CantAllocAudioBuf "Cannot allocate audio out buffer.\n"

#define MSGTR_UnknownAudio "Unknown/missing audio format -> no sound\n"

#define MSGTR_UsingExternalPP "[PP] Using external postprocessing filter, max q = %d.\n"
#define MSGTR_UsingCodecPP "[PP] Using codec's postprocessing, max q = %d.\n"
#define MSGTR_VideoAttributeNotSupportedByVO_VD "Video attribute '%s' is not supported by selected vo & vd.\n"
#define MSGTR_VideoCodecFamilyNotAvailableStr "Requested video codec family [%s] (vfm=%s) not available.\nEnable it at compilation.\n"
#define MSGTR_AudioCodecFamilyNotAvailableStr "Requested audio codec family [%s] (afm=%s) not available.\nEnable it at compilation.\n"
#define MSGTR_OpeningVideoDecoder "Opening video decoder: [%s] %s\n"
#define MSGTR_SelectedVideoCodec "Selected video codec: [%s] vfm: %s (%s)\n"
#define MSGTR_OpeningAudioDecoder "Opening audio decoder: [%s] %s\n"
#define MSGTR_SelectedAudioCodec "Selected audio codec: [%s] afm: %s (%s)\n"
#define MSGTR_BuildingAudioFilterChain "Building audio filter chain for %dHz/%dch/%s -> %dHz/%dch/%s...\n"
#define MSGTR_UninitVideoStr "Uninit video: %s\n"
#define MSGTR_UninitAudioStr "Uninit audio: %s\n"
#define MSGTR_VDecoderInitFailed "VDecoder init failed!\n"
#define MSGTR_ADecoderInitFailed "ADecoder init failed!\n"
#define MSGTR_ADecoderPreinitFailed "ADecoder preinit failed :(\n"
#define MSGTR_AllocatingBytesForInputBuffer "dec_audio: Allocating %d bytes for input buffer.\n"
#define MSGTR_AllocatingBytesForOutputBuffer "dec_audio: Allocating %d + %d = %d bytes for output buffer.\n"

/* ======================= Audio system ======================== */

/* audiosys_oss.cpp */
#define MSGTR_AS_OSS_CantOpenMixer "[AS OSS] audio_setup: Can't open mixer device %s: %s\n"
#define MSGTR_AS_OSS_ChanNotFound "[AS OSS] audio_setup: Audio card mixer does not have channel '%s', using default.\n"
#define MSGTR_AS_OSS_CantOpenDev "[AS OSS] audio_setup: Can't open audio device %s: %s\n"
#define MSGTR_AS_OSS_CantMakeFd "[AS OSS] audio_setup: Can't make file descriptor blocking: %s\n"
#define MSGTR_AS_OSS_CantSet "[AS OSS] Can't set audio device %s to %s output, trying %s...\n"
#define MSGTR_AS_OSS_CantSetChans "[AS OSS] audio_setup: Failed to set audio device to %d channels.\n"
#define MSGTR_AS_OSS_CantUseGetospace "[AS OSS] audio_setup: driver doesn't support SNDCTL_DSP_GETOSPACE :-(\n"
#define MSGTR_AS_OSS_CantUseSelect "[AS OSS]\n   ***  Your audio driver DOES NOT support select()  ***\n Recompile OpenWSP with #undef HAVE_AUDIO_SELECT in config.h !\n\n"
#define MSGTR_AS_OSS_CantReopen "[AS OSS]\nFatal error: *** CANNOT RE-OPEN / RESET AUDIO DEVICE *** %s\n"
#define MSGTR_AS_OSS_UnknownUnsupportedFormat "[AS OSS] Unknown/Unsupported OSS format: %x.\n"

// audiosys_alsa.cpp
#define MSGTR_AS_ALSA_InvalidMixerIndexDefaultingToZero "[AS ALSA] Invalid mixer index. Defaulting to 0.\n"
#define MSGTR_AS_ALSA_MixerOpenError "[AS ALSA] Mixer open error: %s\n"
#define MSGTR_AS_ALSA_MixerAttachError "[AS ALSA] Mixer attach %s error: %s\n"
#define MSGTR_AS_ALSA_MixerRegisterError "[AS ALSA] Mixer register error: %s\n"
#define MSGTR_AS_ALSA_MixerLoadError "[AS ALSA] Mixer load error: %s\n"
#define MSGTR_AS_ALSA_UnableToFindSimpleControl "[AS ALSA] Unable to find simple control '%s',%i.\n"
#define MSGTR_AS_ALSA_ErrorSettingLeftChannel "[AS ALSA] Error setting left channel, %s\n"
#define MSGTR_AS_ALSA_ErrorSettingRightChannel "[AS ALSA] Error setting right channel, %s\n"
#define MSGTR_AS_ALSA_CommandlineHelp "\n[AS ALSA] -ao alsa commandline help:\n"\
"[AS ALSA] Example: -ao alsa:device=hw=0.3\n"\
"[AS ALSA]   Sets first card fourth hardware device.\n\n"\
"[AS ALSA] Options:\n"\
"[AS ALSA]   noblock\n"\
"[AS ALSA]     Opens device in non-blocking mode.\n"\
"[AS ALSA]   device=<device-name>\n"\
"[AS ALSA]     Sets device (change , to . and : to =)\n"
#define MSGTR_AS_ALSA_ChannelsNotSupported "[AS ALSA] %d channels are not supported.\n"
#define MSGTR_AS_ALSA_OpenInNonblockModeFailed "[AS ALSA] Open in nonblock-mode failed, trying to open in block-mode.\n"
#define MSGTR_AS_ALSA_PlaybackOpenError "[AS ALSA] Playback open error: %s\n"
#define MSGTR_AS_ALSA_ErrorSetBlockMode "[AL_ALSA] Error setting block-mode %s.\n"
#define MSGTR_AS_ALSA_UnableToGetInitialParameters "[AS ALSA] Unable to get initial parameters: %s\n"
#define MSGTR_AS_ALSA_UnableToSetAccessType "[AS ALSA] Unable to set access type: %s\n"
#define MSGTR_AS_ALSA_FormatNotSupportedByHardware "[AS ALSA] Format %s is not supported by hardware, trying default.\n"
#define MSGTR_AS_ALSA_UnableToSetFormat "[AS ALSA] Unable to set format: %s\n"
#define MSGTR_AS_ALSA_UnableToSetChannels "[AS ALSA] Unable to set channels: %s\n"
#define MSGTR_AS_ALSA_UnableToDisableResampling "[AS ALSA] Unable to disable resampling: %s\n"
#define MSGTR_AS_ALSA_UnableToSetSamplerate2 "[AS ALSA] Unable to set samplerate-2: %s\n"
#define MSGTR_AS_ALSA_UnableToSetBufferTimeNear "[AS ALSA] Unable to set buffer time near: %s\n"
#define MSGTR_AS_ALSA_UnableToSetPeriodTime "[AS ALSA] Unable to set period time: %s\n"
#define MSGTR_AS_ALSA_BufferTimePeriodTime "[AS ALSA] buffer_time: %d, period_time :%d\n"
#define MSGTR_AS_ALSA_UnableToGetPeriodSize "[AS ALSA] Unable to get period size: %s\n"
#define MSGTR_AS_ALSA_UnableToSetPeriodSize "[AS ALSA] Unable to set period size(%ld): %s\n"
#define MSGTR_AS_ALSA_UnableToSetPeriods "[AS ALSA] Unable to set periods: %s\n"
#define MSGTR_AS_ALSA_UnableToSetHwParameters "[AS ALSA] Unable to set hw-parameters: %s\n"
#define MSGTR_AS_ALSA_UnableToGetBufferSize "[AS ALSA] Unable to get buffersize: %s\n"
#define MSGTR_AS_ALSA_UnableToGetSwParameters "[AS ALSA] Unable to get sw-parameters: %s\n"
#define MSGTR_AS_ALSA_UnableToSetSwParameters "[AS ALSA] Unable to set sw-parameters: %s\n"
#define MSGTR_AS_ALSA_UnableToGetBoundary "[AS ALSA] Unable to get boundary: %s\n"
#define MSGTR_AS_ALSA_UnableToSetStartThreshold "[AS ALSA] Unable to set start threshold: %s\n"
#define MSGTR_AS_ALSA_UnableToSetStopThreshold "[AS ALSA] Unable to set stop threshold: %s\n"
#define MSGTR_AS_ALSA_UnableToSetSilenceSize "[AS ALSA] Unable to set silence size: %s\n"
#define MSGTR_AS_ALSA_PcmCloseError "[AS ALSA] pcm close error: %s\n"
#define MSGTR_AS_ALSA_NoHandlerDefined "[AS ALSA] No handler defined!\n"
#define MSGTR_AS_ALSA_PcmPrepareError "[AS ALSA] pcm prepare error: %s\n"
#define MSGTR_AS_ALSA_PcmPauseError "[AS ALSA] pcm pause error: %s\n"
#define MSGTR_AS_ALSA_PcmDropError "[AS ALSA] pcm drop error: %s\n"
#define MSGTR_AS_ALSA_PcmResumeError "[AS ALSA] pcm resume error: %s\n"
#define MSGTR_AS_ALSA_DeviceConfigurationError "[AS ALSA] Device configuration error."
#define MSGTR_AS_ALSA_PcmInSuspendModeTryingResume "[AS ALSA] Pcm in suspend mode, trying to resume.\n"
#define MSGTR_AS_ALSA_WriteError "[AS ALSA] Write error: %s\n"
#define MSGTR_AS_ALSA_TryingToResetSoundcard "[AS ALSA] Trying to reset soundcard.\n"
#define MSGTR_AS_ALSA_CannotGetPcmStatus "[AS ALSA] Cannot get pcm status: %s\n"

// audio_sdl.cpp
#define MSGTR_AS_SDL_INFO "[AS SDL] Samplerate: %iHz Channels: %s Format %s\n"
#define MSGTR_AS_SDL_DriverInfo "[AS SDL] using %s audio driver.\n"
#define MSGTR_AS_SDL_UnsupportedAudioFmt "[AS SDL] Unsupported audio format: 0x%x.\n"
#define MSGTR_AS_SDL_CantInit "[AS SDL] SDL Audio initialization failed: %s\n"
#define MSGTR_AS_SDL_CantOpenAudio "[AS SDL] Unable to open audio: %s\n"


// audioformat.cpp

#define MSGTR_AUDIO_FORMAT_UnknownFormat "unknown format "


/* ========================== Demux depended =================================== */

/* url.cpp */

#define MSGTR_DEMUX_URL_StringAlreadyEscaped "String appears to be already escaped in url_escape %c%c1%c2\n"
#define MSGTR_MemAllocFailed "Memory allocation failed.\n"

/* asfheader.cpp */

#define MSGTR_DEMUX_ASFHDR_HeaderSizeOver1MB "FATAL: header size bigger than 1 MB (%d)!\nPlease contact us, and send this file.\n"
#define MSGTR_DEMUX_ASFHDR_HeaderMallocFailed "Could not allocate %d bytes for header.\n"
#define MSGTR_DEMUX_ASFHDR_EOFWhileReadingHeader "EOF while reading ASF header, broken/incomplete file?\n"
#define MSGTR_DEMUX_ASFHDR_DVRWantsLibavformat "DVR will probably only work with libavformat, try -demuxer 35 if you have problems\n"
#define MSGTR_DEMUX_ASFHDR_NoDataChunkAfterHeader "No data chunk following header!\n"
#define MSGTR_DEMUX_ASFHDR_AudioVideoHeaderNotFound "ASF: no audio or video headers found - broken file?\n"
#define MSGTR_DEMUX_ASFHDR_InvalidLengthInASFHeader "Invalid length in ASF header!\n"
#define MSGTR_DEMUX_ASFHDR_DRMLicenseURL "DRM License URL: %s\n"
#define MSGTR_DEMUX_ASFHDR_DRMProtected "This file has been encumbered with DRM encryption, it will not play in WSP!\n"

/* cache.cpp */

#define MSGTR_DEMUX_CACHE2_NonCacheableStream "\rThis stream is non-cacheable.\n"
#define MSGTR_DEMUX_CACHE2_ReadFileposDiffers "!!! read_filepos differs!!! Report this bug...\n"

/* network.cpp */

#define MSGTR_DEMUX_NW_UnknownAF "Unknown address family %d\n"
#define MSGTR_DEMUX_NW_ResolvingHostForAF "Resolving %s for %s...\n"
#define MSGTR_DEMUX_NW_CantResolv "Couldn't resolve name for %s: %s\n"
#define MSGTR_DEMUX_NW_ConnectingToServer "Connecting to server %s[%s]: %d...\n"
#define MSGTR_DEMUX_NW_CantConnect2Server "Failed to connect to server with %s\n"
#define MSGTR_DEMUX_NW_SelectFailed "Select failed.\n"
#define MSGTR_DEMUX_NW_ConnTimeout "connection timeout\n"
#define MSGTR_DEMUX_NW_GetSockOptFailed "getsockopt failed: %s\n"
#define MSGTR_DEMUX_NW_ConnectError "connect error: %s\n"
#define MSGTR_DEMUX_NW_InvalidProxySettingTryingWithout "Invalid proxy setting... Trying without proxy.\n"
#define MSGTR_DEMUX_NW_CantResolvTryingWithoutProxy "Could not resolve remote hostname for AF_INET. Trying without proxy.\n"
#define MSGTR_DEMUX_NW_ErrSendingHTTPRequest "Error while sending HTTP request: Didn't send all the request.\n"
#define MSGTR_DEMUX_NW_ReadFailed "Read failed.\n"
#define MSGTR_DEMUX_NW_Read0CouldBeEOF "http_read_response read 0 (i.e. EOF).\n"
#define MSGTR_DEMUX_NW_AuthFailed "Authentication failed. Please use the -user and -passwd options to provide your\n"\
"username/password for a list of URLs, or form an URL like:\n"\
"http://username:password@hostname/file\n"
#define MSGTR_DEMUX_NW_AuthRequiredFor "Authentication required for %s\n"
#define MSGTR_DEMUX_NW_AuthRequired "Authentication required.\n"
#define MSGTR_DEMUX_NW_NoPasswdProvidedTryingBlank "No password provided, trying blank password.\n"
#define MSGTR_DEMUX_NW_ErrServerReturned "Server returns %d: %s\n"
#define MSGTR_DEMUX_NW_CacheSizeSetTo "Cache size set to %d KBytes\n"

/* demux_audio.cpp */

#define MSGTR_DEMUX_AUDIO_UnknownFormat "Audio demuxer: unknown format %d.\n"

/* demux_demuxers.cpp */

#define MSGTR_DEMUX_DEMUXERS_FillBufferError "fill_buffer error: bad demuxer: not vd, ad or sd.\n"
