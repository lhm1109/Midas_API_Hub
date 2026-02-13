#if !defined(AFX_DGNCONJSFTDLG_H__0D76EED1_B604_49BD_9DA0_36810BC5F61D__INCLUDED_)
#define AFX_DGNCONJSFTDLG_H__0D76EED1_B604_49BD_9DA0_36810BC5F61D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConJSFTDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CDgnConJSFTDlg dialog

class CDgnConJSFTDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConJSFTDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

public:
	void Initial_SelectItem();
	void Initial_Data();
	// Add by ZINU.('01.1.3).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();

// Dialog Data
	//{{AFX_DATA(CDgnConJSFTDlg)
	enum { IDD = IDD_DGN_CON_JSFT_DLG };
	int		m_nOption;
	double	m_dJackFactor;
	BOOL	m_bAuto;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConJSFTDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnJSFTAr();
	afx_msg void OnDgnJSFTDel();
	afx_msg void OnDgnJSFTcalProgram();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConJSFTDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONJSFTDLG_H__0D76EED1_B604_49BD_9DA0_36810BC5F61D__INCLUDED_)
