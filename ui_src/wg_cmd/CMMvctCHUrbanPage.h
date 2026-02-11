#if !defined(__CMMVCTCHURBANPAGE_H__)
#define __CMMVCTCHURBANPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctCHUrbanPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHUrbanPage dialog
#include "..\wg_db\EditUnit.h"

class CCMMvctCHUrbanPage : public CInternationalDlg
{
// Construction
public:
	CCMMvctCHUrbanPage(T_BRDG_URBAN* pData, CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CCMMvctCHUrbanPage)
	enum { IDD = IDD_CMD_ML_MVCT_CHINA_URBAN };
	
	CEditUnit m_edtCase1L_a;
	CEditUnit m_edtCase1Fact_a;
	CEditUnit m_edtCase1L_b;	
	CEditUnit m_edtCase1Fact_b;
	CEditUnit m_edtCase1L_U;
	CEditUnit m_edtCase1L_L;
	CEditUnit m_edtCase1F_U;
	CEditUnit m_edtCase1F_L;
	CEditUnit m_edtCase2FactCoef1;
	CEditUnit m_edtCase2FactCoef2;
	CEditUnit m_edtCase2FactLimit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctCHUrbanPage)
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

	T_BRDG_URBAN* m_pData;

	BOOL m_bImpactFactor;
	
	// Generated message map functions
	//{{AFX_MSG(CCMMvctCHUrbanPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdMvchCHUrbanCase1LEdt();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVCTCHURBANPAGE_H__)
