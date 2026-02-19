#if !defined(AFX_DGNLATERALCAPAOPTIONDLG_H__8CF357FC_BFF2_429E_BC8E_83C992D560D3__INCLUDED_)
#define AFX_DGNLATERALCAPAOPTIONDLG_H__8CF357FC_BFF2_429E_BC8E_83C992D560D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnLateralCapaOptionDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "DgnLateralCapaOptionDef.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnLateralCapaOptionDlg dialog
class CDgnLateralCapaOptionGlobalDlg;
class CDgnLateralCapaOptionMatlDlg;
class CDgnLateralCapaOptionSectionDlg;

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////

class __MY_EXT_CLASS__ CDgnLateralCapaOptionDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CDgnLateralCapaOptionDlg(CWnd* pParent = NULL);   // standard constructor

public:
	CDBDoc*		m_pDoc;

	// sub dialogs....
	CDgnLateralCapaOptionGlobalDlg*		m_pDlgGlobal;
	CDgnLateralCapaOptionMatlDlg*		  m_pDlgHinge;
	CDgnLateralCapaOptionSectionDlg*	m_pDlgSection;

public:
	void SetParamData(int nKind, UINT Key);   // nKind : (0)MPGB (1)MPHG (2)MPST
	void OnClose();
	void SetMembDataByDB();

// Dialog Data
	//{{AFX_DATA(CDgnLateralCapaOptionDlg)
	enum { IDD = IDD_DGN_LATERAL_CAPA_OPT_DLG };
	CDlgTabCtrl	m_cTabCtrl;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnLateralCapaOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer(LPARAM lHint, CObject* pHint); 

	
	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnLateralCapaOptionDlg)
	virtual BOOL OnInitDialog();	
	afx_msg void OnSelchangeCmdTabCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	LateralCapaDataAll m_DataAll;
	int		m_nKind;	// m_nKind : (0)MPGB (1)MPHG (2)MPST
	UINT	m_nModifyKey;
};

/////////////////////////////////////
#include "HeaderPost.h"      ////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNLATERALCAPAOPTIONDLG_H__8CF357FC_BFF2_429E_BC8E_83C992D560D3__INCLUDED_)
