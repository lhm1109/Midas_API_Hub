#if !defined __SEISITEMCODEMGR_H__
#define __SEISITEMCODEMGR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"

class CChildDialog;
class CSeisItemDlgNew;

class CSeisItemCodeMgr
{
public:
	CSeisItemCodeMgr(CWnd* pParent);
	virtual ~CSeisItemCodeMgr();

public:
	int GetCodeNameList(CStringArray& aCodeName, CArray<int, int>& aCodeIndex, CArray<BOOL, BOOL>* aCodeEnable = NULL);
	void ChangeNationalAnnex(int nCodeIndex, int nNAIndex);
	int GetCurCodeType();
	void ChangeCodeDlg(int nCodeIndex);
	void Data2CodeDlg(T_SEIS_D& data, int nIndex, BOOL bModify);
	BOOL CodeDlg2Data(T_SEIS_D& data);

protected:
	void CreateCodeDlg(); // 생성자에서 자동생성
	void DeleteCodeDlg(); // 소멸자에서 자동소멸

protected:
	int m_nInitCodeIndex;
	int m_nCurCodeIndex;
	CStringArray m_aCodeName;
	CArray<CChildDialog*, CChildDialog*> m_aCodeDlg;
	CSeisItemDlgNew* m_pParent;
};

#endif // !defined(__SEISITEMCODEMGR_H__)
