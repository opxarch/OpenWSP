
#include "demuxer/stheader.h"

namespace openwsp {

extern int audio_output_channels;

// prototypes:
static int preinit(sh_audio_t *sh);
static int init(sh_audio_t *sh);
static int uninit(sh_audio_t *sh);
static int control(sh_audio_t *sh, int cmd, void* arg, ...);
static int decode(sh_audio_t *sh, unsigned char *buf, int minlen, int maxlen);

#define AD_EXTERN(x) ad_functions_t audio_codec_##x =\
{\
    &info,\
    preinit,\
    init,\
    uninit,\
    control,\
    decode\
};

} // namespace openwsp
