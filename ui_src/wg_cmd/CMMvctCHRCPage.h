#if !defined(__CMMVCTCHRCPAGE_H__)
#define __CMMVCTCHRCPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctCHRCPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHRCPage dialog
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CCMMvctCHRCPage : public CInternationalDlg
{
// Construction
public:
	CCMMvctCHRCPage(T_BRDG_RC* pData, CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CCMMvctCHRCPage)
	enum { IDD = IDD_CMD_ML_MVCT_CHINA_RC };
	
	CEditUnit m_edtCase1L1;
	CEditUnit m_edtCase1F1;
	CEditUnit m_edtCase1L2;
	CEditUnit m_edtCase1F2;
	BOOL      m_bCase2; 
	CEditUnit m_edtCase2L1;
	CEditUnit m_edtCase2F1;
	CEditUnit m_edtCase2L2;
	CEditUnit m_edtCase2F2;
	MComboBox m_cmbGrup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctCHRCPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	BOOL SetDlg2Data();
	void EnableDisableControls(BOOL bImpactFactor);

// Implementation
protected:
	void InitUnit();
	void InitCombo();
	
	CDBDoc* m_pDoc;

	T_BRDG_RC* m_pData;

	BOOL m_bImpactFactor;
	
	// Generated message map functions
	//{{AFX_MSG(CCMMvctCHRCPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdMvchCHRCCase2Chk();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVCTCHRCPAGE_H__)
