#if !defined(__ELEMWALLREDUFACTORDLG_H__)
#define      __ELEMWALLREDUFACTORDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemWallReduFactorDlg.h : header file
//

#include "..\wg_db\wg_db_CobxBngr.h"

/////////////////////////////////////////////////////////////////////////////
// CElemWallReduFactorDlg dialog

class CElemWallReduFactorDlg : public CMenuBarChildDlg
{
// Construction
public:
	CElemWallReduFactorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CElemWallReduFactorDlg)
	enum { IDD = IDD_TM_ELEM_WALL_REDU_FACTOR };
	CCobxBngr	m_wndGroupCombo;
	int		m_nAddOrDel;
	CEdit m_wndShear;
	CEdit m_wndBending;

	CEdit m_wndCRBShear;
	CEdit m_wndCRBBending;
	CEdit m_wndCRBAxial;
	CEdit m_wndOutTorsionBending;
	CEdit m_wndOutShear;
	CEdit m_wndOutBending;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemWallReduFactorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT, UINT> m_WallPlate;
	CArray<UINT, UINT> m_WallCRB;
	CArray<UINT, UINT> m_WallType;
	CArray<UINT, UINT> m_WallType2;
	CArray<UINT, UINT> m_OutofPlane;

	void AlignControls();  

	// Generated message map functions
	//{{AFX_MSG(CElemWallReduFactorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmOption();
	afx_msg void OnTmDefineGroupButton();
	afx_msg void OnTmWallType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ELEMWALLREDUFACTORDLG_H__)
