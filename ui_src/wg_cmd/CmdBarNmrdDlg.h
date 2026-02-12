#if !defined(AFX_CMDBARNMRDDLG_H__7198D5EA_44BF_4D78_9A20_443261192AAC__INCLUDED_)
#define AFX_CMDBARNMRDDLG_H__7198D5EA_44BF_4D78_9A20_443261192AAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdBarNmrdDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCmdBarNmrdDlg dialog

#include "CMDlgBarBase.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\wg_db_CobxGrup.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\mit_frx\MComboBox.h"

class CCmdBarNmrdDlg : public CCMDlgBarBase, public CDBUpdateConnector
{
// Construction
public:
	CCmdBarNmrdDlg();   // standard constructor
	DECLARE_DYNCREATE(CCmdBarNmrdDlg)

// Dialog Data
	//{{AFX_DATA(CCmdBarNmrdDlg)
	enum { IDD = IDD_DBAR_NMRD_DLG };
	CListCtrl	   m_lstSupport;
	CSelectEdit	 m_edtNodeList;
	CTextUnit	   m_stcLength;
	CListBox	   m_lstLdcTo;
	CListBox	   m_lstLdcFrom;
	mit::frx::MComboBox   m_cbxGrup;
	int		       m_nAutoUser;
	CString	     m_csUserWidth;
	
	//}}AFX_DATA

public:

protected:
	void  MakeListHead();
	void  MakeListData();
	void  MakeLoadList();
	void  MakeGrupData();
	void  MakeListBoxHorzScroll();
	void  GetSelectedNode(CArray<UINT,UINT> &arNodeList);
	CString GetLoadCaseName(CString sLoadName,UINT nType);
	void  GetTotalLoadCase(T_SELECTEDLOAD_LIST &aLoadList);
	UINT  GetDBLcomType(UINT nSelLcomType);
private:
	T_NMRD_D m_data; 
	CDBDoc *m_pDoc;
	CArray<UINT,UINT> m_arLTypeFrom;
	CArray<UINT,UINT> m_arLTypeTo;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdBarNmrdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL UpdateBuffer();
	virtual void OnTmClose();
	virtual void OnTmExecute();
	// Generated message map functions
	//{{AFX_MSG(CCmdBarNmrdDlg)
	virtual void OnInitialUpdate();
	afx_msg void OnNmrdOk();
	afx_msg void OnNmrdDel();
	afx_msg void OnNmrdCancel();
	afx_msg void OnNmrdAddrep();
	afx_msg void OnNmrdAutoUserRadio();
	afx_msg void OnNmrdLdcselBtn();
	afx_msg void OnNmrdLdcunselBtn();
	afx_msg void OnSelchangeNmrdGdgrupCbx();
	afx_msg void OnNmrdGdgrupDet();
	afx_msg void OnNmrdDeleteData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDBARNMRDDLG_H__7198D5EA_44BF_4D78_9A20_443261192AAC__INCLUDED_)
