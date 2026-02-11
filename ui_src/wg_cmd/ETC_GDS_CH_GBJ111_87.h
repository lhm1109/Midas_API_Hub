#if !defined(AFX_ETC_GDS_CH_GBJ111_87_H__2A28D726_2AB7_405B_B4C1_56266CDA2EA1__INCLUDED_)
#define AFX_ETC_GDS_CH_GBJ111_87_H__2A28D726_2AB7_405B_B4C1_56266CDA2EA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_CH_GBJ111_87.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_CH_GBJ111_87 dialog
#include "MyChildDialog.h"

class CSpfcCHGBJ1187Util;

class CETC_GDS_CH_GBJ111_87 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_CH_GBJ111_87(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	BOOL ISVALID();
	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit=TRUE);

// Dialog Data
	//{{AFX_DATA(CETC_GDS_CH_GBJ111_87)
	enum { IDD = IDD_ETC_GDS_CH_GBJ111_87 };
	CEditUnit	m_wndKh;
	CEditUnit m_wndTg1;
	CEditUnit m_wndTg2;
	CEditUnit m_wndCz;
	double	m_dMaxPeriod;  
	int		m_wndSFI;
	int		m_wndSC;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_CH_GBJ111_87)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcCHGBJ1187Util> m_pSpfcUtil;

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_CH_GBJ111_87)
	virtual BOOL OnInitDialog();
	afx_msg void OnEtcGdsChGbj11189RdoSfi();
	afx_msg void OnEtcGdsChGbj11189RdoSite();
	afx_msg void OnChangeTg1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_CH_GBJ111_87_H__2A28D726_2AB7_405B_B4C1_56266CDA2EA1__INCLUDED_)
