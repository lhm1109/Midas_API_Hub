
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HealDropLoad_GB_T51288_WalkRhythm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad dialog
#include "HeelLoadChildDialog.h"
#include "..\wg_db\wg_db_TextUnit.h"
class CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad : public CHeelLoadChildDialog
{
	// Construction
public:
	CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad(CWnd* pParent = NULL);   // standard constructor
	void End();
	void MakeSpectrumData();
	void GetGamma(int nIdx, double &dGamma);
	void GetPhi(int nIdx, double &dPhi);
	BOOL ISVALID();
	// Dialog Data
	//{{AFX_DATA(CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad)
	enum {	IDD = IDD_HEELDROPLOAD_WALK_INCENTIVE };
	CComboBox	m_cmbNType;
	CTextUnit	m_stWeightUnit;
	double  m_dPp;
	double	m_dF1;
	int     m_nNType;
	int     m_iRepeat;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeParaNCmb();
	// Generated message map functions
	//{{AFX_MSG(CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad)
	// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

