// $Id: svn_revision.h 2303 2007-11-12 17:15:33Z burlex $

#ifndef _SVN_REVISION_H
#define _SVN_REVISION_H

static const char * REVISION_TEXT	= "$Rev: 2319 ABC $";
static const char * BUILD_TAG		= "2.2-TRUNK";

inline int g_getRevision()
{
	const char * p = REVISION_TEXT + 6;
	return atoi(p);
}

/////////////////////////////////////////////////////////
#endif 
