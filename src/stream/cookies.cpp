/** @file
 * Openwsp - cookie management.
 */

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

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <inttypes.h>
#include <limits.h>

#include "cookies.h"

#include "../log.h"
#include "stream_http.h"

#define MAX_COOKIES 20

char *cookies_file = NULL;

typedef struct cookie_list_type {
    char *name;
    char *value;
    char *domain;
    char *path;

    int secure;

    struct cookie_list_type *next;
} cookie_list_t;

/* Pointer to the linked list of cookies */
static struct cookie_list_type *cookie_list = NULL;


////////////////////////////////////////////////////////////////////////////////


namespace openwsp {

/* Like strdup, but stops at anything <31. */
static char *col_dup(const char *src)
{
    char *dst;
    int length = 0;

    while (src[length] > 31)
	length++;

    dst = (char*)malloc(length + 1);
    strncpy(dst, src, length);
    dst[length] = 0;

    return dst;
}

static int right_hand_strcmp(const char *cookie_domain, const char *url_domain)
{
    int c_l;
    int u_l;

    c_l = strlen(cookie_domain);
    u_l = strlen(url_domain);

    if (c_l > u_l)
	return -1;
    return strcmp(cookie_domain, url_domain + u_l - c_l);
}

static int left_hand_strcmp(const char *cookie_path, const char *url_path)
{
    return strncmp(cookie_path, url_path, strlen(cookie_path));
}

/* Finds the start of all the columns */
static int parse_line(char **ptr, char *cols[6])
{
    int col;
    cols[0] = *ptr;

    for (col = 1; col < 7; col++) {
        for (; (**ptr) > 31; (*ptr)++);
        if (**ptr == 0)
            return 0;
        (*ptr)++;
        if ((*ptr)[-1] != 9)
            return 0;
        cols[col] = (*ptr);
    }

    return 1;
}

/* Loads a file into RAM */
static char *load_file(const char *filename, off_t * length)
{
    int fd;
    char *buffer;

    ws_log(log::M_NETWORK, log::VERBOSE)
        << "Loading cookie file: " << filename
        << endlog;

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        ws_log(log::M_NETWORK, log::VERBOSE) << "Could not open" << endlog;
        return NULL;
    }

    *length = lseek(fd, 0, SEEK_END);

    if (*length < 0) {
        ws_log(log::M_NETWORK, log::VERBOSE) << "Could not find EOF" << endlog;
        return NULL;
    }

    // FIXME?
    if ((size_t)*length > SIZE_MAX - 1) {
        ws_log(log::M_NETWORK, log::VERBOSE) << "File too big, could not malloc." << endlog;
        return NULL;
    }

    lseek(fd, SEEK_SET, 0);

    if (!(buffer = (char*)malloc(*length + 1))) {
        ws_log(log::M_NETWORK, log::VERBOSE) << "Could not malloc." << endlog;
        return NULL;
    }

    if (read(fd, buffer, *length) != *length) {
        ws_log(log::M_NETWORK, log::VERBOSE) << "Read is behaving funny." << endlog;
        return NULL;
    }
    close(fd);
    buffer[*length] = 0;

    return buffer;
}

/* Loads a cookies.txt file into a linked list. */
static struct cookie_list_type *load_cookies_from(const char *filename,
						  struct cookie_list_type
						  *list)
{
    char *ptr;
    off_t length;

    ws_log(log::M_NETWORK, log::VERBOSE) << "Loading cookie file: " << filename << endlog;

    ptr = load_file(filename, &length);
    if (!ptr)
        return list;

    while (*ptr > 0) {
        char *cols[7];
        if (parse_line(&ptr, cols)) {
            struct cookie_list_type *newc;
            newc = (struct cookie_list_type*)malloc(sizeof(cookie_list_t));
            newc->name = col_dup(cols[5]);
            newc->value = col_dup(cols[6]);
            newc->path = col_dup(cols[2]);
            newc->domain = col_dup(cols[0]);
            newc->secure = (*(cols[3]) == 't') || (*(cols[3]) == 'T');
            newc->next = list;
            list = newc;
        }
    }
    return list;
}

/* Attempt to load cookies.txt from various locations. Returns a pointer to the linked list contain the cookies. */
static struct cookie_list_type *load_cookies(void)
{
    DIR *dir;
    struct dirent *ent;
    struct cookie_list_type *list = NULL;
    char *buf;

    char *homedir;

    if (cookies_file)
        return load_cookies_from(cookies_file, list);

    homedir = getenv("HOME");
    if (!homedir)
        return list;


    buf = (char*)malloc(strlen(homedir) + sizeof("/.mozilla/default") + 1);
    sprintf(buf, "%s/.mozilla/default", homedir);
    dir = opendir(buf);
    free(buf);

    if (dir) {
        while ((ent = readdir(dir)) != NULL) {
            if ((ent->d_name)[0] != '.') {
                buf = (char*)malloc(strlen(getenv("HOME")) +
                                     sizeof("/.mozilla/default/") +
                                     strlen(ent->d_name) + sizeof("cookies.txt") + 1);
                sprintf(buf, "%s/.mozilla/default/%s/cookies.txt",
                     getenv("HOME"), ent->d_name);
                list = load_cookies_from(buf, list);
                free(buf);
            }
        }
        closedir(dir);
    }

    buf = (char*)malloc(strlen(homedir) + sizeof("/.netscape/cookies.txt") + 1);
    sprintf(buf, "%s/.netscape/cookies.txt", homedir);
    list = load_cookies_from(buf, list);
    free(buf);

    return list;
}

/* Take an HTTP_header_t, and insert the correct headers. The cookie files are read if necessary. */
void cookies_set(HTTP_header_t * http_hdr, const char *domain, const char *url)
{
    int found_cookies = 0;
    struct cookie_list_type *cookies[MAX_COOKIES];
    struct cookie_list_type *list, *start;
    int i;
    const char *path;
    char *buf;

    path = strchr(url, '/');
    if (!path)
	path = "";

    if (!cookie_list)
	cookie_list = load_cookies();


    list = start = cookie_list;

    /* Find which cookies we want, removing duplicates. Cookies with the longest domain, then longest path take priority */
    while (list) {
        /* Check the cookie domain and path. Also, we never send "secure" cookies. These should only be sent over HTTPS. */
        if ((right_hand_strcmp(list->domain, domain) == 0)
            && (left_hand_strcmp(list->path, path) == 0) && !list->secure) {
            int replacing = 0;
            for (i = 0; i < found_cookies; i++) {
                if (strcmp(list->name, cookies[i]->name) == 0) {
                    replacing = 0;
                    if (strlen(list->domain) <= strlen(cookies[i]->domain)) {
                        cookies[i] = list;
                    } else if (strlen(list->path) <= strlen(cookies[i]->path)) {
                        cookies[i] = list;
                    }
                }
            }
            if (found_cookies > MAX_COOKIES) {
                /* Cookie jar overflow! */
                break;
            }
            if (!replacing)
                cookies[found_cookies++] = list;
        }
        list = list->next;
    }


    buf = strdup("Cookie:");

    for (i = 0; i < found_cookies; i++) {
        char *nbuf;

        nbuf = (char*)malloc(strlen(buf) + strlen(" ") + strlen(cookies[i]->name) +
                strlen("=") + strlen(cookies[i]->value) + strlen(";") + 1);
        sprintf(nbuf, "%s %s=%s;", buf, cookies[i]->name,
             cookies[i]->value);
        free(buf);
        buf = nbuf;
    }

    if (found_cookies)
        http_set_field(http_hdr, buf);
    else
        free(buf);
}

} // namespace openwsp
