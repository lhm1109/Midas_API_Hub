#if !defined(__RCDESIGNPARAMDLG_H__)
#define __RCDESIGNPARAMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CRCDesignParamDlg.h : header file
//

#include "DgnDlgBase.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\MIT_frx\MButton.h"
#include "wg_dgnres.h"

/////////////////////////////////////////////////////////////////////////////
// CRCDesignParamDlg dialog



class CRCBeamParameter;
class CRCColumnParameter;
class CRCWallParameter;

#include "HeaderPre.h"
using namespace mit::frx;
class __MY_EXT_CLASS__ CRCDesignParamDlg : public CDgnDlgBase, public CDBUpdateConnector //CDialogMove
{
// Construction
public:
	CRCDesignParamDlg(CWnd* pParent = NULL);   // standard constructor
	~CRCDesignParamDlg();
	
// interface
public:
	//void SetParamData(T_RPSC_K RpscK);
	//void UpdateView(BOOL bEndI);

// Dialog Data
	//{{AFX_DATA(CRCDesignParamDlg)
	enum { IDD = IDD_DGN_CON_DESIGN_PARAM_BASE };	
	MButton	m_btnAdd;
	MButton m_btnMod;
	MButton	m_btnDel;
	CDlgTabCtrl	m_ctrlTab;  
	CString	m_strDesignCode;
	//}}AFX_DATA
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCDesignParamDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CArray<T_ELEM_K,T_ELEM_K> m_arSelElemK;
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	BOOL m_bGroupMessage;

protected:
	CDBDoc* m_pDoc;

	BOOL m_bModify;

	CRCBeamParameter* m_pBeamDlg;
	CRCColumnParameter* m_pColumnDlg;
	CRCWallParameter* m_pWallDlg;

	void ChangeGroupandLoadCombination();

	void SetRCDesignCodeName();


protected:
	void LayoutChildDlg();
	//void ChangeItem(int nIndex);

	void EnableCalcBtn(bool Enable);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRCDesignParamDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnClose();
	afx_msg void OnDgnModifyConcMateBtn();
	afx_msg void OnDgnModifyMemberTypeBtn();
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDesigncode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__RCDESIGNPARAMDLG_H__)
