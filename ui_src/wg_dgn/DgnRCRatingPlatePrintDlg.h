#if !defined(_DGNRCRATINGPLATEPRINTDLG_H__)
#define _DGNRCRATINGPLATEPRINTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRCRatingPlatePrintDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\MIT_frx\MComboBox.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CDgnRCRatingPlatePrintDlg dialog

class CDgnRCRatingPlatePrintDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnRCRatingPlatePrintDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnRCRatingPlatePrintDlg)
	enum { IDD = IDD_DGN_RATING_RC_PLATE_PRINT_OPTION_DLG };

	int	    	m_nOption;
	mit::frx::MComboBox m_cmbMemb;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRCRatingPlatePrintDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	
	void InitCombo();
	BOOL Dlg2Data(T_RPRP_K &rKey, T_RPRP_D &rData);
	
	// Generated message map functions
	//{{AFX_MSG(CDgnRCRatingPlatePrintDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
// 	afx_msg void OnDgnExecute();	
// 	afx_msg void OnDgnClose();
	afx_msg void OnDgnMembCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_DGNRCRATINGPLATEPRINTDLG_H__)
