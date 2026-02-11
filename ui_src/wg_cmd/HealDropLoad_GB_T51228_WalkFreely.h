#if !defined(AFX_HEALDROPLOAD_GB_T51288_WALKFREELY_H__93F1D52A_7EFA_4623_A453_B83FCB2BD086__INCLUDED_)
#define  AFX_HEALDROPLOAD_GB_T51288_WALKFREELY_H__93F1D52A_7EFA_4623_A453_B83FCB2BD086__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HealDropLoad_GB_T51288_WalkFreely.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_GB_T51288_WalkFreely dialog
#include "HeelLoadChildDialog.h"
#include "..\wg_db\wg_db_TextUnit.h"
class CHealDropLoad_GB_T51288_WalkFreely : public CHeelLoadChildDialog
{
// Construction
public:
	CHealDropLoad_GB_T51288_WalkFreely(CWnd* pParent = NULL);   // standard constructor
	void End();
	void MakeSpectrumData();
	BOOL ISVALID();

// Dialog Data
	//{{AFX_DATA(CHealDropLoad_GB_T51288_WalkFreely)
	enum { IDD = IDD_HEELDROPLOAD_WALK_FREELY };
	CTextUnit	m_stWeightUnit;
	double  m_dG;
	double	m_dFs;
	int 	m_iStepNum;
	int 	m_iTotalCrowd;
	double	m_dTimeStep;
	int     m_iRepeat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHealDropLoad_GB_T51288_WalkFreely)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CHealDropLoad_GB_T51288_WalkFreely)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEALDROPLOAD_GB_T51288_WALKFREELY_H__93F1D52A_7EFA_4623_A453_B83FCB2BD086__INCLUDED_)
