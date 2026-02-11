#if !defined(__CMMVCTCHSBEMPAGE_H__)
#define __CMMVCTCHSBEMPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctCHSbemPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHSbemPage dialog
#include "..\wg_db\EditUnit.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

class CCMMvctCHSbemPage : public CInternationalDlg
{
// Construction
public:
	CCMMvctCHSbemPage(T_FREQ_SBEM* pData, CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CCMMvctCHSbemPage)
	enum { IDD = IDD_CMD_ML_MVCT_CHINA_SBEM };
	
	CEditUnit m_edtL;
	CEditUnit m_edtE;
	CEditUnit m_edtIc;
	CEditUnit m_edtmc;

	MillustViewer m_ImgViewer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctCHSbemPage)
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

	T_FREQ_SBEM* m_pData;

	BOOL m_bImpactFactor;
	
	// Generated message map functions
	//{{AFX_MSG(CCMMvctCHSbemPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVCTCHSBEMPAGE_H__)
