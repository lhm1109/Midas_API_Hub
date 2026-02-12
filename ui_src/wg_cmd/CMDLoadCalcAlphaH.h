#if !defined(AFX_CMDLOADCALCALPHAH_H__2BD263A9_0DB2_485B_9D3C_26E28CA38A6F__INCLUDED_)
#define AFX_CMDLOADCALCALPHAH_H__2BD263A9_0DB2_485B_9D3C_26E28CA38A6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMDLoadCalcAlphaH.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

/////////////////////////////////////////////////////////////////////////////
// CCMDLoadCalcAlphaH dialog

class CCMDLoadCalcAlphaH : public CDialogMove
{
// Construction
public:
	CCMDLoadCalcAlphaH(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMDLoadCalcAlphaH)
	enum { IDD = IDD_CMD_LOAD_ALPHA_H };
	double	m_dHeight;
	int		m_nOption;
	//}}AFX_DATA

public:
	double m_dAlpha_H;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMDLoadCalcAlphaH)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMDLoadCalcAlphaH)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdCalcAlphaH();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDLOADCALCALPHAH_H__2BD263A9_0DB2_485B_9D3C_26E28CA38A6F__INCLUDED_)
