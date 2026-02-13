// ProfileWrite.h: interface for the ProfileWrite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILEWRITE_H__6F6DE76C_34A4_4B4E_80EA_B20050308147__INCLUDED_)
#define AFX_PROFILEWRITE_H__6F6DE76C_34A4_4B4E_80EA_B20050308147__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ProfileWrite  
{
public:
	static void endLineByCheck();
	static void startLinesByCheck(TCHAR *);
	static void profilingEnd();
	ProfileWrite(TCHAR *szFuncName);
	virtual ~ProfileWrite();
};

#ifdef _PROFILING
#define PROFILING(p) ProfileWrite static_ProfileWrite_pw_XXX(p)
#define PLSTART(p)  ProfileWrite::startLinesByCheck(p);
#define PLEND()     ProfileWrite::endLineByCheck();
#define PROFILING_END() ProfileWrite::profilingEnd();
#else   
#define PROFILING(p)
#define PLSTART(p)
#define PLEND(p)
#define PROFILING_END()
#endif


#endif // !defined(AFX_PROFILEWRITE_H__6F6DE76C_34A4_4B4E_80EA_B20050308147__INCLUDED_)
