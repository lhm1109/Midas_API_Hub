#if !defined(__CDgnCPGDesignParamIRC22_15_Dlg_H__)
#define __CDgnCPGDesignParamIRC22_15_Dlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDgnCPGDesignParamIRC22_15_Dlg.h : header file
//
#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamIRC22_15_Dlg dialog

class CDgnCPGDesignParamBaseDlg;
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnCPGDesignParamIRC22_15_Dlg : public CChildDialog
{
	// Construction
public:
	CDgnCPGDesignParamIRC22_15_Dlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnCPGDesignParamIRC22_15_Dlg();

	// Dialog Data
	//{{AFX_DATA(CDgnCPGDesignParamIRC22_15_Dlg)
	enum { IDD = IDD_DGN_CPG_PARAM_IRC22_2015 };

	double	m_dGamm_C;
	double	m_dGamm_C_Acci;
	double	m_dGamm_M0;
	double	m_dGamm_M1;
	double	m_dGamm_s;
	double	m_dGamm_V;
	double	m_dGamm_Ff;
	double	m_dGamm_Mf;

	double	m_dt_Ld;
	double  m_dk1;
	double  m_dk3;
	double  m_dk4;
	double  m_dk6;
	double  m_dCrackk3;
	double  m_dCrackk4;

	double  m_dDeflnCtrl;

	double m_dLoadCycles;
	double m_dSteelStressLim;
	double m_dConcStressLim;
	double m_dGamma_M0_IRC;
	double m_dGamma_M1_IRC;

	MComboBox m_cmbIRCExpos;
	int    m_iExpClass;
 CTextUnit	m_DeflUnit;

	BOOL	m_bBendig_Resist;
	BOOL	m_bResist_Vertical_Shear;
	BOOL	m_bResist_Later_Torsion_Buckling;
	BOOL	m_bResist_Trans_Force;
	BOOL	m_bResist_Longi_Shear;
	BOOL	m_bResist_Fatigue;
	BOOL  m_bPostBucklTensField;
	BOOL  m_bStress_Limit;
	BOOL  m_bLongi_Shear;
	BOOL  m_bDeflnCtrl;
	BOOL  m_bCrackWidthChk;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnCPGDesignParamIRC22_15_Dlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnCPGDesignParamIRC22_15_Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	virtual void OnCancel();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CDgnCPGDesignParamBaseDlg* m_pParent;
	CDBDoc* m_pDoc;

	T_CPGD_D *m_pData;

public:
	inline void SetData(T_CPGD_D* pData) { m_pData = pData; }
	void Data2Dlg();
	void Dlg2Data();		
	void InitComboBox_IRC112();
	afx_msg void OnBnClickedDngCpgParamGroup6();
	afx_msg void OnBnClickedDngCpgParamGroup5();
	void SetDefaultData();
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CDgnCPGDesignParamIRC22_15_Dlg_H__)
