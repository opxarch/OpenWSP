/** @file
 * Openwsp - glob.
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
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#include <openwsp/misc.h>
#include <openwsp/assert.h>

#if OS(WIN32)
#include <windows.h>
#else
#error port me!
#endif

#include <openwsp/glob.h>

////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

int glob(const char *pattern, int flags,
          int (*errfunc)(const char *epath, int eerrno), glob_t *pglob)
{
#if OS(WIN32)
	HANDLE searchhndl;
    WIN32_FIND_DATA found_file;
 	if(errfunc)printf("glob():ERROR:Sorry errfunc not supported by this implementation\n");
	if(flags)printf("glob():ERROR:Sorry no flags supported by this globimplementation\n");
	//printf("PATTERN \"%s\"\n",pattern);
	pglob->gl_pathc = 0;
	searchhndl = FindFirstFile( pattern,&found_file);
    if(searchhndl == INVALID_HANDLE_VALUE)
	{
		if(GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			pglob->gl_pathc = 0;
		    //printf("could not find a file matching your search criteria\n");
	        return 1;
		}
		else 
		{
			//printf("glob():ERROR:FindFirstFile: %i\n",GetLastError());
			return 1;
		}
	 }
    pglob->gl_pathv = (char**)malloc(sizeof(char*));
    pglob->gl_pathv[0] = strdup(found_file.cFileName);
    pglob->gl_pathc++;
    while(1)
    {
		if(!FindNextFile(searchhndl,&found_file))
		{
			if(GetLastError()==ERROR_NO_MORE_FILES)
			{
				//printf("glob(): no more files found\n");
                break;
			}
			else
			{
				//printf("glob():ERROR:FindNextFile:%i\n",GetLastError());
				return 1;
			}
		}
		else
		{
            //printf("glob: found file %s\n",found_file.cFileName);
            pglob->gl_pathc++;       
            pglob->gl_pathv = (char**)realloc(pglob->gl_pathv,pglob->gl_pathc * sizeof(char*));
            pglob->gl_pathv[pglob->gl_pathc-1] = strdup(found_file.cFileName);       
 		}
    }
    FindClose(searchhndl);
    return 0;
#else
    WS_ASSERT(0)
    return -1;
#endif
}

void globfree(glob_t *pglob) {
#if OS(WIN32)
	size_t i;
	for(i=0; i <pglob->gl_pathc ;i++)
	{
		free(pglob->gl_pathv[i]);
	}
	free(pglob->gl_pathv);
#endif
}

} // namespace openwsp
