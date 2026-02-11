#if !defined(__CMMVLDITEMKRLRFDDLG_H__)
#define __CMMVLDITEMKRLRFDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemKRLRFD11Dlg.h : header file
//

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

#define COLCOUNT 8

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemKRLRFD11Dlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvldItemKRLRFD11Dlg : public CDialogMove
{
// Construction
public:
	CCMMvldItemKRLRFD11Dlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVLD_D &ParamData) 
	{ m_csOldName = ParamData.LoadCaseName; m_Data = ParamData; m_bModify = TRUE; }
	BOOL OnApplySubDlg(BOOL bModify, T_MVLD_BASE& data, int nPos);

//----------------------------------------------------------------------
// Implementation
protected:
	void InitUnit();

	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(int nIndex, T_MVLD_BASE &Data);
	BOOL DeleteItem(int nIndex, T_MVLD_BASE &Data);
	BOOL ModifyItem(int nIndex, T_MVLD_BASE &Data);
	CString DataToStr(int i, T_MVLD_BASE &Data);
	CString GetLaneName(UINT nLaneKey);

	void Data2Dlg();
	BOOL Dlg2Data();

	BOOL ApplyOrOK();

	CDBDoc*   m_pDoc;
	T_MVLD_D  m_Data;
	CString   m_csOldName;
	BOOL      m_bModify;
	BOOL      m_bSurface;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvldItemKRLRFD11Dlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_KRLRFD11 };
	
	CEditUnit	m_edtLoadCase;
	CEditUnit	m_edtDesc;
	CEditUnit	m_edtSF1;
	CEditUnit	m_edtSF2;
	CEditUnit	m_edtSF3;
	CEditUnit	m_edtSF4;
	CEditUnit	m_edtSF5;
	int       m_nCombOpt;
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemKRLRFD11Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemKRLRFD11Dlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVLDITEMKRLRFDDLG_H__)
