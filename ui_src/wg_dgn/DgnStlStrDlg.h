#if !defined(AFX_DGNSTLSTRDLG_H__147352C5_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNSTLSTRDLG_H__147352C5_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlStrDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnStlStrDlg dialog

class CDgnStlStrDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnStlStrDlg(CWnd* pParent = NULL);   // standard constructor

public:
	void Initial_Data();
	void Initial_SelectItem();
	// Add by ZINU.('01.1.3).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();

public:

// Dialog Data
	//{{AFX_DATA(CDgnStlStrDlg)
	enum { IDD = IDD_DGN_STL_STRR_DLG };
	int		m_nOption;
	double	m_Fa;
	double	m_Fby;
	double	m_Fbz;
	double	m_Ft;
	double	m_Fv;
	MButton m_wndUser_Fa;
	MButton m_wndUser_Fby;
	MButton m_wndUser_Fbz;
	MButton m_wndUser_Ft;
	MButton m_wndUser_Fv;
	double  m_StLtRatio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlStrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlStrDlg)
	virtual void Execute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnDfnstressAr();
	afx_msg void OnDgnDfnstressDel();
	afx_msg void OnDgnUserInputStress();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSTLSTRDLG_H__147352C5_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
