/** @file
 * Openwsp - mf.
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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <openwsp/misc.h>

#if HAVE(GLOB)
#include <glob.h>
#else
#include <openwsp/glob.h>
#endif

#include "../log.h"
#include "stream/stream.h"

#include "mf.h"

/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
int    mf_w = 0; //352; // let codecs to detect it
int    mf_h = 0; //288;
float  mf_fps = 25.0;
char * mf_type = NULL; //"jpg";

namespace openwsp {

////////////////////////////////////////////////////////////////////////////////

static char *strsepInner(char **stringp, const char *delim) {
    register char *s;
    register const char *spanp;
    register int c, sc;
    char *tok;

    if ((s = *stringp) == NULL)
        return NULL;
    for (tok = s;;) {
      c = *s++;
      spanp = delim;
      do {
       if ((sc = *spanp++) == c) {
         if (c == 0)
           s = NULL;
         else
           s[-1] = 0;
         *stringp = s;
         return tok;
       }
      } while (sc != 0);
    }
    /* NOTREACHED */
    return NULL;
}

mf_t* open_mf(char * filename){
#if defined(HAVE_GLOB) || defined(__MINGW32__)
 glob_t        gg;
 struct stat   fs;
 int           i;
 char        * fname;
 mf_t        * mf;
 int           error_count = 0;
 int	       count = 0;

 mf=(mf_t*)calloc( 1,sizeof( mf_t ) );

 if( filename[0] == '@' )
  {
   FILE *lst_f=fopen(filename + 1,"r");
   if ( lst_f )
    {
     fname=(char*)malloc( 255 );
     while ( fgets( fname,255,lst_f ) )
      {
       /* remove spaces from end of fname */
       char *t=fname + strlen( fname ) - 1;
       while ( t > fname && isspace( *t ) ) *(t--)=0;
       if ( stat( fname,&fs ) )
        {
         ws_log(log::M_STREAM,log::VERBOSE)
             << "[mf] file not found: '" << fname << "'"
             << endlog;
        }
        else
        {
         mf->names=(char**)realloc( mf->names,( mf->nr_of_files + 1 ) * sizeof( char* ) );
         mf->names[mf->nr_of_files]=strdup( fname );
         mf->nr_of_files++;
        }
      }
      fclose( lst_f );

      ws_log::write(log::M_STREAM, log::INFO,"[mf] number of files: %d\n",mf->nr_of_files );
      goto exit_mf;
    }
    ws_log( log::M_STREAM,log::INFO)
        << "[mf] " << filename+1 << " is not indirect filelist"
        << endlog;
  }

 if( strchr( filename,',') )
  {
   ws_log(log::M_STREAM, log::INFO)
       << "[mf] filelist: " << filename
       << endlog;

   while ( ( fname=strsepInner( &filename,"," ) ) )
    {
     if ( stat( fname,&fs ) )
      {
       ws_log( log::M_STREAM, log::VERBOSE)
           << "[mf] file not found: '" << fname << "'"
           << endlog;
      }
      else
      {
       mf->names=(char**)realloc( mf->names,( mf->nr_of_files + 1 ) * sizeof( char* ) );
       mf->names[mf->nr_of_files]=strdup( fname );
//       ws_log( MSGT_STREAM,log::V,"[mf] added file %d.: %s\n",mf->nr_of_files,mf->names[mf->nr_of_files] );
       mf->nr_of_files++;
      }
    }
   ws_log( log::M_STREAM,log::INFO)
       << "[mf] number of files: " << mf->nr_of_files
       << endlog;

   goto exit_mf;
  }

 fname=(char*)malloc( strlen( filename ) + 32 );

 if ( !strchr( filename,'%' ) )
  {
   strcpy( fname,filename );
   if ( !strchr( filename,'*' ) ) strcat( fname,"*" );

   ws_log( log::M_STREAM, log::INFO)
       << "[mf] search expr: " << fname
       << endlog;

   if ( glob( fname,0,NULL,&gg ) )
    { free( mf ); free( fname ); return NULL; }

   mf->nr_of_files=gg.gl_pathc;
   mf->names=(char**)calloc( gg.gl_pathc, sizeof( char* ) );

   ws_log( log::M_STREAM, log::INFO)
       << "[mf] number of files: " << mf->nr_of_files
       << " (" << gg.gl_pathc * sizeof( char* ) << ")"
       << endlog;

   for( size_t n=0;n < gg.gl_pathc;n++ )
    {
     stat( gg.gl_pathv[n],&fs );
     if( S_ISDIR( fs.st_mode ) ) continue;
     mf->names[n]=strdup( gg.gl_pathv[n] );
//     ws_log( MSGT_STREAM,log::DBG2,"[mf] added file %d.: %s\n",n,mf->names[n] );
    }
   globfree( &gg );
   goto exit_mf;
  }

 ws_log( log::M_STREAM, log::INFO)
     << "[mf] search expr: " << filename
     << endlog;

 while ( error_count < 5 )
  {
   sprintf( fname,filename,count++ );
   if ( stat( fname,&fs ) )
    {
     error_count++;
     ws_log( log::M_STREAM, log::VERBOSE)
         << "[mf] file not found: '" << fname << "'"
         << endlog;
    }
    else
    {
     mf->names=(char**)realloc( mf->names,( mf->nr_of_files + 1 ) * sizeof( char* ) );
     mf->names[mf->nr_of_files]=strdup( fname );
//     ws_log( MSGT_STREAM,log::V,"[mf] added file %d.: %s\n",mf->nr_of_files,mf->names[mf->nr_of_files] );
     mf->nr_of_files++;
    }
  }

 ws_log( log::M_STREAM, log::INFO) << "[mf] number of files: "
     << mf->nr_of_files
     << endlog;

exit_mf:
 free( fname );
 return mf;
#else
 ws_log(log::M_STREAM, log::FATAL)
     << "[mf] mf support is disabled on your os"
     << endlog;
 return 0;
#endif
}

} // namespace openwsp

