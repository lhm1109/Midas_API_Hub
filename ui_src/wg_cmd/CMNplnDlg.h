#if !defined(AFX_CMNPLNDLG_H__2B49F742_E174_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMNPLNDLG_H__2B49F742_E174_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMNplnDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\wg_base\wg_base_MouseEdit.h"

#include "CMDlgBarBase.h"
/////////////////////////////////////////////////////////////////////////////
// CCMNplnDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CCMNplnDlg : public CCMDlgBarBase, public CDBUpdateConnector
{
protected:
	CCMNplnDlg();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCMNplnDlg)

// Form Data
public:
	//{{AFX_DATA(CCMNplnDlg)
	enum { IDD = IDD_DBAR_NPLN_DLG };
	CEdit	m_wndPlaneName;
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
	//}}AFX_DATA

// Attributes
public:
	CDBDoc* m_pDoc;

// Operations
public:
	virtual BOOL ExternalInit(UINT key);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMNplnDlg)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	CArray<UINT, UINT> m_aXYZCtrl;
	CArray<UINT, UINT> m_a3PtCtrl;
	virtual ~CCMNplnDlg();
	void ShowCurNpnlData();
	void CtrlMoveDistY(CArray<UINT, UINT> &aCtrlID, int nDistY);
	void CtrlEnableDisable(CArray<UINT,UINT>&CtrlArray,BOOL bIsEnable);  
	void CtrlShowHide(CArray<UINT,UINT>&CtrlArray,BOOL bIsShow);
	void AlignControls();
	void SetListCtrlHeader();
	BOOL GetSelectedItem(int &nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_NPLN_K key, T_NPLN_D &data);
	void SetItem(int nIndex, T_NPLN_K Key, T_NPLN_D &Data);
	void InsertItem(T_NPLN_K Key, T_NPLN_D &Data);
	void DeleteItem(T_NPLN_K Key, T_NPLN_D &Data);
	BOOL GetPosition(CString& strPos, double& Px ,double& Py ,double& Pz);
	BOOL Dlg2Item(T_NPLN_D &Data);
	BOOL Item2Dlg(T_NPLN_D &Data);

	void SelectListItem(CString& csPlaneName);
	void SelectListItem(int nIndex);

	void ChangePlaneType(UINT nID);

	static int CALLBACK CompareByPlaneName(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCMNplnDlg)
	afx_msg void OnDlgClose();
	afx_msg void OnChangePlaneType();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickCmdNplnList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCmdNplnList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMNPLNDLG_H__2B49F742_E174_11D3_92DE_0000C0B0E6B3__INCLUDED_)
