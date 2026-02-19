#pragma once

#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "HeaderPre.h"

/* Russian  SP_63_2018 : Seismic accounting for member (RC) */
class CDBDoc;

class __MY_EXT_CLASS__ CDgnConSafmDlg : public CMenuBarChildDlg
{
	// Construction
public:
	CDgnConSafmDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(CDgnConSafmDlg)
	enum { IDD = IDD_DGN_CON_SAFM_DLG };

public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDgnConSafmDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConSafmDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int	m_nOption;
	double m_dMtr1;
	double m_dMtr2;
};

#include "HeaderPost.h"
