#if !defined(AFX_CMTHCRDLG_H__INCLUDED_)
#define AFX_CMTHCRDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThcrDlg.h : header file
//
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_db\wg_db_selectctrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\MIT_frx\MComboBox.h"

#include "CMChildBarBase.h"

/////////////////////////////////////////////////////////////////////////////
// CCMThcrDlg dialog

class CCMThcrDlg : public CCMChildBarBase, public CDBUpdateConnector
{
// Construction
public:
	CCMThcrDlg(CWnd* pParent = NULL);   // standard constructor

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual void Execute(){ OnThcrAdd(); }
	virtual BOOL ExternalInit(UINT key);
// Dialog Data
	//{{AFX_DATA(CCMThcrDlg)
	enum { IDD = IDD_DBAR_THCR_DLG };

	CListCtrl	m_ctrlList;
	mit::frx::MEdit m_edtName;

	CMouseEdit	m_edtMasterNode;
	CSelectEdit m_edtSelectSubNode;

	mit::frx::MComboBox	m_CompCb;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThcrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	void InitCtrl();
	void MakeListHeader();

	BOOL Dlg2Data();
	void Data2Dlg();

	void MakeItemEx();
	void SetItem(int nIndex, T_THCR_K ThcrK, T_THCR_D& ThcrD);
	CString DataToStr(int nIndex, T_THCR_D& ThcrD);
	BOOL GetSelectedIndex(int &nIndex);

	int m_nOutType;
	T_THCR_D m_Data;

// Generated message map functions
	//{{AFX_MSG(CCMThcrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnThcrAdd();
	afx_msg void OnThcrModify();
	afx_msg void OnThcrDelete();
	afx_msg void OnClose();
	afx_msg void OnListItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHCRDLG_H__INCLUDED_)
