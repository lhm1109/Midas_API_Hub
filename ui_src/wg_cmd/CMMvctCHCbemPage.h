#if !defined(__CMMVCTCHCBEMPAGE_H__)
#define __CMMVCTCHCBEMPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctCHCbemPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHCbemPage dialog
#include "..\wg_db\EditUnit.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

class CCMMvctCHCbemPage : public CInternationalDlg
{
// Construction
public:
	CCMMvctCHCbemPage(T_FREQ_CBEM* pData, CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CCMMvctCHCbemPage)
	enum { IDD = IDD_CMD_ML_MVCT_CHINA_CBEM };

	CEditUnit m_edta;
	CEditUnit m_edtb;
	CEditUnit m_edtL;
	CEditUnit m_edtE;
	CEditUnit m_edtIc;
	CEditUnit m_edtmc;

	MillustViewer m_ImgViewer_Posi;
	MillustViewer m_ImgViewer_Nega;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctCHCbemPage)
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

	T_FREQ_CBEM* m_pData;

	BOOL m_bImpactFactor;
	
	// Generated message map functions
	//{{AFX_MSG(CCMMvctCHCbemPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVCTCHCBEMPAGE_H__)
