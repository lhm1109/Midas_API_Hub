#if !defined(__DGNCONBARSECTDLG_H__)
#define __DGNCONBARSECTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RatingCaseDlg.h : header file
//

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "DgnTabCtrl.h"

#include "DgnConBeamBarSectDlg.h"
#include "DgnConColumnBarSectDlg.h"
#include "DgnConBraceBarSectDlg.h"
#include "DgnConWallBarSectDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CDgnConBarSectDlg dialog
#define BEAM_TAB   0
#define COLUMN_TAB 1
#define BRACE_TAB  2
#define WALL_TAB   3

#include "HeaderPre.h"


class CDgnConBarSectDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConBarSectDlg(CWnd* pParent = NULL);   // standard constructor
	
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual void Execute() { OnDgnApply(); }
	void Initial_Data();
	void Update_InitDataByCode();

	void SetTabOrder(int iTab);
	void ChangeCurrentTab(int nTabIndex);
// Dialog Data
	//{{AFX_DATA(CDgnConBarSectDlg)
	enum { IDD = IDD_DGN_CON_BAR_SECT_DLG };
	CDlgTabCtrl	m_Tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBarSectDlg)

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnApply();
	afx_msg void OnDgnClose();
	//}}AFX_VIRTUAL

// Implementation
protected:

	CDBDoc* m_pDoc;

	int m_nCurTab;  //현재 활성화된 텝
	CDgnConBeamBarSectDlg* m_pSubDlg_Beam;
	CDgnConColumnBarSectDlg* m_pSubDlg_Column;
	CDgnConBraceBarSectDlg* m_pSubDlg_Brace;
	CDgnConWallBarSectDlg* m_pSubDlg_Wall;

	void InitTabControl();

	void InitChildAIJCtrl();

	void SetRebarCombo();
	void SetInitData();
	
	// Generated message map functions
	//{{AFX_MSG(CDgnConBarSectDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNCONBARSECTDLG_H__)
