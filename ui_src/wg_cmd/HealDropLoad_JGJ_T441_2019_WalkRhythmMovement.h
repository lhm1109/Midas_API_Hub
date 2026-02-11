
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HealDropLoad_GB_T51288_WalkRhythm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement dialog
#include "HeelLoadChildDialog.h"
#include "..\wg_db\wg_db_TextUnit.h"
class CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement : public CHeelLoadChildDialog
{
	// Construction
public:
	CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement(CWnd* pParent = NULL);   // standard constructor
	void End();
	void MakeSpectrumData();
	void GetGamma(double &dGamma);
	BOOL ISVALID();
	void InitOrderNumComb();

	// Dialog Data
	//{{AFX_DATA(CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement)
	enum { IDD = IDD_HEELDROPLOAD_WALK_MOVEMENT };
	CComboBox	m_cmbNType;
	CComboBox	m_cmbSportType;
	CComboBox	m_cmbOrderNum;
	CTextUnit	m_stQpUnit;
	double  m_dQp;
	double	m_dF1;
	int     m_nNType;
	int     m_nSportType;
	int     m_nOrderNum;
	int     m_iRepeat;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeNTypeCmb();
	afx_msg void OnSelchangeSportTypeCmb();
	afx_msg void OnSelchangeCoordTypeCmb();
	// Generated message map functions
	//{{AFX_MSG(CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement)
	// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

