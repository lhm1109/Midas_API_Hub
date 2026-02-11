#pragma once
// SttfListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CSteelTimeTempListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CSteelTimeTempListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CSteelTimeTempListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSteelTimeTempListDlg)
	enum { IDD = IDD_ETC_STTF_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSteelTimeTempListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual void UnitChanged();

// Implementation
protected:
	void SetHeaderTitle(BOOL bInit = FALSE);
	virtual void MakeItemEx(BOOL bUnitChanged = FALSE);

	BOOL InsertItem(T_STTF_K Key, T_STTF_D &Data);
	BOOL DeleteItem(T_STTF_K Key, T_STTF_D &Data);
	BOOL ModifyItem(T_STTF_K KeyOld, T_STTF_K Key, T_STTF_D &Data);
	CString DataToStr(int i, T_STTF_K Key, T_STTF_D &Data);

public:
	void SetSeismicEvaluation(BOOL bSet) { m_bSeismicEvaluation = bSet; }

protected:
	CDBDoc* m_pDoc;
	BOOL m_bSeismicEvaluation;

	// Generated message map functions
	//{{AFX_MSG(CSteelTimeTempListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSttfBtnAdd();
	afx_msg void OnSttfBtnModify();
	afx_msg void OnSttfBtnDelete();
	afx_msg void OnSttfBtnClose();
	afx_msg void OnDblclkSttfList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

