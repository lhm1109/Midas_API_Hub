#if !defined(__CPGPARAMDLG_H__)
#define __CPGPARAMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPGParamDlg.h : header file
//
#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CCPGParamDlg dialog

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCPGParamDlg : public CDialogMove
{
// Construction
public:
	CCPGParamDlg(CWnd* pParent = NULL);   // standard constructor
	~CCPGParamDlg();

// Dialog Data
	//{{AFX_DATA(CCPGParamDlg)
	enum { IDD = IDD_DGN_CPG_PARAM };

	mit::frx::MComboBox	m_DesignCode;
    mit::frx::MComboBox	m_SubCode;

	double	m_dGamm_C;
	double	m_dGamm_S;
	double	m_dGamm_M0;
	double	m_dGamm_M1;
	double	m_dGamm_V;
	double	m_dGamm_Ff;
	double	m_dGamm_Mf;
	double	m_dGamm_Mf_s;
	double  m_dGamm_M_ser;
	double	m_dt_Ld;
	double  m_dk1;
	double  m_dk2;
	double  m_dk3;
	double  m_dks;

	BOOL	m_bBendig_Resist;
	BOOL	m_bResist_Vertical_Shear;
	BOOL	m_bResist_Later_Torsion_Buckling;
	BOOL	m_bResist_Trans_Force;
	BOOL	m_bResist_Longi_Shear;
	BOOL	m_bResist_Fatigue;
	BOOL  m_bStress_Limit;
	BOOL  m_bLongi_Shear;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPGParamDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	virtual void OnCancel();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

	CDBDoc* m_pDoc;

	T_CPGD_D m_MainData;

	void LoadDlgData();
	void SaveDlgData();		
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CPGPARAMDLG_H__)
