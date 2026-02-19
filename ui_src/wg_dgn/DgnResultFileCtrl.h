#if !defined(AFX_DGNRESULTFILECTRL_H__)
#define AFX_DGNRESULTFILECTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HeaderPre.h"

enum DesignResultId
{
	D_DESIGN_FORCE_RESULT=0,
	D_DESIGN_LCASE_FORCE_RESULT,
	D_DESIGN_LOAD_CASE_FORCE,
	D_DESIGN_LOAD_CASE_PART_FORCE,
	D_DESIGN_FILE_ARRAY_MAX,  // 해석 기능 마지막, 배열 생성
	D_DESIGN_ANALYSIS_ALL,
	

};

enum DesignResultFlag
{
	D_DESIGN_FLAG_MAIN=0,

	D_DESIGN_FLAG_ARRAY_MAX,      // 배열 생성을 위하여 마지막에 위치 할 것
};

class __MY_EXT_CLASS__ CDgnResultFileCtrl    // Test Code by PCS(15.07.08)
{
public:
	CDgnResultFileCtrl();
	virtual ~CDgnResultFileCtrl();
	void Initialize();

// 해석 결과 파일
	BOOL    IsEnableDesignResult(int nDesignResultId);
	BOOL    OpenDesignResultFile(int nDesignResultId);
	BOOL    CloseDesignResultFile(int nDesignResultId);
	BOOL    DeleteDesignResultFile(int nDesignResultId, BOOL bUpdateView);
	CFile*  GetDgnForceResultFilePoint(int nDesignResultId);
	BOOL    ExistDesignResultFile(int nDesignResultId);
	BOOL    BackupDesignResultFile(int nDesignResultId);
	BOOL    RestoreDesignResultFile(int nDesignResultId);

//
	void    SetResultFlag(int nResultFlagId, BOOL bResultFlag);   // D_RESULT_FLAG_????
	BOOL    GetResultFlag(int nResultFlagId);  // D_RESULT_FLAG_????
	void    ClearResultFlag();                 
	int     GetCountResultFlagTrue();

	static CString GetResultFileName(int nDesignResultId);
	LPARAM GetViewUpdateType(int nDesignResultId);
	
	CString GetDesignExt(){return m_sDesignExt;}
	CFile * GetResultFile(int nResultFlagId){ return &m_ResultFile[nResultFlagId]; }  

	static void DisplayErrorMessage(LPCTSTR lpszErrorMsg, int nErrorCode);

protected:
	CFile m_ResultFile[D_DESIGN_FILE_ARRAY_MAX];
	static CString m_sDesignExt;
	BOOL  m_bResultFlag[D_DESIGN_FLAG_ARRAY_MAX];
};

#include "HeaderPost.h"

#endif // !defined(AFX_DGNRESULTFILECTRL_H__)

