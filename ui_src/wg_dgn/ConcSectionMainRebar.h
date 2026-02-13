#if !defined(__CONCSECTIONMAINREBAR_H__)
#define __CONCSECTIONMAINREBAR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMRebarPscMoment.h : header file
//

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\ChildDialog.h"
#include "..\MIT_frx\MButton.h"
#include "DgnStruct.h"
#include "DgnTabCtrl.h"


class CConcSectionCreateRebar;
class CConcSectionCreateRebar_J;

/////////////////////////////////////////////////////////////////////////////
// CConcSectionMainRebar dialog

class I_DlgViewPanel;
class ConcSectionRebarViewDlg;
using namespace mit::frx;
class CConcSectionMainRebar : public CChildDialog
{
// Construction
public:
	CConcSectionMainRebar(CWnd* pParent = NULL);   // standard constructor

// interface
public:

	CConcSectionCreateRebar* m_pMainRebarI;
	CConcSectionCreateRebar_J* m_pMainRebarJ;

	ConcSectionRebarViewDlg* m_pParent;
	
	void RedrawView();

	BOOL bSameIJ;

	void SetDlgViewPanel(I_DlgViewPanel* pIDVP);

	void AddRebarData(REBAR_KEY RebarKey, T_REBT_MABR MaReberData, BOOL bI_End);
	void DeleteRebarData(REBAR_KEY RebarKey, BOOL bI_End);

	int GetCurSel();

	void RemoveMainRebarDataListI(REBAR_KEY Key, T_REBT_MABR Data);
	void RemoveMainRebarDataListJ(REBAR_KEY Key, T_REBT_MABR Data);

	void SetInitDataI(CArray<T_REBT_MABR,T_REBT_MABR&>& MainRebarDataList);
	void SetInitDataJ(CArray<T_REBT_MABR,T_REBT_MABR&>& MainRebarDataList);

	void SetRebarListDataI(CMap<REBAR_KEY,REBAR_KEY,T_REBT_MABR,T_REBT_MABR>& m_MabrDataList);
	void SetRebarListDataJ(CMap<REBAR_KEY,REBAR_KEY,T_REBT_MABR,T_REBT_MABR>& m_MabrDataList);

	void LoadDlgData(BOOL bSameij);
	void SaveDlgData();

	void InitDialog();

	void SetAsRebarArea();

	//
	CString m_strDgnCode;

// Dialog Data
	//{{AFX_DATA(CConcSectionMainRebar)
	enum { IDD = IDD_DGN_CON_MAIN_REBAR_TAB };
	//CDGNTabCtrl	m_ctrlTab;
	CDlgTabCtrl	m_ctrlTab;
	MButton	m_chkSameIJ;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConcSectionMainRebar)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CMap<REBAR_KEY,REBAR_KEY,T_REBT_MABR,T_REBT_MABR> m_DataI;
	CMap<REBAR_KEY,REBAR_KEY,T_REBT_MABR,T_REBT_MABR> m_DataJ;
	I_DlgViewPanel*  m_pIDVP;
	

	

protected:
	void LayoutChildDlg();

	void Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConcSectionMainRebar)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSameCheck();
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CONCSECTIONMAINREBAR_H__)
