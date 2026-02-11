#if !defined(__CmdAutoLoadCombImperfectionLoadDlg_H__)
#define __CmdAutoLoadCombImperfectionLoadDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombImperfectionLoadDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "CmdAutoLoadCombImperfectionLoadGrid.h"

class CDBDoc;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCmdAutoLoadCombImperfectionLoadDlg: public CDialogMove
{
public:
	CCmdAutoLoadCombImperfectionLoadDlg(LPCTSTR strCurCode, CWnd* pParent = NULL);   // standard constructor
	~CCmdAutoLoadCombImperfectionLoadDlg();

	enum { IDD = IDD_CMD_LCOMBDEFAULT_IMPF_LOAD_DLG};

	void GetData(CArray<_LCOM_IMPF, _LCOM_IMPF&>& aData);
	void SetData(CArray<_LCOM_IMPF, _LCOM_IMPF&>& aData);

protected: 
	// 함수(컨트럴)
	afx_msg void OnOk();
	DECLARE_MESSAGE_MAP()

	// 함수(상속)
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// 함수(일반)
	void SetControl();
	BOOL Dlg2Data();
	void Data2Dlg();
	BOOL CheckData(_LCOM_IMPF& Data);
	BOOL CheckDataLCase(int nAnalType, UINT uiKey);

	// 변수(컨트럴)
	CCmdAutoLoadCombImperfectionLoadGrid m_Grid;

	// 변수(일반)
	CDBDoc* m_pDoc;

	CArray<_LCOM_IMPF, _LCOM_IMPF&> m_aData;
};
#include "HeaderPost.h"

#endif // !defined(__CmdAutoLoadCombImperfectionLoadDlg_H__)


