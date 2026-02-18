// ProfileWrite.cpp: implementation of the ProfileWrite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <list>
#include <map>
#include <string>

using namespace std;

#include "ProfileWrite.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "profiler.h"

ProfileWrite::ProfileWrite(TCHAR *szFuncName)
{  
	PROFILER.StackPush(szFuncName);
}

ProfileWrite::~ProfileWrite()
{
	PROFILER.StackPop();
}

void ProfileWrite::startLinesByCheck(TCHAR *p)
{  
	PROFILER.LinesByCheckStart(p);
}

void ProfileWrite::endLineByCheck()
{  
	PROFILER.LinesByCheckEnd();
}

void ProfileWrite::profilingEnd()
{
	Profiler::DestroyInstance();
}


