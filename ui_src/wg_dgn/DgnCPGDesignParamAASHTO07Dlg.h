#if !defined(__CDgnCPGDesignParamAASHTO07Dlg_H__)
#define __CDgnCPGDesignParamAASHTO07Dlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDgnCPGDesignParamAASHTO07Dlg.h : header file
//
#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamAASHTO07Dlg dialog

class CDgnCPGDesignParamBaseDlg;
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnCPGDesignParamAASHTO07Dlg : public CChildDialog
{
// Construction
public:
	CDgnCPGDesignParamAASHTO07Dlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnCPGDesignParamAASHTO07Dlg();

// Dialog Data
	//{{AFX_DATA(CDgnCPGDesignParamAASHTO07Dlg)
	enum { IDD = IDD_DGN_CPG_PARAM_AASHTO07_LRFD };

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
	BOOL  m_bTorWarping;
	BOOL  m_bAppendix6;
	BOOL  m_bDistortion;       
	BOOL  m_b1p3RhMy;          
	BOOL  m_bPBTFA4SH;        
	BOOL  m_bLongTermSectProp;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnCPGDesignParamAASHTO07Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnCPGDesignParamAASHTO07Dlg)
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
	void ShowHideControls();
	void AlignCtrl();

};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CDgnCPGDesignParamAASHTO07Dlg_H__)
