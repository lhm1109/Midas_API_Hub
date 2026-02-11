#if !defined(AFX_ETC_GDS_KS2000_H__F127A441_5897_11D4_92C8_0080AD78AAC8__INCLUDED_)
#define AFX_ETC_GDS_KS2000_H__F127A441_5897_11D4_92C8_0080AD78AAC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_KS2000.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KS2000 dialog
#include "MyChildDialog.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcKS2000Util;
using namespace mit::frx;

class CETC_GDS_KS2000 : public CMyChildDialog
{
// Construction
public:
	void End();
	void EndNotClose(){};
	CETC_GDS_KS2000(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	BOOL ISVALID();
	BOOL Dlg2Data();
	virtual void Data2Dlg();
	void SetPushOverData(BOOL bInit=TRUE);

protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcKS2000Util> m_pSpfcUtil;
// Dialog Data
	//{{AFX_DATA(CETC_GDS_KS)
	enum { IDD = IDD_ETC_GDS_KS2000 };
	MComboBox	m_cmbRMC;
	MComboBox	m_cmbOIF;
	int		m_rdoSOIL;
	CString	m_sOIF;
	CString	m_sRMC;
	int		m_nEPA;
	double m_dMaxPeriod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_KS2000)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_KS2000)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_KS2000_H__F127A441_5897_11D4_92C8_0080AD78AAC8__INCLUDED_)
