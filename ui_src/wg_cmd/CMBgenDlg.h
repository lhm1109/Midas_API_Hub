#if !defined(AFX_CMBGENDLG_H__9FB0EE81_3D14_11D4_B00F_0000C0B9C58C__INCLUDED_)
#define AFX_CMBGENDLG_H__9FB0EE81_3D14_11D4_B00F_0000C0B9C58C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMlspDlg.h : header file
//

#include "CMDlgBarBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h" // for T_MAKEBUILDING

#include "..\wg_db\wg_db_EditElem.h"
#include "..\wg_db\wg_db_EditNode.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMBgenDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h" // for T_MAKEBUILDING

class CCMBgenDlg : public MChildFormView, public CDBUpdateConnector
{
protected:
	CCMBgenDlg();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCMBgenDlg)
//----------------------------------------------------------------
// Overridables
public:

//----------------------------------------------------------------
// Implementations
protected:
	CArray<T_MAKEBUILDING, T_MAKEBUILDING&> arMakeBuilding;
	BOOL ModifyFlag;

// Form Data
public:
	//{{AFX_DATA(CCMBgenDlg)
	enum { IDD = IDD_DBAR_BGEN_DLG };
	CListCtrl	m_List;
	CMouseEdit	m_wndDistEdit;
	CTextUnit	m_wndDistUnit;
	CEditElem	m_StartElementNumber;
	CEditNode	m_StartNodeNumber;
	CFormulaEditSpin m_edtCopyNum;
	CFormulaEditSpin m_edtMatInc;
	CFormulaEditSpin m_edtColInc;
	CFormulaEditSpin m_edtBeamInc;
	CFormulaEditSpin m_edtBraceInc;
	CFormulaEditSpin m_edtWallInc;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMBgenDlg)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CGXGridWnd m_wndGrid;

	virtual ~CCMBgenDlg();
	void ControlSpin(int i, BOOL Flag);
	void initEditValue();
	void UpdateUnit(); 
	void InitTitle();
	void ModifyTitle();
//  BOOL m_bChangeUnit;
	void InitAttributeControl();
	CDBDoc* m_pDoc;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCMBgenDlg)
	afx_msg void OnTmClose();
	afx_msg void OnDeltaposCmdCopynumSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdMatSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdColSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdBeamSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdBraceSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdWallSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdStartNodeNumBtn();
	afx_msg void OnCmdStartElemNumBtn();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnMerge();
	afx_msg void OnCmdCopynodedata();
	afx_msg void OnCmdCopyelemdata();
	afx_msg void OnTmExecute();
	afx_msg void OnCmdCopybuildingdata();
	afx_msg void OnItemclickCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdBgentableBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMBGENDLG_H__9FB0EE81_3D14_11D4_B00F_0000C0B9C58C__INCLUDED_)
