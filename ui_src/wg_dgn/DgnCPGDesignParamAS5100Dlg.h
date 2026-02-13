#if !defined(__CDgnCPGDesignParamAS5100Dlg_H__)
#define __CDgnCPGDesignParamAS5100Dlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDgnCPGDesignParamAS5100Dlg.h : header file
//
#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamAS5100Dlg dialog

class CDgnCPGDesignParamBaseDlg;
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnCPGDesignParamAS5100Dlg : public CChildDialog
{
// Construction
public:
	CDgnCPGDesignParamAS5100Dlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnCPGDesignParamAS5100Dlg();

// Dialog Data
	//{{AFX_DATA(CDgnCPGDesignParamAS5100Dlg)
	enum { IDD = IDD_DGN_CPG_PARAM_AS5100 };

	double	m_dPhy_FL;
	double	m_dPhy_SH;
	double	m_dPhy_ST;
	double	m_dPhy_SC;

	BOOL m_bUlt_FL;
	BOOL m_bUlt_FH;
	BOOL m_bUlt_FST;
	BOOL m_bUlt_LS;

	BOOL m_bSer_ST;
	BOOL m_bSer_LS;


	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnCPGDesignParamAS5100Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnCPGDesignParamAS5100Dlg)
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
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CDgnCPGDesignParamAS5100Dlg_H__)
