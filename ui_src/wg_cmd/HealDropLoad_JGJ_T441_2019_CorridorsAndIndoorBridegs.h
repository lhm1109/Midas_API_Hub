
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HealDropLoad_GB_T51288_WalkRhythm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs dialog
#include "HeelLoadChildDialog.h"
#include "..\wg_db\wg_db_TextUnit.h"
class CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs : public CHeelLoadChildDialog
{
	// Construction
public:
	CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs(CWnd* pParent = NULL);   // standard constructor
	void End();
	void MakeSpectrumData();
	void GetGamma1(double& dGamma1);
	void GetPhi(double dfs1, double dfs2, double &dPhi);
	void GetPhiL(double dfsL, double& dPhiL);
	BOOL ISVALID();
	void ChangeItemStatic();
	void ChangeAutoCalc();

	// Dialog Data
	//{{AFX_DATA(CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs)
	enum {	IDD = IDD_HEELDROPLOAD_WALK_CORRIDOR };
	CComboBox	m_cmbNType;
	CComboBox	m_cmbOrderNumType;
	CTextUnit	m_stPbUnit;
	CTextUnit	m_stAreaUnit;
	double  m_dPb;
	double	m_dF1;
	double  m_dPhi;
	double  m_dArea;
	double  m_dDampRto;
	BOOL	m_bAutoCal;
	int		m_nLoadDir;
	int     m_nNType;
	int     m_nOrderNumType;
	int     m_iRepeat;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeNCmb();
	afx_msg void OnSelchangeOrderNumCmb();
	afx_msg void OnChkLoadDirRadio();
	afx_msg void OnChkAutoCalcPhi();
	// Generated message map functions
	//{{AFX_MSG(CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs)
	// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
