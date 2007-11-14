// $Id: svn_revision.h 2020 2007-10-21 12:17:52Z burlex $

#ifndef _SVN_REVISION_H
#define _SVN_REVISION_H

static const char * REVISION_TEXT	= "$Rev: 2020 $";
static const char * BUILD_TAG		= "2.0-RELEASE";

inline int g_getRevision()
{
	const char * p = REVISION_TEXT + 6;
	return atoi(p);
}

///////////////////////////////////////////////
#endif 
