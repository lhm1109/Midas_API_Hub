#if !defined(AFX_CMENVLITEMDLG_H__BDF960E4_28D1_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMENVLITEMDLG_H__BDF960E4_28D1_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMEnvlItemDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMEnvlItemDlg dialog

class CCMEnvlItemDlg : public CDialogMove
{
// Construction
public:
	CCMEnvlItemDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_ENVL_D &ParamData) 
	{ m_csOldName = ParamData.EnvelopeName; m_Data = ParamData; m_bModify = TRUE; }

//----------------------------------------------------------------------
// Implementation
protected:
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL ApplyOrOK();
	void SetHeaderTitle();
	void MakeItemEx(BOOL bUnsel);

	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);
	void MakeLoadComb();

protected:
	CDBDoc*   m_pDoc;
	T_ENVL_D  m_Data;
	CString   m_csOldName;
	BOOL      m_bModify;

	CArray<BOOL, BOOL> m_aSelFlag;
	CArray<CString, CString&> m_aName;
	CArray<UINT, UINT> m_aKey;
	CArray<int, int> m_aAnalType; // D_LCOM_????

public:
// Dialog Data
	//{{AFX_DATA(CCMEnvlItemDlg)
	enum { IDD = IDD_CMD_ENVL_ITEM };
	CEdit	m_wndDesc;
	CEdit	m_wndName;
	CListCtrl	m_SelList;
	CListCtrl	m_UnselList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMEnvlItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMEnvlItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdApply();
	virtual void OnCancel();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCmdList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMENVLITEMDLG_H__BDF960E4_28D1_11D4_92DE_0000C0B0E6B3__INCLUDED_)
