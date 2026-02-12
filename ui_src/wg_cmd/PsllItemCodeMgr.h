// PsllItemCodeMgr.h: interface for the CPsllItemCodeMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__PSLLITEMCODEMGR_H__)
#define __PSLLITEMCODEMGR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"

class CChildDialog;
class CPsllItemDlg;

class CPsllItemCodeMgr  
{
public:
	CPsllItemCodeMgr(CWnd* pParent);
	virtual ~CPsllItemCodeMgr();

	// Interface
public:
	int GetCodeNameList(CStringArray& aCodeName);
	void ChangeCodeDlg(int nCode);
	void Data2CodeDlg(T_PSLL_D& data, int nCode, BOOL bModify);
	BOOL CodeDlg2Data(T_PSLL_D& data);
	// Operation
protected:
	void CreateCodeDlg();    // 생성자에서 자동생성 
	void DeleteCodeDlg();    // 소멸자에서 자동소멸 

	// Member Var.
protected:
	int  m_nCurCode;
	CStringArray m_aCodeName;
	CArray<CChildDialog*, CChildDialog*> m_aCodeDlg;
	CPsllItemDlg* m_pParent;
};

#endif // !defined(__PSLLITEMCODEMGR_H__)
