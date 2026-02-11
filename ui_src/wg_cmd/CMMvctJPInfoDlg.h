#if !defined(AFX_CMMVCTJPINFODLG_H__159F1004_0F97_4134_A81F_0A0EAEC59347__INCLUDED_)
#define AFX_CMMVCTJPINFODLG_H__159F1004_0F97_4134_A81F_0A0EAEC59347__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctJPInfoDlg.h : header file
//

#include "CMDlgBase.h"
//#include "..\wg_db\wg_db_DlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvctJPInfoDlg dialog

class CCMMvctJPInfoDlg : public CCMDlgBase
{
// Construction
public:
	CCMMvctJPInfoDlg** m_ppMySelf;
	CCMMvctJPInfoDlg(CWnd* pParent = NULL);   // standard constructor

	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

// Dialog Data
	//{{AFX_DATA(CCMMvctJPInfoDlg)
	enum { IDD = IDD_CMD_ML_LLAN_INFO_JAPAN };
	CEditUnit	m_wndP1Edit;
	CEditUnit	m_wndP2Edit;
	CEditUnit	m_wndCrowdLoad;
	CEditUnit	m_wndTLoadEdit;
	CEditUnit	m_wndImpactCoeff;
	CEditUnit	m_wndExtraCoeff;
	CTextUnit	m_wndP1Unit;
	CTextUnit	m_wndP2Unit;
	CTextUnit	m_CrowdUnit;
	CTextUnit	m_wndTLoadUnit;
	CMouseEdit	m_wndSelBy1;
	CMouseEdit	m_wndSelBy2;
	CTextUnit	m_wndSelByUnit1;
	CTextUnit	m_wndSelByUnit2;
	MEdit	m_wndLength;
	CTextUnit	m_wndLengthUnit;
	int		m_nSelectBy;
	BOOL	m_bP1Load;
	BOOL	m_bP2Load;
	BOOL	m_bCrowdLoad;
	BOOL	m_bTLoadCheck;
	BOOL	m_bExtraCoeff;
	BOOL	m_bImpactCoeff;
	CListCtrl	m_wndElemList;
	CString	m_strName;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctJPInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	//BOOL IsRelevantCode(int nCode);
	//void ShowHideByCode();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvctJPInfoDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVCTJPINFODLG_H__159F1004_0F97_4134_A81F_0A0EAEC59347__INCLUDED_)
