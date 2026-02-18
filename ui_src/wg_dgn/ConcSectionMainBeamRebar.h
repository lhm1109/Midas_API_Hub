#if !defined(__CONCSECTIONMAINBEAMREBAR_H__)
#define __CONCSECTIONMAINBEAMREBAR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\ChildDialog.h"
#include "..\MIT_frx\MButton.h"
#include "DgnStruct.h"

/////////////////////////////////////////////////////////////////////////////
// CConcSectionMainBeamRebar dialog

class I_DlgViewPanel;
class CConcSectionBeamRebarViewDlg;
class CConcSectionCreateBeamRebar;
using namespace mit::frx;
class CConcSectionMainBeamRebar : public CChildDialog
{
// Construction
public:
	CConcSectionMainBeamRebar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConcSectionMainBeamRebar)
	enum { IDD = IDD_DGN_CON_MAIN_BEAM_REBAR_TAB };
	CDlgTabCtrl	m_ctrlTab;
	MButton	m_chkSameIJ;	
	//}}AFX_DATA

// interface
public:

	CConcSectionCreateBeamRebar* m_pMainRebarI;
	CConcSectionCreateBeamRebar* m_pMainRebarM;
	CConcSectionCreateBeamRebar* m_pMainRebarJ;

	CConcSectionBeamRebarViewDlg* m_pParent;

	BOOL m_bSameIMJ;

	void SetDlgViewPanel(I_DlgViewPanel* pIDVP);

	void AddRebarData(REBAR_KEY RebarKey, T_RBGC_MABR MaReberData, int nPosition);
	void DeleteRebarData(REBAR_KEY RebarKey, int nPosition);

	int GetCurSel();

	void RemoveMainRebarDataListI(REBAR_KEY Key, T_RBGC_MABR Data);
	void RemoveMainRebarDataListM(REBAR_KEY Key, T_RBGC_MABR Data);
	void RemoveMainRebarDataListJ(REBAR_KEY Key, T_RBGC_MABR Data);

	void SetInitDataI(CArray<T_RBGC_MABR,T_RBGC_MABR&>& MainRebarDataList);
	void SetInitDataM(CArray<T_RBGC_MABR,T_RBGC_MABR&>& MainRebarDataList);
	void SetInitDataJ(CArray<T_RBGC_MABR,T_RBGC_MABR&>& MainRebarDataList);

	void SetRebarListDataI(CMap<REBAR_KEY,REBAR_KEY,T_RBGC_MABR,T_RBGC_MABR>& m_MabrDataList);
	void SetRebarListDataM(CMap<REBAR_KEY,REBAR_KEY,T_RBGC_MABR,T_RBGC_MABR>& m_MabrDataList);
	void SetRebarListDataJ(CMap<REBAR_KEY,REBAR_KEY,T_RBGC_MABR,T_RBGC_MABR>& m_MabrDataList);

	void LoadDlgData(BOOL bSameIMJ);
	void SaveDlgData();

	void SetAsRebarArea();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConcSectionMainBeamRebar)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	I_DlgViewPanel*  m_pIDVP;  

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConcSectionMainBeamRebar)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSameCheck();
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CONCSECTIONMAINBEAMREBAR_H__)
