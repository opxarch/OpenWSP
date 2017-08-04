#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

extern int CFG_stream_cache_size;
extern int CFG_stream_cache_min_percent;
extern int CFG_stream_cache_seek_min_percent;

int init_app_config(void);

#endif // __APP_CONFIG_H__
