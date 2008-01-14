// $Id: svn_revision.h 3128 2008-01-14 04:18:50Z debug $

#ifndef _SVN_REVISION_H
#define _SVN_REVISION_H

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

static const char * REVISION_TEXT	= "$Rev: 3128 $";
static const char * BUILD_TAG		= "3.1-TRUNK-ABC";

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

ASCENT_INLINE int g_getRevision()
{
	const char* p = REVISION_TEXT + 6;
	return atoi( p );
}

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

#endif 
