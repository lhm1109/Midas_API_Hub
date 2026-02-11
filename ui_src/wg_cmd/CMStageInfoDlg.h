#if !defined(AFX_CMSTAGEINFODLG_H__C9AD9F3F_DC2C_4777_A1D8_48B3419E4652__INCLUDED_)
#define AFX_CMSTAGEINFODLG_H__C9AD9F3F_DC2C_4777_A1D8_48B3419E4652__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageInfoDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CCMStageInfoDlg dialog

class CCMStageInfoDlg : public CChildDialogMove
{
// Construction
	int m_nStageId;
	T_STAG_D m_TempStagData;
public:
	CCMStageInfoDlg(CWnd* pParent = NULL);   // standard constructor
	void SetCurStageData(int nStageId, T_STAG_D& CurStagData);

	void MakeNodeList();
	void MakeGroupList ();
	void MakeBngrList  ();
	void MakeLdgrList  ();
	//void MakeTendonList();


// Dialog Data
	//{{AFX_DATA(CCMStageInfoDlg)
	enum { IDD = IDD_CMD_DEF_STAGE_INFO_DLG };
	CListBox	m_wndNodeList;
	CListBox	m_wndLdgrList;
	CListBox	m_wndBngrList;
	CListBox	m_wndGrupList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMStageInfoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSTAGEINFODLG_H__C9AD9F3F_DC2C_4777_A1D8_48B3419E4652__INCLUDED_)
