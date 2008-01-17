// $Id: svn_revision.h 3205 2008-01-17 14:03:53Z debug $

#ifndef _SVN_REVISION_H
#define _SVN_REVISION_H

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

static const char * REVISION_TEXT	= "$Rev: 3205 $";
static const char * BUILD_TAG		= "3.2-TRUNK-ABC";

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

ASCENT_INLINE int g_getRevision()
{
	const char* p = REVISION_TEXT + 6;
	return atoi( p );
}

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

#endif 
