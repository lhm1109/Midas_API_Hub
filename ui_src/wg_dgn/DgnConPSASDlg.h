#if !defined(AFX_DgnConPSASDlg_H__E4F533CF_FF79_4848_952D_DE6CE6BCC160__INCLUDED_)
#define AFX_DgnConPSASDlg_H__E4F533CF_FF79_4848_952D_DE6CE6BCC160__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConPSASDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CDgnConPSASDlg dialog

class CDgnConPSASDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConPSASDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

public:
	CString m_strCode;

	void ChangeDlgItemInfo();
	void Initial_Data();
	void Initial_SelectItem();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();
	BOOL Set_DefaultByCode(CString strCode);

// Dialog Data
	//{{AFX_DATA(CDgnConPSASDlg)
	enum { IDD = IDD_DGN_CON_PSAS_DLG };
	int		m_nOption;
	double	m_TranComp;
	double	m_TranSupT;
	double	m_TranMidT;
	double	m_ServSusC;
	double	m_ServTotC;
	double	m_ServTens;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConPSASDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConPSASDlg)
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnDfnstressAr();
	afx_msg void OnDgnDfnstressDel();
	afx_msg void OnDgnPSASUpdateBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnConPSASDlg_H__E4F533CF_FF79_4848_952D_DE6CE6BCC160__INCLUDED_)
