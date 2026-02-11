#if !defined(__CMBUCKDLG_H__)
#define __CMBUCKDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMBuckDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxSpfc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

#include "CMDlgBarBase.h"
/////////////////////////////////////////////////////////////////////////////
// CCMBuckDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMBuckDlg : public CDialogMove, public CDBUpdateConnector
{
public:
	CCMBuckDlg(CWnd* pParent = NULL); // protected constructor used by dynamic creation
	virtual ~CCMBuckDlg();
	//DECLARE_DYNCREATE(CCMBuckDlg)

// Form Data
public:
	//{{AFX_DATA(CCMBuckDlg)
	enum { IDD = IDD_DBAR_BUCK_DLG };
	CSelectLC	m_LoadCase;
	CListCtrl	m_List;
	MEdit	m_wndScaleFactor;
	MEdit	m_wndConvTol;
	CFormulaEditSpin 	m_wndIterEdit;
	CFormulaEditSpin 	m_wndBuckNumEdit;
	int   m_nLoadType;
	BOOL  m_bPositive;
	MButton  m_bAxialForce;
	MButton  m_bSturm;
	CEditUnit m_wndSearchFrom;
	CEditUnit m_wndSearchTo;
	//}}AFX_DATA

// Attributes
public:
	CDBDoc* m_pDoc;
	T_BUCK_D m_Data;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMBuckDlg)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	CArray<UINT, UINT> m_aLoadFactor;
	CArray<UINT, UINT> m_aBuckComb;

protected:
	void SetListCtrlHeader();
	BOOL GetSelectedItem(int &nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_BUCK_BASE &data);
	void SetItem(int nIndex, T_BUCK_BASE &data);
	BOOL ValidItem(T_BUCK_BASE& data);
	BOOL Dlg2Item(T_BUCK_BASE &data);
	BOOL Item2Dlg(T_BUCK_BASE &data);
	void EnableDisableCtrl();

	void Data2Dlg();
	void Dlg2Data();

	// Generated message map functions
	//{{AFX_MSG(CCMBuckDlg)
	afx_msg void OnCmdApply();
	afx_msg void OnCmdClose();
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdRemoveData();
	afx_msg void OnCmdLoadFactor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMBUCKDLG_H__)
