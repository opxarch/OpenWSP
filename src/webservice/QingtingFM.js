/**@file
 * OpenWSP webservice - QingTing-FM APIs
 */

/*
 * NOTE: This file is generated automatically by 'apicmp'.
 * Don't try to modify this file directly, or you have to change the
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
    "Qingting FM support API",
    "OpenWSP",
    0,0,1,
    "2017.07.09"
);

// servers
var api = {
    host: 'http://api2.qingting.fm',
    domains: '/v6/media/mediacenterlist',
    categories: '/v6/media/categories'
};

// internals
var audio_stream_host;
var audio_stream_access;
var audio_stream_replay;

var util = new OpenWSP.util();
var debug = new OpenWSP.debug();

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
        
        debug.print("server resolved: " + audio_stream_host);
        
        return true;
    },
    
    /**
     * Get the catalogs of radio stations.
     * @return a array of catalogs if succeeded.
     */
    catalogs: function() {
        var code = request(api.host + api.categories);
        var json = JSON.parse(code);
        
        var catas = json.data;
        var catas_count = catas.length;
        
        if ((catas == void 0) ||
            (catas[0].name == void 0) ||
            (catas[0].id == void 0) ) {
            debug.error("catalogs(): parsing the data.")
            return;
        }
        
        var res = [];
        
        for (var i=0; i<catas_count; i++) {
            res[i] = new Object();
            res[i].name = catas[i].name;
            res[i].id = catas[i].id;
        }
        
        return res;
    },
    
    channels: function() {
        
    },
    
    programs: function() {
        
    },
    
    release: function() {
        debug = null
        util = null
    }
};

