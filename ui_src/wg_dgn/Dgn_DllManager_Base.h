// Dgn_DllManager_Base.h: interface for the CDgn_DllManager_Base class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGN_DLLMANAGER_BASE_H__B368140B_B3DD_4A53_9490_AA75570935B6__INCLUDED_)
#define AFX_DGN_DLLMANAGER_BASE_H__B368140B_B3DD_4A53_9490_AA75570935B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DgnStruct.h"

#include "..\wg_db\DB_ST_DN.h"
#include "..\wg_db\Db_DllManager_Base.h"
#include "..\wg_db\DBLib.h"

#include "..\dgnengine\src\DgnReportBase\XLOptStruct.h"

typedef BOOL (PDGNENGINE_FUNC)(void*, void*, size_t, size_t, BOOL&);
typedef BOOL (PDGNENGINE_SETD)(void*, size_t, BOOL&);
typedef BOOL (PDGNENGINE_FUNC2)(int, void*, void*, size_t, size_t, BOOL&);
typedef BOOL (PDGNENGINE_FUNC3)(void*, void*, void*, size_t, size_t, size_t, BOOL&);
typedef BOOL (PDGNENGINE_FUNC4)(void*, void*, void*, CStringW&, size_t, size_t, size_t, BOOL&);
typedef BOOL (PDGNENGINE_FUNC5)(void*, void*, void*, void*, void*, size_t, size_t, size_t, size_t, size_t, BOOL&);
typedef BOOL (PDGNENGINE_FUNC6)(void*, void*, void*, void*, size_t, size_t, size_t, size_t, BOOL&);

//
typedef BOOL (PDGNENGINE_SETRPT)(void*, void*, size_t, size_t, BOOL&);
typedef BOOL (PDGNENGINE_RPT)(int, void*, void*, void*, size_t, size_t, size_t, BOOL&);
typedef BOOL (PDGNENGINE_RPT_PROG)(int, void*, void*, void*, void*, size_t, size_t, size_t, BOOL&);
typedef BOOL (PDGNENGINE_SETRPT2)(void*, size_t, BOOL&);
typedef BOOL (PDGNENGINE_RPT2)(void*, void*, void*, size_t, size_t, size_t, void*, BOOL&, void*, BOOL&);

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgn_DllManager_Base : public CDb_DllManager_Base        
{
public:
	CDgn_DllManager_Base();
	virtual ~CDgn_DllManager_Base();
	
public:
	// 해당 Code에 대한 DgnEngine과 연결할 국가 Group에 대한 ID를 넘겨줌
	int Get_NationalIDformCode(int iCodeType, int iCode); 
	int Get_NationalIDformRCCode(int iCode);    // return : DGNENGINE_NATION(1:KR, 2:US, 3:CH, 4:JP)
	int Get_NationalIDformSteelCode(int iCode); // return : DGNENGINE_NATION(1:KR, 2:US, 3:CH, 4:JP)
	int Get_NationalIDformSRCCode(int iCode);   // return : DGNENGINE_NATION(1:KR, 2:US, 3:CH, 4:JP)
	int Get_NationalIDformPSCCode(int iCode);   // return : DGNENGINE_NATION(1:KR, 2:US, 3:CH, 4:JP)
	int Get_NationalIDformCSGCode(int nCode);
	int Get_NationalIDformSodCode(int nCode);

	CString Get_CalcDllFileName(int iCodeType, int iCode);
	CString Get_ReportDllFileName(int iCodeType, int iCode);
	CString Get_CalcDllFileName(int iNationType);
	CString Get_ReportDllFileName(int iNationType);
protected:
	HINSTANCE GetOrLoad_CalcDllHandle(int iCodeType, int iCode);	
	HINSTANCE GetOrLoad_ReportDllHandle(int iCodeType, int iCode);	
	HINSTANCE GetOrLoad_CalcDllHandle(int iNationType);	
	HINSTANCE GetOrLoad_ReportDllHandle(int iNationType);


public:
	int  ConvertToDLL_Code(int nCode);

	void GetRptOptionData(CString strProgramDir, 
												CString strProjectFileDir, 
												CString strSaveFileName,
												int nDgnCode,
												D_EXCEL_PRT_OPTION &ROpt);

	int GetSteelCodeConvert2Engine(const UINT& nCode);
};

#include "HeaderPost.h"      

#endif // !defined(AFX_DGN_DLLMANAGER_BASE_H__B368140B_B3DD_4A53_9490_AA75570935B6__INCLUDED_)
