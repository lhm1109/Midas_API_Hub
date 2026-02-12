#if !defined(AFX_ETC_GDS_IS2002_H__32C3FF7D_EE7F_4E09_8051_A8E62931CEAA__INCLUDED_)
#define AFX_ETC_GDS_IS2002_H__32C3FF7D_EE7F_4E09_8051_A8E62931CEAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_IS2002.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcIS2002Util;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IS2002 dialog
#include "MyChildDialog.h"
class CETC_GDS_IS2002 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_IS2002(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	void UpdateDampingWnd(CWnd* pDampingWnd);
	BOOL ISVALID();


	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit=TRUE);
	void Dlg2UtilData() override;

// Dialog Data
	//{{AFX_DATA(CETC_GDS_IS2002)
	enum { IDD = IDD_ETC_GDS_IS2002 };
	MComboBox	m_wndIs02RCmb;
	MComboBox	m_wndIs02ImportanceCmb;
	CEditUnit m_wndIs02Damping;
	CEditUnit m_wndIs02DampingFactor;
	int		m_nZone;
	int		m_nSoil;
	double m_dMaxPeriod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_IS2002)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcIS2002Util> m_pSpfcUtil;

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_IS2002)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEtcGdsIs2002Damping();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_IS2002_H__32C3FF7D_EE7F_4E09_8051_A8E62931CEAA__INCLUDED_)
