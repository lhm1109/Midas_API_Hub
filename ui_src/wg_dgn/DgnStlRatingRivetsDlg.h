// DgnStlRatingRivetsDlg.h: interface for the CDgnStlRatingRivetsDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DgnStlRatingRivetsDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_DgnStlRatingRivetsDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "DgnStlRatingRivetsTabDlg.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "DgnTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\MIT_frx\MButton.h"

class CDBDoc;

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CDgnStlRatingRivetsTabDlg;
class __MY_EXT_CLASS__ CDgnStlRatingRivetsDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CDgnStlRatingRivetsDlg(CWnd* pParent = NULL);

	// Dialog Data
		//{{AFX_DATA(CDgnStlRatingRivetsDlg)
	enum { IDD = IDD_DGN_STL_RATING_RIVETS_DLG };
	
	int		m_nOption;
	mit::frx::MButton m_Ctrl_BothIJ;
	CDlgTabCtrl	m_Tab;
	//}}AFX_DATA

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDgnStlRatingRivetsDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	// Implementation
protected:
	CDBDoc* m_pDoc;

	void Initial_SelectItem();
	BOOL Dlg2Data();

	void EnableDisableControls();

	T_RIVT_D m_Data;

	T_RIVT_SUB_D m_DataI;
	T_RIVT_SUB_D m_DataJ;

	BOOL m_bBothIJ;

	CDgnStlRatingRivetsTabDlg* m_pSubDlg_I;
	CDgnStlRatingRivetsTabDlg* m_pSubDlg_J;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlRatingRivetsDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnOption();
	afx_msg void OnDgnBothIJChk();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnStlRatingRivetsDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
