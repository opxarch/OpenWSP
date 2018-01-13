/**@file
 * OpenWSP webservice - QingTing-FM APIs
 */

/*
 * NOTE: This file is generated automatically by 'apicmp'.
 * Don't try to modify this file directly, or you may have to change the
 * checksum and other fields in the file header.
 *
 * this file is NOT any part of qingting API, and it's
 * only issued and used by OpenWSP.
 */
 
/*
 *  OpenWSP (an opensource web stream codec)
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

register(
    "QingTing-FM",
    "Qingting FM support API",
    "OpenWSP",
    0,0,1,
    "2017.07.09"
);

/*******************************************************************************
*   Internal Global Variables                                                  *
*******************************************************************************/

// servers
var api = {
    host:       'http://api2.qingting.fm',
    domains:    '/v6/media/mediacenterlist',
    categories: '/v6/media/categories/5',
    channels:   '/v6/media/categories/5/channels',
    channel:    '/v6/media/channellives'
};

// internals
var audio_stream_host;
var audio_stream_access;
var audio_stream_replay;
var audio_stream_bitrate;
var device_id;

var util = new OpenWSP.util();
var debug = new OpenWSP.debug();


////////////////////////////////////////////////////////////////////////////////

 
/**
 * Main interface that communicates with OpenWSP.
 */
var wspAPI = {
    /**
     * Connect to the server.
     * @return true if succeeded.
     * @return false if failed.
     */
    connect: function() {
        var code = request(api.host + api.domains);
        if (code == void 0) {
            debug.error("catalogs(): connect to the server.");
            return;
        }
        
        var json = JSON.parse(code);
        
        var rs = json.data.radiostations_http.mediacenters;
        var rs_count = rs.length;
        
        if (rs == void 0) {
            debug.error("connect(): reading server hosts.");
            return false;
        }
        
        debug.print("connecting to the radio station...");
        debug.print("count: " + rs_count);
        
        var tst_url;
        var response_time;
        var min_response_time = 0;
        
        /*
         test the response time of all servers.
         */
        for (var i=0; i<rs_count; i++) {
            tst_url = rs[i].protocol + "://" + rs[i].domain;
            response_time = util.getResponseTime(tst_url, 80)
            
            /* find the fastest */
            if (!min_response_time ||
                    (response_time < min_response_time) ) {
                min_response_time = response_time;
                audio_stream_host = rs[i].domain;
                audio_stream_access = rs[i].access;
                audio_stream_replay = rs[i].replay;
                audio_stream_bitrate = rs[i].bitrate;
                if (audio_stream_bitrate == void 0) {
                    audio_stream_bitrate = 64; // default
                }
            }
            
            debug.print("host: " + tst_url);
            debug.print("time: " + response_time);
        }
        
        if ((audio_stream_host == void 0) ||
                ((audio_stream_access == void 0) &&
                (audio_stream_replay == void 0)) )
          {
            debug.error("connect(): parsing the data.");
            return false;
        }
        
        /* add the default protocol field */
        if (audio_stream_host.indexOf('://') == -1) {
            audio_stream_host = 'http://' + audio_stream_host;
        }
        
        device_id = 10000 * Math.random();
        
        debug.print("server resolved: " + audio_stream_host);
        
        return true;
    },
    
    /**
     * Get the catalogs of radio stations.
     * @return a array of catalogs if succeeded.
     */
    catalogs: function() {
        var code = request(api.host + api.categories);
        if (code == void 0) {
            debug.error("catalogs(): connect to the server.");
            return;
        }
        
        var json = JSON.parse(code);
        
        var catas = json.data;
        var catas_count = catas.length;
        
        if ((catas == void 0) ||
            (catas[0].name == void 0) ||
            (catas[0].id == void 0) ) {
            debug.error("catalogs(): parsing the data.");
            return;
        }
        
        var res = new Array();
        
        for (var i=0; i<catas_count; i++) {
            var subs_count = catas[i].values.length
            
            /* resolve the sub categories */
            for (var j=0; j<subs_count; j++) {
                var dst = new Object();
                
                dst.name = catas[i].values[j].name;
                dst.desc = "unknown";
                dst.catalog = catas[i].name;
                dst.id = catas[i].values[j].id;
                
                res.push(dst);
            }
        }
        
        return res;
    },
    
    /**
     * Get the radio channels in the catalog.
     * @return a array of channels if succeeded.
     */
    channels: function(id) {
        
        var res = new Array();
        
        var page = 1;
        
        do {
            var parms = '/order/0/attr/' + id + '/curpage/' + page + '/pagesize/30';
            var code = request(api.host + api.channels + parms);
            if (code == void 0) {
                debug.print("catalogs(): connect to the server.");
                break;
            }
            
            var json = JSON.parse(code);
            var data = json.data;
            var count = data.length;
            
            if (count) {
                /*
                 * traveling each channel nodes, parsing data and push then
                 * to the list.
                 */
                for (var i=0; i<count; i++) {
                    var dst = inner_channel_info__(data[i]);
                    if (dst != void 0) {
                        res.push(dst);
                    }
                }
                
            } else {
                /* we have reached at the end */
                break;
            }
            
            page++;
            
        } while(1);
        
        return res;
    },
    
    /**
     * Get the infomation of a radio channel.
     * @return a array of catalogs if succeeded.
     */
    channel: function(id) {
        var code = request(api.host + api.channel + '/' + id);
        if (code == void 0) {
            debug.error("catalogs(): connect to the server.");
            return;
        }
        
        var json = JSON.parse(code);
        var data = json.data;
        
        var rs = inner_channel_info__(data);
        if (rs == void 0) {
            debug.error('failed on channel(): parsing.');
        }
        return rs;
    },
    
    programs: function() {
        
    },
    
    channelURL: function(id) {
        var res = new Object();
        
        var locator = audio_stream_access;
        locator = locator.replace('${res_id}', id);
        locator = locator.replace('${BITRATE}', audio_stream_bitrate);
        locator = locator.replace('${DEVICEID}', device_id);
        
        res.url = audio_stream_host + locator;
        
        debug.print("channelURL(): " + res.url);
        return res;
    },
    
    release: function() {
        debug = null
        util = null
    }
};



