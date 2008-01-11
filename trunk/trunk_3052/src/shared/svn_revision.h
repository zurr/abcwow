// $Id: svn_revision.h 3000 2008-01-08 23:12:47Z burlex $

#ifndef _SVN_REVISION_H
#define _SVN_REVISION_H

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

static const char * REVISION_TEXT	= "$Rev: 3000 $";
static const char * BUILD_TAG		= "2.9-TRUNK";

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

ASCENT_INLINE int g_getRevision()
{
	const char* p = REVISION_TEXT + 6;
	return atoi( p );
}

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

#endif 
