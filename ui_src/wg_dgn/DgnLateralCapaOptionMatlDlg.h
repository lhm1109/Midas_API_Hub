#if !defined(AFX_DGNLATERALCAPAOPTIONMATLDLG_H__E1B6E911_91B9_42B8_AE79_5D505DD18662__INCLUDED_)
#define AFX_DGNLATERALCAPAOPTIONMATLDLG_H__E1B6E911_91B9_42B8_AE79_5D505DD18662__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnLateralCapaOptionMatlDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
//#include "..\wg_base\wg_base_ChildDialog.h"
#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "DgnLateralCapaOptionDef.h"


/////////////////////////////////////////////////////////////////////////////
// CDgnLateralCapaOptionMatlDlg dialog

class CDgnLateralCapaOptionMatlChildDlg;
class CDgnLateralCapaOptionDlg;
class CDgnLateralCapaOptionMatlDlg : public CDlgChild, public CDBUpdateConnector
{
// Construction
public:
	CDgnLateralCapaOptionMatlDlg(LateralCapaDataAll* pMPhiDataAll, CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnLateralCapaOptionMatlDlg)
	enum { IDD = IDD_DGN_LATERAL_CAPA_OPT_MATERIAL_DLG };
	CDlgTabCtrl	m_cTabCtrl;
	//}}AFX_DATA
public:
	CDBDoc*			m_pDoc;
	CDgnLateralCapaOptionDlg *m_pParent;
	CDgnLateralCapaOptionMatlChildDlg* m_RcPscDlg;
	//CDgnLateralCapaOptionMatlChildDlg* m_SteelDlg;
	//CDgnLateralCapaOptionMatlChildDlg* m_SRCDlg;

	LateralCapaDataAll* m_pDataAll;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnLateralCapaOptionMatlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL	

public:
	void	SetTabDlg();
	
	//virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void MyUpdate(LPARAM lHint, CObject* pHint);
	void UpdateBuffer(LPARAM lHint, CObject* pHint);   
	void SetModifyData(UINT Key);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnLateralCapaOptionMatlDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeCmdTabCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	T_MATL_K	m_nModifyKey;
	BOOL			m_bModify;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNLATERALCAPAOPTIONMATLDLG_H__E1B6E911_91B9_42B8_AE79_5D505DD18662__INCLUDED_)
