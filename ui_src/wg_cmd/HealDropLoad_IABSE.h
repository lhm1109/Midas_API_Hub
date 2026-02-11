#if !defined(AFX_HEALDROPLOAD_IABSE_H__2EF85EEC_4AC6_411A_A8B8_B8BA6FE5E889__INCLUDED_)
#define AFX_HEALDROPLOAD_IABSE_H__2EF85EEC_4AC6_411A_A8B8_B8BA6FE5E889__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HealDropLoad_IABSE.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// HealDropLoad_IABSE dialog
#include "HeelLoadChildDialog.h"
#include "..\wg_db\wg_db_TextUnit.h" // Static Text auto-change Unit 

#define IABSE         1
#define AIJRC_WALK    2
#define AIJRC_RUN     4
#define ALLEN_RAINER  8
#define IABSE1STEP    16
#define GB_T51288_FREE 21
#define GB_T51288_RHYT 22

class HealDropLoad_IABSE : public CHeelLoadChildDialog
{
// Construction
public:
	int m_nLoadType;
	HealDropLoad_IABSE(int nLoadType,CWnd* pParent = NULL);   // standard constructor
	void End();
	void MakeSpectrumData();
	void MakeIABSE1StepData();
	void MakeIABSEContData();
	BOOL ISVALID();
// Dialog Data
	//{{AFX_DATA(HealDropLoad_IABSE)
	enum { IDD = IDD_HEELDROPLOAD_IABSE };
	CTextUnit	m_stWeightUnit;
	double	m_dFs;
	double	m_dG;
	double	m_dTimeStep;
	int		m_nRepeat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(HealDropLoad_IABSE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(HealDropLoad_IABSE)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEALDROPLOAD_IABSE_H__2EF85EEC_4AC6_411A_A8B8_B8BA6FE5E889__INCLUDED_)
