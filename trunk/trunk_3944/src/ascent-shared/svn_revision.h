// $Id: svn_revision.h 3850 2008-02-12 16:52:46Z debug $

#ifndef _SVN_REVISION_H
#define _SVN_REVISION_H

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\

static const char * REVISION_TEXT	= "$Rev: 3850 $";
static const char * BUILD_TAG		= "3.8-TRUNK";

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\

ASCENT_INLINE int g_getRevision()
{
	const char* p = REVISION_TEXT + 6;
	return atoi( p );
}

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\

#endif   
