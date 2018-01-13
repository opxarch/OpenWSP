/** @file
 * apicmp - Compiler for webservice APIs
 */

/*
 *  OpenWSP (an opensource web stream player)
 *  Copyleft (C) 2016, The 1st Middle School in Yongsheng Lijiang China
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
#include <string.h>

#include <openwsp/apifile.h>
#include <openwsp/crc.h>
#include <openwsp/bswap.h>

#define DEBUG_APICMP 0
#define BUFF_CHUNK_SIZE (8192)

/**
 * Show the uasge.
 * @param   argc        The count of command-line arguments.
 * @param   argv        Vector list of command-line arguments.
 * @returns Exiting status.
 */
static int usage(int argc, char *argv[]) {
    fprintf(stderr,
            "usage: %s [sourcefile] [outfile].\n"
            "expected 2 parameters but now %d.\n", argv[0], argc);
    fflush(stderr);
    return 1;
}

/**
 * File size.
 *
 * @returns file size in bytes.
 * @returns 0 on failure.
 * @param   pf   File to size.
 */
static size_t fsize(FILE *pf) {
    long len;
    off_t Pos = ftell(pf);
    if ( Pos >= 0
        &&  !fseek(pf, 0, SEEK_END)) {
        len = ftell(pf);
        if ( len >= 0
            &&  !fseek(pf, 0, SEEK_SET))
            return len;
    }
    return 0;
}

/**
 * Program main entry
 *
 * @param   argc        The count of command-line arguments.
 * @param   argv        Vector list of command-line arguments.
 * @returns Exiting status.
 */
int main(int argc, char *argv[]) {
    int         i = 0;
    const char  *sourcefile;
    const char  *outfile;
    FILE        *fsrc;
    FILE        *fout;
    uint32_t    srclen = 0;
    uint32_t    outlen = 0;
    apifile_t   *buff;
    uint8_t     *wpos;
    size_t      len;

    if (argc < 3) {
        return usage(argc, argv);
    }

    sourcefile = argv[1];
    outfile = argv[2];

#if DEBUG_APICMP
    printf("apicmp: source file: %s\n", sourcefile);
    printf("apicmp: out file: %s\n", outfile);
#endif

    fsrc = fopen(sourcefile, "rb");
    if (!fsrc) {
        fprintf(stderr, "apicmp: error: cann't open the input file '%s'\n", sourcefile);
        return 1;
    }

    srclen = fsize(fsrc);
    outlen = sizeof(*buff) + srclen + 1;

    buff = (apifile_t *)malloc(outlen);
    if (!buff) {
        fprintf(stderr, "apicmp: error: failed on allocing memory.\n");
        return 1;
    }

    /*
     Generate file header
     */
    memset(buff, 0, outlen);
    buff->magic             = ne2be (NUMBER_MAGIC_APIFILE);
    buff->codepage_len      = ne2be (srclen);
    buff->codepage_offset   = ne2be (sizeof (apifile_t));

#if DEBUG_APICMP
    printf("apicmp: codepage_len: 0x%x\n", srclen);
    printf("apicmp: codepage_offset: 0x%x\n", sizeof (apifile_t);
#endif

    wpos = (uint8_t *)buff;
    buff += 1;

    /*
     Generate code page
     */
    len = fread(buff, 1,srclen, fsrc);
    if (!len && ferror(fsrc)) {
        fprintf(stderr, "apicmp: error: cann't read the input file '%s'.\n", sourcefile);
        return 1;
    }
    if (len < srclen) {
        fprintf(stderr, "apicmp: error: cann't reach the end of input file '%s'.\n", sourcefile);
        return 1;
    }

    fclose(fsrc);

    ((uint8_t*)buff)[srclen] = '\0';

    for (i = 0; i < len; i++) {
        ((uint8_t*)buff)[i] += ASCII_BASE_OFFSET;
    }

    /*
     Generate CRC-32 checksum
     */
    ((apifile_t *)wpos)->codepage_checksum = ne2be ( crc32 (buff, len) );

#if DEBUG_APICMP
    printf("apicmp: codepage_checksum: 0x%x\n", (be2ne( (apifile_t *)wpos)->codepage_checksum) );
#endif

    /*
     Dump the content of file if we're in debug
     */
#if DEBUG_APICMP > 1
    printf("======= BEGIN dump file =======\n");
    for (i = 0; i < outlen; i++) {
        printf("%x ", wpos[i]);
        if (!((i + 1) % 16))
            printf("\n");
    }
    printf("\n======= END dump file =======\n");
#endif

    fout = fopen(outfile, "wb");
    if (!fout) {
        fprintf(stderr, "apicmp: error: cann't create the output file '%s'\n", outfile);
        return 1;
    }

    /*
     Write to the output file
     */
    len = fwrite(wpos, 1,outlen, fout);
    if (!len && ferror(fout)) {
        fprintf(stderr, "apicmp: error: cann't write the output file.\n");
        return 1;
    }
    if (len < outlen) {
        fprintf(stderr, "apicmp: error: writing was truncated.\n");
        return 1;
    }

    fclose(fout);

    /* done. */
    return 0;
}
