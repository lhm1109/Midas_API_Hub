#if !defined(AFX_STTLLOADCASEDLG_H__93312C03_6EBB_11D4_9AAF_0000C0B9C58C__INCLUDED_)
#define AFX_STTLLOADCASEDLG_H__93312C03_6EBB_11D4_9AAF_0000C0B9C58C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SttlLoadCaseDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

#include "..\MIT_frx\MEdit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CSttlLoadCaseDlg dialog

class CSttlLoadCaseDlg : public CMenuBarChildDlg ,public CDBUpdateConnector
{
// Construction
public:
	CSttlLoadCaseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { };
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CSttlLoadCaseDlg)
	enum { IDD = IDD_TM_STTL_LOADCASE };
	CListCtrl	m_List;
	MEdit	m_edtDesc;
	MEdit	m_edtScaleFactor;
	CFormulaEditSpin	m_edtSMax;
	CFormulaEditSpin	m_edtSMin;
	CListBox	m_lstSelGroup;
	CListBox	m_lstSetGroup;
	MEdit	m_edtLoadCaseName;
	//}}AFX_DATA


// Overrides
	virtual	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSttlLoadCaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	T_SMPT_K m_SmptKey;	
	T_SMPT_D m_SmptData;

	T_SMLC_K m_SmlcKey;
	T_SMLC_D m_SmlcData;
	T_SMLC_DT m_SmlcDataList;

	void InitListTitle();
	void InitSmlcData();
	void InitSmptData();
	void CtrlMinMaxValue();
	void ResetSmptData();
	void ResetSmptSelData();
	int GetCurrentRow();

	T_SMLC_K m_SmlcKeyBak;
	T_SMLC_D m_SmlcDataBak;

	int GetCurrentSmlcKey();
	CString GetCurrentSmlcKeyName();
	// Generated message map functions
	//{{AFX_MSG(CSttlLoadCaseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmSttlLcaseAdd();
	afx_msg void OnTmSttlLcaseModify();
	afx_msg void OnTmSttlLcaseDelete();
	afx_msg void OnItemchangedTmSttlLcaseList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmSttlGroupClose();
	afx_msg void OnTmSttlLcaseSelBtn();
	afx_msg void OnTmSttlLcaseRemBtn();
	afx_msg void OnDeltaposTmSttlLcaseSminSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposTmSttlLcaseSmaxSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTmSttlLcaseSelList();
	afx_msg void OnDblclkTmSttlLcaseSetList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STTLLOADCASEDLG_H__93312C03_6EBB_11D4_9AAF_0000C0B9C58C__INCLUDED_)
