#if !defined(AFX_ETC_GDS_CH_BRG89_H__0B29DED0_DDB0_4E34_9446_AE6A52181E63__INCLUDED_)
#define AFX_ETC_GDS_CH_BRG89_H__0B29DED0_DDB0_4E34_9446_AE6A52181E63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_CH_BRG89.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcCHBRG89Util;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_CH_BRG89 dialog
#include "MyChildDialog.h"

class CETC_GDS_CH_BRG89 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_CH_BRG89(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	BOOL ISVALID();	  
	void SetDefaultValue();
	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit=TRUE);

// Dialog Data
	//{{AFX_DATA(CETC_GDS_CH_BRG89)
	enum { IDD = IDD_ETC_GDS_CH_BRG89 };
	MComboBox	m_wndCz;
	MComboBox	m_wndCi;
	CEditUnit	m_wndTg;
	CEditUnit	m_wndMu;
	CEditUnit	m_wndKh;
	CEditUnit m_wndK;
	double	m_dMaxPeriod;
	int		m_wndSFI;
	int		m_wndSC;
	CString	m_strCi;
	CString	m_strCz;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_CH_BRG89)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcCHBRG89Util> m_pSpfcUtil;

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_CH_BRG89)
	virtual BOOL OnInitDialog();
	afx_msg void OnEtcGdsChBrg89RdoSfi();
	afx_msg void OnEtcGdsChBrg89RdoSite();
	afx_msg void OnChangeTg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_CH_BRG89_H__0B29DED0_DDB0_4E34_9446_AE6A52181E63__INCLUDED_)
