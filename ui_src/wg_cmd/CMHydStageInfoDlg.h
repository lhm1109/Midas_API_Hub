#if !defined(AFX_CMHYDSTAGEINFODLG_H__3847C706_B776_4FF5_A7F5_9E640A990206__INCLUDED_)
#define AFX_CMHYDSTAGEINFODLG_H__3847C706_B776_4FF5_A7F5_9E640A990206__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMHydStageInfoDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CCMHydStageInfoDlg dialog

class CCMHydStageInfoDlg : public CChildDialogMove
{
// Construction
	int m_nStageId;
	T_HSTG_D m_TempStagData;
public:
	CCMHydStageInfoDlg(CWnd* pParent = NULL);   // standard constructor
	
	void SetCurStageData(int nStageId, T_HSTG_D& CurStagData);
	void MakeNodeList();
	void MakeGroupList();
	void MakeBngrList ();
	void MakeLdgrList  ();

// Dialog Data
	//{{AFX_DATA(CCMHydStageInfoDlg)
	enum { IDD = IDD_CMD_DEF_HYD_STAGE_INFO_DLG };
	CListBox	m_wndBngrList;
	CListBox	m_wndGrupList;
	CListBox	m_wndNodeList;
	CListBox	m_wndLdgrList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMHydStageInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMHydStageInfoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMHYDSTAGEINFODLG_H__3847C706_B776_4FF5_A7F5_9E640A990206__INCLUDED_)
