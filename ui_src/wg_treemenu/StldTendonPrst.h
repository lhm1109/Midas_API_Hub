#if !defined(AFX_STLDTENDONPRST_H__2A9290A8_8CC4_44B6_866F_043D08998EB3__INCLUDED_)
#define AFX_STLDTENDONPRST_H__2A9290A8_8CC4_44B6_866F_043D08998EB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_CobxLdgr.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_NotifyListCtrl.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CStldTendonPrst dialog

class CStldTendonPrst : public CMenuBarChildDlg , public CDBUpdateConnector
{
// Construction
public:
	CDBDoc* m_pDoc;
	CStldTendonPrst(CWnd* pParent = NULL);   // standard constructor
	~CStldTendonPrst();
	void MakeListHeader();
	virtual void Execute(){};


	void		Data2Dlg(T_TDPL_K  key );
	void		Dlg2Data(T_TDPL_D& data);
	void    UpdateTendonList();
	void    UpdateRightTendonList();
	void    UpdateTdplList();
	void    DisplayCurSel();

	void    GetTdplListFromDBDoc(CArray<T_TDPL_K,T_TDPL_K>*pTdplKeys,
															 CArray<T_TDPL_D,T_TDPL_D>*pTdplDatas);
	
	
	virtual	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	LRESULT OnListCtrlKeyUp(WPARAM wParam, LPARAM lParam);
	void StressOptMan(int nStressOpt);
	public:
	CArray<T_TDPL_K, T_TDPL_K> m_aKeyTdpl;

	int m_n1stJackEnd;
// Dialog Data
	//{{AFX_DATA(CStldTendonPrst)
	enum { IDD = IDD_TM_STLD_ZASSIGN_TENDON_PRST };
	MComboBox	m_wnd1stJackCombo;
	CListCtrl	m_wndRList;
	CListCtrl	m_wndLList;
	CCobxLdgr	m_CobxLoadGroup;
	CSelectLC	m_CobxLoadCase ;
	CEditUnit	m_wndStreeBegin;
	CEditUnit	m_wndStressEnd;
	CTextUnit	m_wndStressUnit1;
	CTextUnit	m_wndStressUnit2;
	CNotifyListCtrl	m_wndTdplList;
	CFormulaEditSpin m_editGrouting;
	int		m_nStressOpt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldTendonPrst)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStldTendonPrst)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmBtnLoadCase();
	afx_msg void OnTmBtnLoadGroup();
	afx_msg void OnTmTprstLeftButton();
	afx_msg void OnTmTprstRightButton();
	afx_msg void OnTmTprstAddBtn();
	afx_msg void OnTmTprstDelBtn();
	afx_msg void OnTmTprstModyBtn();
	afx_msg void OnClickTmTprstList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmClose();
	afx_msg void OnTmTprstStressChk();
	afx_msg void OnTmTprstForceChk();
	afx_msg void OnDblclkTmTprstLlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTmTprstRlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTm1stjackCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDTENDONPRST_H__2A9290A8_8CC4_44B6_866F_043D08998EB3__INCLUDED_)
