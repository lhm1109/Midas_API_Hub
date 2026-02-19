#if !defined(__DGNCONEXPOSURECONDIDLG_H__)
#define __DGNCONEXPOSURECONDIDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConExposureCondiDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnConExposureCondiDlg dialog
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;

class CDgnConExposureCondiDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConExposureCondiDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	void Update_InitDataByCode();

// Dialog Data
	//{{AFX_DATA(CDgnConExposureCondiDlg)
	enum { IDD = IDD_DGN_CON_EXPOSURE_CONDI_DLG };

	int		m_nOption;
	int   m_nExposure;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConExposureCondiDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConExposureCondiDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNCONEXPOSURECONDIDLG_H__)
