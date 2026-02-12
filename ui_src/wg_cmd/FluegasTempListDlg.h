#pragma once
// FlueListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CFlueListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CFlueListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CFlueListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFlueListDlg)
	enum { IDD = IDD_ETC_FLUE_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlueListDlg)
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

	BOOL InsertItem(T_FLUE_K Key, T_FLUE_D &Data);
	BOOL DeleteItem(T_FLUE_K Key, T_FLUE_D &Data);
	BOOL ModifyItem(T_FLUE_K KeyOld, T_FLUE_K Key, T_FLUE_D &Data);
	CString DataToStr(int i, T_FLUE_K Key, T_FLUE_D &Data);

public:
	void SetSeismicEvaluation(BOOL bSet) { m_bSeismicEvaluation = bSet; }

protected:
	CDBDoc* m_pDoc;
	BOOL m_bSeismicEvaluation;

	// Generated message map functions
	//{{AFX_MSG(CFlueListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnFlueBtnAdd();
	afx_msg void OnFlueBtnModify();
	afx_msg void OnFlueBtnDelete();
	afx_msg void OnFlueBtnClose();
	afx_msg void OnDblclkFlueList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

