#if !defined(__CMMVCTCHSUSPPAGE_H__)
#define __CMMVCTCHSUSPPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctCHSuspPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHSuspPage dialog
#include "..\wg_db\EditUnit.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

class CCMMvctCHSuspPage : public CInternationalDlg
{
// Construction
public:
	CCMMvctCHSuspPage(T_FREQ_SUSP* pData, CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CCMMvctCHSuspPage)
	enum { IDD = IDD_CMD_ML_MVCT_CHINA_SUSP };
	
	CEditUnit m_edtL;
	CEditUnit m_edtE;
	CEditUnit m_edtI;
	CEditUnit m_edtHg;
	CEditUnit m_edtm;

	MillustViewer m_ImgViewer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctCHSuspPage)
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

	T_FREQ_SUSP* m_pData;

	BOOL m_bImpactFactor;
	
	// Generated message map functions
	//{{AFX_MSG(CCMMvctCHSuspPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVCTCHSUSPPAGE_H__)
