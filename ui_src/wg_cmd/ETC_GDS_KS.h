#if !defined(AFX_ETC_GDS_KS_H__E39085CE_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_)
#define AFX_ETC_GDS_KS_H__E39085CE_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_KS.h : header file
//
#include "MyChildDialog.h"

#include "..\MIT_frx\MComboBox.h"

class CSpfcKSUtil;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KS dialog

class CETC_GDS_KS : public CMyChildDialog
{
// Construction
public:
	void End();
	void EndNotClose(){};
	CETC_GDS_KS(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	~CETC_GDS_KS(); 
	BOOL ISVALID();
	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit=TRUE);

protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcKSUtil> m_pSpfcUtil;
// Dialog Data
	//{{AFX_DATA(CETC_GDS_KS)
	enum { IDD = IDD_ETC_GDS_KS };
	MComboBox	m_cmbRMC;
	MComboBox	m_cmbOIF;
	int		m_rdoSOIL;
	CString	m_sOIF;
	CString	m_sRMC;
	int		m_nEPA;
	double	m_dMaxPeriod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_KS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_KS)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_KS_H__E39085CE_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_)
