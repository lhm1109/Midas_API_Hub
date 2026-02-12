#if !defined(__CMMVCTCHSTLPAGE_H__)
#define __CMMVCTCHSTLPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctCHStlPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHStlPage dialog
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CCMMvctCHStlPage : public CInternationalDlg
{
// Construction
public:
	CCMMvctCHStlPage(T_BRDG_STL* pData, CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CCMMvctCHStlPage)
	enum { IDD = IDD_CMD_ML_MVCT_CHINA_STL };
	
	CEditUnit m_edtCase1_U;
	CEditUnit m_edtCase1_L;
	BOOL      m_bCase2; 
	CEditUnit m_edtCase2_U;
	CEditUnit m_edtCase2_L;
	MComboBox m_cmbGrup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctCHStlPage)
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

	T_BRDG_STL* m_pData;

	BOOL m_bImpactFactor;
	
	// Generated message map functions
	//{{AFX_MSG(CCMMvctCHStlPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdMvchCHStlCase2Chk();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVCTCHSTLPAGE_H__)
