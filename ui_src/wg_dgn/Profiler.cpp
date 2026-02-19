// Profiler.cpp: implementation of the Profiler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <list>
#include <map>
#include <string>

using namespace std;

#include "Profiler.h"
#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
Profiler *g_pPf = NULL;

Profiler::Profiler()
{
	if (!QueryPerformanceFrequency(&m_ticksPerSec)) 
	{ 
		// 고해상도 타이머를 지원하지 않음 
		m_ticksPerSec.QuadPart=0;
		ASSERT(0);
		return; 
	} 

	m_pLineCheckStartPos = NULL;
	m_strPathName=CDBDoc::GetDocPoint()->GetPathName();
}

Profiler::~Profiler()
{
	CString strFullFileName = m_strPathName;

#if defined(_CIVIL)
	strFullFileName.Replace((CProduct::IsRusLocal() ? _T(".nbm") : _T(".mcb")), _T("_PROF_DGN.RLT"));
#elif defined(_MGEN)
	strFullFileName.Replace(_T(".mgbx"), _T("_PROF_DGN.RLT"));
#endif

	FILE *fp = _wfopen(strFullFileName,_T("wt"));
	ASSERT(fp);
	map<wstring,FuncTimingInfo*>::iterator it;

	fwprintf(fp,_T("%12s%12s%12s%8s%12s\n"),_T("WorkSec"),_T("WorkOnlySec"),_T("ChildSec"),_T("count"),_T("function"));
	
	for( it=FTMap.begin() ; it!=FTMap.end() ; it++)
	{
		FuncTimingInfo *pFuncTimeInfo = it->second;

		fwprintf(fp,_T("%12lf%12lf%12lf%7d %12s\n")
								 ,pFuncTimeInfo->workingSec
								 ,pFuncTimeInfo->workingSec - pFuncTimeInfo->childWorkingSec - pFuncTimeInfo->lostSec
								 ,pFuncTimeInfo->childWorkingSec
								 ,pFuncTimeInfo->count
								 ,pFuncTimeInfo->sFuncName.c_str()
								 );
		delete pFuncTimeInfo;
	}
	FTMap.clear();

	fwprintf(fp,_T("\n+Frofiling by lines---------------------------------------------------------------\n"));
	fwprintf(fp,_T("%12s%8s%12s\n"),_T("WorkSec"),_T("count"),_T("lines_name"));
	for( it=LTMap.begin() ; it!=LTMap.end() ; it++ )
	{
		FuncTimingInfo *pFuncTimeInfo = it->second;

		fwprintf(fp,_T("%12lf%7d %12s\n")
								 , pFuncTimeInfo->workingSec , pFuncTimeInfo->count ,pFuncTimeInfo->sFuncName.c_str() );

		delete pFuncTimeInfo;
	}
	LTMap.clear();

	
	fwprintf(fp,_T("\n+Skiped Frofiling by lines----------------------------------------------------------\n"));
	fwprintf(fp,_T("%8s%12s\n"),_T("count"),_T("lines_name"));
	
	map<wstring,int>::iterator skipIter;  
	for( skipIter = skipInfoMap.begin() ; skipIter!=skipInfoMap.end() ; skipIter++)
	{
		fwprintf(fp,_T("%7d %12s\n")
								 , skipIter->second ,skipIter->first.c_str() );

	}


	fclose(fp);
}

Profiler& Profiler::GetInstance()
{
	if(g_pPf==NULL) g_pPf = new Profiler;
	return *g_pPf;
}

void Profiler::DestroyInstance()
{  
	ASSERT(g_pPf);
	delete g_pPf;
	g_pPf = NULL;  
}

void Profiler::StackPush(TCHAR *szFuncDef)
{
	FuncStartInfo fs;
	QueryPerformanceCounter(&fs.beginT); 
	fs.childWorkingSec = 0;
	fs.sFuncName = szFuncDef;
	FSStack.push_back(fs);
	QueryPerformanceCounter(&FSStack.rbegin()->afterProfilePrepareT );
}

