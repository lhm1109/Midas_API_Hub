// CMWindItemCodeMgr.h: interface for the CCMWindItemCodeMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CMWINDITEMCODEMGR_H__)
#define __CMWINDITEMCODEMGR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"

class CChildDialog;
class CCMWindItemDlg;

class CCMWindItemCodeMgr  
{
public:
	CCMWindItemCodeMgr(CWnd* pParent);
	virtual ~CCMWindItemCodeMgr();

	// Interface
public:
	int  GetCodeNameList(CStringArray& aCodeName, CArray<int, int>& aCodeIndex, CArray<BOOL, BOOL>* aCodeEnable=NULL);
	void ChangeNationalAnnex(int nCodeIndex, int nNAIndex);
	int GetCurCodeType();
	void ChangeCodeDlg(int nCodeIndex);
	void Data2CodeDlg(T_WIND_D& data, int nIndex, BOOL bModify, BOOL bSingapore, T_KEY KeyWAorWT=NULL, T_KEY KeyWind = NULL);
	BOOL CodeDlg2Data(T_WIND_D& data);
	void GetWnatData(T_WNAT_D& WnatD);
	BOOL GetWnpsData(T_WNPS_D& WnpsD);
 
	// Operation
protected:
	void CreateCodeDlg();    // 생성자에서 자동생성 
	void DeleteCodeDlg();    // 소멸자에서 자동소멸 

	// Member Var.
protected:
	int  m_nInitCodeIndex;
	int  m_nCurCodeIndex;
	CStringArray m_aCodeName;
	CArray<CChildDialog*, CChildDialog*> m_aCodeDlg;
	CArray<int , int> m_aCodeType;
	CCMWindItemDlg* m_pParent;
};

#endif // !defined(__CMWINDITEMCODEMGR_H__)
