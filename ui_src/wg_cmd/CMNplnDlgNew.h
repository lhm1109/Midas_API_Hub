#if !defined(AFX_CMNPLNDLGNEW_H__9BE2BB6A_DB51_439B_AAEB_19A1302896A7__INCLUDED_)
#define AFX_CMNPLNDLGNEW_H__9BE2BB6A_DB51_439B_AAEB_19A1302896A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMNplnDlgNew.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\mit_frx\MEdit.h"
#include "..\wg_base\MChildFormView.h"

#include "CMDlgBarBase.h"
/////////////////////////////////////////////////////////////////////////////
// CCMNplnDlgNew form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CCMNplnDlgNew : public MChildFormView, public CDBUpdateConnector
{
protected:
	CCMNplnDlgNew();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCMNplnDlgNew)

// Form Data
public:
	//{{AFX_DATA(CCMNplnDlgNew)
	enum { IDD = IDD_DBAR_NPLN_DLG_NEW };
	mit::frx::MEdit	m_wndPlaneName;
	CListCtrl	m_List;
	CBCGPStatic	m_wndXYZTitle;
	CTextUnit	m_wndXYZTolUnit;
	CEditUnit	m_wndXYZTol;
	CTextUnit	m_wndXYZUnit;
	CMouseEdit	m_wndXYZ;
	CTextUnit	m_wndTolUnit;
	CEditUnit	m_wndTol;
	CTextUnit	m_wnd3rdUnit;
	CTextUnit	m_wnd2ndUnit;
	CTextUnit	m_wnd1stUnit;
	CMouseEdit	m_wnd3rdPt;
	CMouseEdit	m_wnd2ndPt;
	CMouseEdit	m_wnd1stPt;
	int		m_nPlaneType;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CArray<UINT, UINT> m_aXYZCtrl;
	CArray<UINT, UINT> m_a3PtCtrl;
// Operations
public:
	void MakeListHeader();
	void MakeListContents();
	CString DataToStr(int i, T_NPLN_K key, T_NPLN_D &data);
	void GetDataStringArr(T_NPLN_K key,CStringArray &arStr);
	void AlignControls();
	void InitCtrls();
	void PlaneTypeCtrlMan(int nPlaneType);
	void SetNplnData   (T_NPLN_K Key);
	BOOL GetCurNplnData(T_NPLN_D &Data);
	void SelectListItem(CString& csPlaneName);
	void SelectListItem(int nIndex);
	BOOL GetSelectedItem(int &nIndex);

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMNplnDlgNew)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCMNplnDlgNew();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual bool ShowApplyBtn() { return false; }
	// Generated message map functions
	//{{AFX_MSG(CCMNplnDlgNew)
	afx_msg void OnPlanTypeChange();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnItemchangedCmdNplnList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnTmClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedCmdTolTitle();
	afx_msg void OnEnChangeCmdTolEdit();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMNPLNDLGNEW_H__9BE2BB6A_DB51_439B_AAEB_19A1302896A7__INCLUDED_)
