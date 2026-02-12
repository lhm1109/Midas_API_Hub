#if !defined(__ETC_DESIGN_SPECT_TS_H__)
#define __ETC_DESIGN_SPECT_TS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_DESIGN_SPECT_TS.h : header file
//
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_DESIGN_SPECT_TS dialog
#include "HeaderPre.h"

class CETC_GDS_KS_BRG;
class CETC_GDS_KSCE2019;
class CETC_GDS_KDS2019;

class __MY_EXT_CLASS__ CETC_DESIGN_SPECT_TS : public CDialogMove
{
// Construction
public:
	CETC_DESIGN_SPECT_TS(CWnd* pParent = NULL);  

public:
// Dialog Data
	//{{AFX_DATA(CETC_DESIGN_SPECT_TS)
	enum { IDD = IDD_ETC_DESIGN_SPECTRUM_TS };

	MComboBox	m_cmbCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_DESIGN_SPECT_TS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitCombo();
	void CreatePage();
	void ShowPage();
	
// Implementation
public:
	double m_dTs;

protected:
	CETC_GDS_KS_BRG*   m_pKSBRG;
	CETC_GDS_KSCE2019* m_pKSCE2019;
	CETC_GDS_KDS2019*  m_pKDS2019;

	// Generated message map functions
	//{{AFX_MSG(CETC_DESIGN_SPECT_TS)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSpectTsCodeCmb();
	afx_msg void OnDgnSpectTsOKBtn();
	afx_msg void OnDgnSpectTsCancelBtn();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL m_bFirstActive;
	BOOL m_bInitActive;
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ETC_DESIGN_SPECT_TS_H__)
