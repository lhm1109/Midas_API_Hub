#if !defined(__MPHI_MATERIAL_DLG_JP_H__)
#define __MPHI_MATERIAL_DLG_JP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MPhiMaterialDlg_JP.h : header file

#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CMPhiMaterialDlg_JP dialog
struct MPhiDataAll;
class CDBDoc;
class CMPhiMaterialRcDlg_JP;
class CMPhiMaterialSteelDlg_JP;
class CMPhiMaterialSrcDlg_JP;

class CMPhiMaterialDlg_JP : public CDlgChild
{
// Construction
public:
	CMPhiMaterialDlg_JP(MPhiDataAll* pMPhiDataAll, CWnd* pParent = NULL);   // standard constructor
	CDlgTabCtrl	m_cTabCtrl;
	enum { IDD = IDD_CMD_MPHI_ELEMENT_DLG };

public:
	CDBDoc* m_pDoc;
	CMPhiMaterialRcDlg_JP* m_RcPscDlg;
	CMPhiMaterialSteelDlg_JP* m_SteelDlg;
	CMPhiMaterialSrcDlg_JP* m_SRCDlg;

	MPhiDataAll* m_pMPhiDataAll;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMPhiMaterialDlg_JP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetTabDlg();
	//virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void MyUpdate(LPARAM lHint, CObject* pHint);
	void UpdateBuffer(LPARAM lHint, CObject* pHint);
	void SetModifyData(UINT Key);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMPhiHingeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdTabCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOK();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	T_MATL_K	m_nModifyKey;
	BOOL		m_bModify;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__MPHI_MATERIAL_DLG_JP_H__)
