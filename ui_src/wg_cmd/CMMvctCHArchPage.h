#if !defined(__CMMVCTCHARCHPAGE_H__)
#define __CMMVCTCHARCHPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctCHArchPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHArchPage dialog
#include "..\wg_db\EditUnit.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

class CCMMvctCHArchPage : public CInternationalDlg
{
// Construction
public:
	CCMMvctCHArchPage(T_FREQ_ARCH* pData, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMMvctCHArchPage)
	enum { IDD = IDD_CMD_ML_MVCT_CHINA_ARCH };

	CEditUnit m_edtn;
	CEditUnit m_edtf;
	CEditUnit m_edtL;
	CEditUnit m_edtE;
	CEditUnit m_edtIc;
	CEditUnit m_edtmc;

	MillustViewer m_ImgViewer1;
	MillustViewer m_ImgViewer2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctCHArchPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	BOOL SetDlg2Data();
	void ControlsShowHide(int nArchType);
	void EnableDisableControls(BOOL bImpactFactor, BOOL bArchType=FALSE);
	void EnableDisableComponent(BOOL bEnable);

// Implementation
protected:
	void InitUnit();
	void AlignControl();

	T_FREQ_ARCH* m_pData;

	int  m_nArchType;
	BOOL m_bImpactFactor;
	
	// Generated message map functions
	//{{AFX_MSG(CCMMvctCHArchPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVCTCHARCHPAGE_H__)
