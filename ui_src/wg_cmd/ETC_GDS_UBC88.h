#if !defined(AFX_ETC_GDS_UBC88_H__E39085CC_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_)
#define AFX_ETC_GDS_UBC88_H__E39085CC_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_UBC88.h : header file
//
#include "MyChildDialog.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcUBC88Util;
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_UBC88 dialog

class CETC_GDS_UBC88 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_UBC88(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	BOOL ISVALID();
	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit=TRUE);

// Dialog Data
	//{{AFX_DATA(CETC_GDS_UBC88)
	enum { IDD = IDD_ETC_GDS_UBC };
	MComboBox	m_cmbRw;
	MComboBox	m_cmbOIF;
	CString	m_sOIF;
	CString	m_sRw;
	int		m_nSoil;
	int		m_nSZF;
	double	m_dMaxPeriod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_UBC88)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcUBC88Util> m_pSpfcUtil;

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_UBC88)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_UBC88_H__E39085CC_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_)
