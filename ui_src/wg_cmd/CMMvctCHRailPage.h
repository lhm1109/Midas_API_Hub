#if !defined(__CMMVCTCHRAILPAGE_H__)
#define __CMMVCTCHRAILPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctCHRailPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHRailPage dialog
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CCMMvctCHRailPage : public CInternationalDlg
{
// Construction
public:
	CCMMvctCHRailPage(T_BRDG_RAIL* pData, int nBrgType, CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CCMMvctCHRailPage)
	enum { IDD = IDD_CMD_ML_MVCT_CHINA_RAIL };

	CEditUnit m_edtMu1;
	CEditUnit m_edtMu2;
	CEditUnit m_edtMu3;
	BOOL      m_bLfai;
	CEditUnit m_edtLfai;
	BOOL      m_bLength;
	MComboBox m_cmbLengthGrup;
	CEditUnit m_edtMuR1;
	CEditUnit m_edtMuR2;
	CEditUnit m_edtMuR3;
	CEditUnit m_edthc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctCHRailPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	BOOL SetDlg2Data();
	void EnableDisableControls(BOOL bImpactFactor, int nSpanType);

// Implementation
protected:
	void InitUnit();
	void InitCombo();
	void ControlsShowHide();	

	CDBDoc* m_pDoc;

	T_BRDG_RAIL* m_pData;

	int  m_nBrgType;
	int  m_nSpanType;
	BOOL m_bImpactFactor;

	CArray<UINT, UINT> m_aMuReduc;
	
	// Generated message map functions
	//{{AFX_MSG(CCMMvctCHRailPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdMvchCHRailLfaiChk();
	afx_msg void OnCmdMvchCHRailLengthChk();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVCTCHRAILPAGE_H__)
