#if !defined(__MVLDITEMJPDLG_H__)
#define __MVLDITEMJPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemJPDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemJPDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMMvldItemJPDlg : public CDialogMove
{
// Construction
public:
	CCMMvldItemJPDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMMvldItemJPDlg();   

public:
	void SetParamData(T_MVLDjp_D &ParamData) 
	{ m_csOldName = ParamData.LoadCaseName; m_Data = ParamData; m_bModify = TRUE; }

protected:
	BOOL ApplyOrOK();
	void UpdateCtrls();
	void SetData2Dlg();
	BOOL SetDlg2Data();
	void SetData2ChildDlg();
	BOOL SetChildDlg2Data();

	void CreateChildDlg();
	void DestroyChildDlg();
	BOOL ShowChildDialog();

protected:
	CDBDoc* m_pDoc;
	T_MVLDjp_D  m_Data;

	BOOL m_bModify;
	int m_nCurLoadType;
	CString   m_csOldName;
	CArray<CChildDialog*, CChildDialog*> m_pChildDlgs;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvldItemJPDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_JAPAN };
	CBCGPStatic	m_wndHolder;
	CEditUnit	m_editUser2;
	CEditUnit	m_editUser1;
	int		m_nImpact;
	int		m_nLoadType;
	BOOL	m_bImpact;
	CString	m_strLoadCase;
	CString	m_strDescription;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemJPDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemJPDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdApply();
	afx_msg void OnImpactRadio4();
	afx_msg void OnImpactCheck();
	afx_msg void OnLoadTypeRadio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	 CArray<UINT, UINT> arImpactGroup;
	 CArray<UINT, UINT> arUserRdGroup;
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVLDITEMJPDLG_H__1A99444E_3B2C_45EE_A0ED_CCC11833F6FE__INCLUDED_)
