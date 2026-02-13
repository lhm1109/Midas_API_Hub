#if !defined(__RATINGPARAMDLG_H__)
#define __RATINGPARAMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RatingParamDlg.h : header file
//
#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CRatingParamDlg dialog

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRatingParamDlg : public CDialogMove
{
// Construction
public:
	CRatingParamDlg(CWnd* pParent = NULL);   // standard constructor
	~CRatingParamDlg();

// Dialog Data
	//{{AFX_DATA(CRatingParamDlg)
	enum { IDD = IDD_DGN_PSC_RATING_PRARM_DLG };
	CTextUnit	m_TensSteelUnit;
	CTextUnit	m_TensStressUnit;
	CTextUnit	m_CompStressUnit;
	CTextUnit	m_TensStressUnit_Sl;
	CTextUnit	m_CompStressUnit_Sl;
	CEditUnit	m_dAlwTendonStress;
	CEditUnit	m_dAlwTensStress;
	CEditUnit	m_dAlwCompStress;
	CEditUnit	m_dAlwTensStress_Sl;
	CEditUnit	m_dAlwCompStress_Sl;

	CTextUnit	m_TensStressUnit_Legal;
	CTextUnit	m_CompStressUnit_Legal;
	CTextUnit	m_TensStressUnit_Sl_Legal;
	CTextUnit	m_CompStressUnit_Sl_Legal;
	CEditUnit	m_dAlwTensStress_Legal;
	CEditUnit	m_dAlwCompStress_Legal;
	CEditUnit	m_dAlwTensStress_Sl_Legal;
	CEditUnit	m_dAlwCompStress_Sl_Legal;

	CTextUnit	m_AlwStressPermit_Prest_Unit;
	CTextUnit   m_AlwStressPermit_Reinf_Unit;	
	CEditUnit	m_dAlwStressPermit_Prest;
	CEditUnit	m_dAlwStressPermit_Reinf;

	//CButton	m_ratioDeflection;
	//CButton	m_ratioStrain;

	double	m_dSystemFactor;
	BOOL	m_bFlexure;
	int		m_iMn_Method;
	BOOL	m_bShear;
	BOOL	m_bStress;
	BOOL	m_bStress_Sl;
	BOOL	m_bStress_Legal;
	BOOL	m_bStress_Sl_Legal;
	BOOL	m_bStress_Permit;
	BOOL	m_bSteel;	
	int		m_iShear;
	BOOL  m_bStrain; //Diagnostic Test Result 0:strain 1:Displacement
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRatingParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRatingParamDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnDgnRatingParamFlexureCheck();
	afx_msg void OnDgnRatingParamStressCheck();
	afx_msg void OnDgnRatingParamStressSlabCheck();
	afx_msg void OnDgnRatingParamStressCheck_Legal();
	afx_msg void OnDgnRatingParamStressSlabCheck_Legal();
	afx_msg void OnDgnRatingParamPresSteelCheck();
	afx_msg void OnDgnRatingParamShearCheck();
	afx_msg void OnDgnRatingParamStressCheck_Permit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

	CDBDoc* m_pDoc;

	T_BLRS_D m_MainData;

	void InitCtrl();
	void ResizeDialog();

	void SetInitUnit();

	void LoadDlgSysTemFactor();
	void LoadDlgStrength();
	void LoadDlgService();
	void LoadDlgDiagnostic_Method();

	void SaveDlgSysTemFactor();	
	void SaveDlgStrength();
	void SaveDlgService();
	void SaveDlgDiagnostic_Method();
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__RATINGPARAMDLG_H__)
