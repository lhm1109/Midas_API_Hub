// Profiler.h: interface for the Profiler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILER_H__2C234D87_57D7_4066_BE4B_225F07582F9E__INCLUDED_)
#define AFX_PROFILER_H__2C234D87_57D7_4066_BE4B_225F07582F9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Profiler  
{  
	Profiler();
public:
	struct FuncStartInfo {
		wstring sFuncName;
		LARGE_INTEGER beginT ,afterProfilePrepareT;
		double childWorkingSec;
	};

	struct FuncTimingInfo
	{
		wstring sFuncName;
		double workingSec;
		double lostSec;
		double childWorkingSec;
		int    count;
	};

private:
	LARGE_INTEGER m_ticksPerSec; 
	list<FuncStartInfo> FSStack;
	map<wstring,FuncTimingInfo*> FTMap;

	FuncStartInfo *m_pLineCheckStartPos;//FuncStartInfo의 이름과 after_Profile_PrepareT만 사용한다. 
	map<wstring,FuncTimingInfo*> LTMap;  //line timing information...
	map<wstring,int> skipInfoMap;

	CString m_strPathName;

public:
	VOID LinesByCheckEnd();
	VOID LinesByCheckStart(TCHAR *);
	static Profiler& GetInstance();
	static void DestroyInstance();

	void StackPop();
	void StackPush(TCHAR *szFuncDef);
	virtual ~Profiler();

};

#define PROFILER Profiler::GetInstance()

#endif // !defined(AFX_PROFILER_H__2C234D87_57D7_4066_BE4B_225F07582F9E__INCLUDED_)
