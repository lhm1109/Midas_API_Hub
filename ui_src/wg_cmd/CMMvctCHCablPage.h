#if !defined(__CMMVCTCHCABLPAGE_H__)
#define __CMMVCTCHCABLPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctCHCablPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHCablPage dialog
#include "..\wg_db\EditUnit.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

class CCMMvctCHCablPage : public CInternationalDlg
{
// Construction
public:
	CCMMvctCHCablPage(T_FREQ_CABL* pData, CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CCMMvctCHCablPage)
	enum { IDD = IDD_CMD_ML_MVCT_CHINA_CABL };

	CEditUnit m_edta;
	CEditUnit m_edtL;

	MillustViewer m_ImgViewer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctCHCablPage)
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

	T_FREQ_CABL* m_pData;

	BOOL m_bImpactFactor;
	
	// Generated message map functions
	//{{AFX_MSG(CCMMvctCHCablPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVCTCHCABLPAGE_H__)
