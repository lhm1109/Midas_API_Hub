#if !defined(__DgnSeisAnchorShearFactor_H__)
#define __DgnSeisAnchorShearFactor_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorShearFactor.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorShearFactor dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnSeisAnchorShearFactor : public CDialogMove
{
// Construction
public:
	CDgnSeisAnchorShearFactor(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorShearFactor)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_SHEAR_FACTOR_DLG };
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorShearFactor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Data2Dlg();
	BOOL Dlg2Data();

public:
	int m_nShearFactorType;

protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorShearFactor)
	virtual BOOL OnInitDialog();
	afx_msg void OnShearFactorOK();
	afx_msg void OnShearFactorCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchorShearFactor_H__)