////////////////////////////////////////////////////////////////////////////////


/**
 * Request the data through HTTP.
 * @param url Uniform Resoure Locator string.
 */
function request(url) {
    var file = new OpenWSP.stream(url);
    var buff = file.read();
    file = null;
    return buff;
}

/**
 * Inner, worker for channels() and channel().
 * @param item The JSON data node.
 * @return (object)     a new channel object.
 * @return (undefined)  the source data is invalid.
 */
function inner_channel_info__(item) {
    var dst = new Object();

    /*
     * validate the root
     */
    if (item == void 0) {
        return void 0;
    }
    
    /*
     * query the basic information
     */
    dst.name = item.title;
    dst.description = item.description;
    dst.id = item.id;
    
    /*
     * thumb resources
     */
    if (item.thumbs == void 0) {
        dst.thumb_small = void 0;
        dst.thumb_medium = void 0;
        dst.thumb_large = void 0;
    } else {
        dst.thumb_small  = item.thumbs.small_thumb;
        dst.thumb_medium = item.thumbs.medium_thumb;
        dst.thumb_large  = item.thumbs.large_thumb;
    }
    
    /*
     * query the media information
     */
    dst.mediainfo = new Array();
    
    /* is the channel unavailable ? */
    if (item.mediainfo != void 0) {
        var media = new Object();
        
        media.id = item.mediainfo.id;
        dst.mediainfo.push(media);
    } else {
        debug.print('no mediainfo!');
    }
    
    /*
     * extra properties
     */
    dst.exproperty = new Array();
    var prop;
    
    if (item.freq != void 0 && item.freq.length) {
        prop = new Object();
        
        prop.name = "frequency (MHz)";
        prop.description = "the frequency in FM radio broadcast.";
        prop.value = item.freq;
        
        dst.exproperty.push(prop);
    }
    
    if (item.update_time != void 0 && item.update_time.length) {
        prop = new Object();
        
        prop.name = "update time";
        prop.description = "the last updated time.";
        prop.value = item.update_time;
        
        dst.exproperty.push(prop);
    }
    
    if (item.stream_status != void 0) {
        prop = new Object();
        
        prop.name = "stream status";
        prop.description = "0 if the stream is unhealthy, 1 if it's on the contrary.";
        prop.value = item.stream_status;
        
        dst.exproperty.push(prop);
    }
    
    return dst;
}
