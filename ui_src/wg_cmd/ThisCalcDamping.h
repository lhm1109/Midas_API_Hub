#if !defined(AFX_THISCALCDAMPING_H__0E02650F_5D34_41FB_9A2C_5CE5B1C95CCB__INCLUDED_)
#define AFX_THISCALCDAMPING_H__0E02650F_5D34_41FB_9A2C_5CE5B1C95CCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThisCalcDamping.h : header file
//

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CThisCalcDamping dialog
//#include "HeaderPre.h"
class CThisCalcDamping : public CDialogMove
{
// Construction
public:
	CThisCalcDamping(CWnd* pParent = NULL);   // standard constructor

public:
	BOOL   m_bMass;
	BOOL   m_bStiff;
	double m_dRm;
	double m_dRk;
	double m_dFrequency;
	double m_dPeriod;  

// Dialog Data
	//{{AFX_DATA(CThisCalcDamping)
	enum { IDD = IDD_ETC_THIS_DAMPING_CALC };
	int		    m_nFreqPeriodRdo;
	MButton   m_wndMassChk;
	MButton   m_wndStiffChk;
	CEditUnit m_wndMassCoef;
	CEditUnit m_wndStiffCoef;
	CEditUnit m_wndFrequency;
	CEditUnit m_wndPeriod;
	CEditUnit m_wndDampingRatio;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThisCalcDamping)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void CtrlManager();
	void AutoCalcDampingRatio();

	// Generated message map functions
	//{{AFX_MSG(CThisCalcDamping) 
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeFreqPeriod();
	virtual void OnOK();
	afx_msg void OnChangeCmdFrequencyEdit();
	afx_msg void OnChangeCmdPeriodEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THISCALCDAMPING_H__0E02650F_5D34_41FB_9A2C_5CE5B1C95CCB__INCLUDED_)
