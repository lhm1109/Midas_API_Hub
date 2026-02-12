#if !defined(AFX_HEALDROPLOAD_GB_T51288_WALKRTHYTHM_H__93F1D52A_7EFA_4623_A453_B83FCB2BD086__INCLUDED_)
#define  AFX_HEALDROPLOAD_GB_T51288_WALKRTHYTHM_H__93F1D52A_7EFA_4623_A453_B83FCB2BD086__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HealDropLoad_GB_T51288_WalkRhythm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_GB_T51288_WalkRhythm dialog
#include "HeelLoadChildDialog.h"
#include "..\wg_db\wg_db_TextUnit.h"
class CHealDropLoad_GB_T51288_WalkRhythm : public CHeelLoadChildDialog
{
	// Construction
public:
	CHealDropLoad_GB_T51288_WalkRhythm(CWnd* pParent = NULL);   // standard constructor
	void End();
	void MakeSpectrumData();
	void GetAlhpa(double &dAlpha1, double &dAlpha2, double &dAlpha3);
	void GetCn(double &dCn1, double &dCn2, double &dCn3);
	void GetPhi(double Pi, double &dPhi1,  double &dPhi2, double &dPhi3);
	BOOL ISVALID();
	BOOL CheckFsData();

	// Dialog Data
	//{{AFX_DATA(CHealDropLoad_GB_T51288_WalkRhythm)
	enum { IDD = IDD_HEELDROPLOAD_WALK_RHYTHM };
	CComboBox	m_cmbSportType;
	CComboBox	m_cmbCoordType;
	CTextUnit	m_stWeightUnit;
	double  m_dG;
	double	m_dFs;
	int 	m_iTimeStep;
	int 	m_iTotalCrowd;
	double	m_dTimeStep;
	int     m_iRepeat;

	int     m_nSportType;
	int     m_nCoordType;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHealDropLoad_GB_T51288_WalkRhythm)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeSportTypeCmb();
	afx_msg void OnSelchangeCoordTypeCmb();
	// Generated message map functions
	//{{AFX_MSG(CHealDropLoad_GB_T51288_WalkRhythm)
	// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //!defined(AFX_HEALDROPLOAD_GB_T51288_WALKRTHYTHM_H__93F1D52A_7EFA_4623_A453_B83FCB2BD086__INCLUDED__