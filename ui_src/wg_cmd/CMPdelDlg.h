#if !defined(__CMPDELDLG_H__)
#define __CMPDELDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMPdelDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxSpfc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\MIT_frx\MEdit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMPdelDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMPdelDlg : public CDialogMove, public CDBUpdateConnector
{
public:
	CCMPdelDlg(CWnd* pParent = NULL); // protected constructor used by dynamic creation
	//DECLARE_DYNCREATE(CCMPdelDlg)
	virtual ~CCMPdelDlg();

// Form Data or Dialog Data
public:
	//{{AFX_DATA(CCMPdelDlg)
	enum { IDD = IDD_DBAR_PDEL_DLG };
	CListCtrl	m_List;
	MEdit	m_wndScaleFactor;
	CSelectLC	m_LoadCase;
	MEdit	m_wndConvTol;
	CFormulaEditSpin	m_wndIterEdit;
	//}}AFX_DATA

// Attributes
public:
	CDBDoc* m_pDoc;
	T_PDEL_D m_Data;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMPdelDlg)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	void SetListCtrlHeader();
	BOOL GetSelectedItem(int &nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_PDEL_BASE &data);
	void SetItem(int nIndex, T_PDEL_BASE &data);
	BOOL ValidItem(T_PDEL_BASE& data);
	BOOL Dlg2Item(T_PDEL_BASE &data);
	BOOL Item2Dlg(T_PDEL_BASE &data);

	// Generated message map functions
	//{{AFX_MSG(CCMPdelDlg)
	afx_msg void OnCmdClose();
	afx_msg void OnCmdApply();
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdRemoveData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMPDELDLG_H__)
