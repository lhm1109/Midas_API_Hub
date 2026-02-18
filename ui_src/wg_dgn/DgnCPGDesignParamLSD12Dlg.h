#if !defined(__CDgnCPGDesignParamLSD12Dlg_H__)
#define __CDgnCPGDesignParamLSD12Dlg_H__

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
// CDgnCPGDesignParamLSD12Dlg dialog

class CDgnCPGDesignParamBaseDlg;
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnCPGDesignParamLSD12Dlg : public CChildDialog
{
// Construction
public:
	CDgnCPGDesignParamLSD12Dlg(CWnd* pParent);   // standard constructor
	~CDgnCPGDesignParamLSD12Dlg();

// Dialog Data
	//{{AFX_DATA(CDgnCPGDesignParamLSD12Dlg)
	enum { IDD = IDD_DGN_CPG_PARAM_KSCE_LSD12 };

	double	m_dPhy_y;
	double	m_dPhy_u;
	double	m_dPhy_c;
	double	m_dPhy_f;
	double	m_dPhy_v;
	double	m_dPhy_se;
	double	m_dPhy_b;
	//double  m_dGamma;
	int     m_GirdernType;
	int     m_BridgeType;

	BOOL	m_bFlexure;
	BOOL	m_bShear;
	BOOL	m_bServiceLimitSate;
	BOOL	m_bConstructibility;
	BOOL	m_bFatigueLimitState;
	BOOL	m_bSubMembers;
	BOOL m_bWarpStress;
	BOOL m_bPBTF_Eff;
	BOOL  m_b1p3RhMy;          

	BOOL m_bPtrOpt_Sum;
	BOOL m_bPtrOpt_Detail;
	BOOL m_bTorWarping;
	BOOL m_bAppendix6;

	BOOL m_bCompReinforcement;

	CArray<UINT, UINT> m_aCtrlCompRein;
	CArray<UINT, UINT> m_aCtrlLimitStat;
	CArray<UINT, UINT> m_aCtrlDgnParam;
	CArray<UINT, UINT> m_aCtrlPrintOpt;


	//BOOL  m_bConstructionStage;
	//}}AFX_DATA


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
	afx_msg void OnOk();
	virtual void OnCancel();
	
	void AlignCtrl(int nCode);
	void ShowHideCtrl(int nCode);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CDgnCPGDesignParamBaseDlg* m_pParent;
	CDBDoc* m_pDoc;

	T_CPGD_D *m_pData;

public:
	inline void SetData(T_CPGD_D* pData) { m_pData = pData;}
	void Data2Dlg();
	void Dlg2Data();		
	afx_msg void OnBnClickedDgnCpgParamCurvedRdo();
	afx_msg void OnBnClickedDgnCpgParamStraightRdo();
	afx_msg void OnSelectGirderType();
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CDgnCPGDesignParamLSD12Dlg_H__)