void Profiler::StackPop()
{
	LARGE_INTEGER popStartT;
	LARGE_INTEGER popEndT;
	double workingSec;

	FuncTimingInfo *pFuncTimeInfo;
	
	QueryPerformanceCounter(&popStartT); 

	FuncStartInfo fs = *FSStack.rbegin();
	FSStack.pop_back();

	map<wstring,FuncTimingInfo*>::iterator it;
	it = FTMap.find(fs.sFuncName);
	if( it==FTMap.end() )
	{
		pFuncTimeInfo = new FuncTimingInfo;
		FTMap.insert( make_pair(fs.sFuncName,pFuncTimeInfo) );
	 
		pFuncTimeInfo->sFuncName = fs.sFuncName; 
		pFuncTimeInfo->childWorkingSec = 0.0;
		pFuncTimeInfo->workingSec = 0.0;
		pFuncTimeInfo->lostSec = 0.0;
		pFuncTimeInfo->count = 1;
	}
	else
	{    
		pFuncTimeInfo = it->second;
		pFuncTimeInfo->count ++;
	}
	
	QueryPerformanceCounter(&popEndT); 

	pFuncTimeInfo->childWorkingSec += fs.childWorkingSec;
	pFuncTimeInfo->lostSec += (( popEndT.QuadPart-popStartT.QuadPart + fs.afterProfilePrepareT.QuadPart - fs.beginT.QuadPart)
														/ double(m_ticksPerSec.QuadPart));

	workingSec = ((popEndT.QuadPart - fs.beginT.QuadPart) / (double)m_ticksPerSec.QuadPart);
	
	pFuncTimeInfo->workingSec += workingSec;

	if( FSStack.size()>0) 
	{
		FSStack.rbegin()->childWorkingSec += workingSec;
	}
}

VOID Profiler::LinesByCheckStart(TCHAR *lineBlockName )
{
	if( m_pLineCheckStartPos != NULL )
	{
		map<wstring,int>::iterator skipInfoMapIter = 
			skipInfoMap.find( m_pLineCheckStartPos->sFuncName );
		
		if( skipInfoMapIter == skipInfoMap.end() )
		{
			skipInfoMap.insert( make_pair( m_pLineCheckStartPos->sFuncName , 1));
		}
		else skipInfoMapIter->second ++;

		delete m_pLineCheckStartPos;
		m_pLineCheckStartPos = NULL;
	}

	ASSERT(!m_pLineCheckStartPos);

	m_pLineCheckStartPos = new FuncStartInfo;
	m_pLineCheckStartPos->sFuncName = lineBlockName;
	
	QueryPerformanceCounter(&m_pLineCheckStartPos->afterProfilePrepareT );
}

VOID Profiler::LinesByCheckEnd()
{
	ASSERT(m_pLineCheckStartPos);
	
	double          workS;
	LARGE_INTEGER   endT;
	FuncTimingInfo *pLineTimeInfo;

	QueryPerformanceCounter(&endT); 

	workS = (endT.QuadPart - m_pLineCheckStartPos->afterProfilePrepareT.QuadPart)
				 / double(m_ticksPerSec.QuadPart);
	
	map<wstring,FuncTimingInfo*>::iterator it;

	it = LTMap.find(m_pLineCheckStartPos->sFuncName);
	if( it==LTMap.end() )
	{
		pLineTimeInfo = new FuncTimingInfo;
		LTMap.insert( make_pair( m_pLineCheckStartPos->sFuncName , pLineTimeInfo ) );   
		pLineTimeInfo->sFuncName = m_pLineCheckStartPos->sFuncName;     
		pLineTimeInfo->workingSec = workS;    
		pLineTimeInfo->count = 1;
	}
	else
	{    
		pLineTimeInfo = it->second;
		pLineTimeInfo->count ++;
		pLineTimeInfo->workingSec += workS;
	}

	delete m_pLineCheckStartPos;
	m_pLineCheckStartPos = NULL;
}
