#if !defined(__CRatingPhiCPGLSD15Dlg_H__)
#define __CRatingPhiCPGLSD15Dlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDgnCPGDesignParamLSD12Dlg.h : header file
//
#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CRatingPhiCPGLSD15Dlg dialog

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRatingPhiCPGLSD15Dlg : public CDialogMove
{
	// Construction
public:
	CRatingPhiCPGLSD15Dlg(CWnd* pParent = NULL);   // standard constructor
	// Dialog Data
	//{{AFX_DATA(CDgnCPGDesignParamLSD12Dlg)
	enum { IDD = IDD_DGN_RATING_LSD15_CPG_PARAM };

	CEditUnit	m_dPhy_y;
	CEditUnit	m_dPhy_u;
	CEditUnit	m_dPhy_c;
	CEditUnit	m_dPhy_f;
	CEditUnit	m_dPhy_v;
	CEditUnit	m_dPhy_se;
	CEditUnit	m_dPhy_b;
	//double  m_dGamma;
	int     m_iGirdernType;
	int     m_iBridgeType;

	BOOL m_bWarpStress;
	BOOL m_bPBTF_Eff;
	BOOL m_b1p3RhMy; 

	CDBDoc* m_pDoc;

	T_RPAS_D m_Data;

	void Initial_Data();
	void Dlg2Data();
	//BOOL  m_bConstructionStage;
	//}}AFX_DATA
	afx_msg void OnBnClickedDgnCpgParamCurvedRdo();
	afx_msg void OnBnClickedDgnCpgParamStraightRdo();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnCPGDesignParamLSD12Dlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnCPGDesignParamLSD12Dlg)
	virtual BOOL OnInitDialog();
	virtual void OnOk();
	virtual void OnCancel();
	afx_msg void OnDgnPhiUpdateBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
			
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CDgnCPGDesignParamLSD12Dlg_H__)
