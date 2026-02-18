#pragma once

// CDgnCPGDesignParamCSA14Dlg.h : header file
//
#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamCSA14Dlg dialog

class CDgnCPGDesignParamBaseDlg;
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnCPGDesignParamCSA14Dlg : public CChildDialog
{
	// Construction
public:
	CDgnCPGDesignParamCSA14Dlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnCPGDesignParamCSA14Dlg();

	// Dialog Data
	//{{AFX_DATA(CDgnCPGDesignParamCSA14Dlg)
	enum { IDD = IDD_DGN_CPG_PARAM_CSA_14 };

	double	m_dPhi_s_Flex;
	double	m_dPhi_s_Shear;
	double	m_dPhi_s_Comp;
	double	m_dPhi_s_Tens;
	double	m_dPhi_s_Tors;
	double	m_dPhi_r;
	double	m_dPhi_sc;
	double	m_dPhi_c;
	double	m_dPhi_u;

	//double  m_dGamma;
	int     m_GirdernType;
	int     m_BridgeType;

	BOOL	m_bFlexure;
	BOOL	m_bShear;
	BOOL	m_bServiceLimitSate;
	BOOL	m_bConstructibility;
	BOOL	m_bFatigueLimitState;
	BOOL	m_bStiffener;
	BOOL	m_bCurvedGirder;
	BOOL  m_bConstructionStage;
	BOOL  m_bDistortion;       

	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnCPGDesignParamCSA14Dlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnCPGDesignParamCSA14Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	afx_msg void OnSelectGirderType();
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
};

#include "HeaderPost.h"


