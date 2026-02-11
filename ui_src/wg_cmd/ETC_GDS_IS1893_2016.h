#if !defined(AFX_ETC_GDS_IS1893_2016_H__32C3FF7D_EE7F_4E09_8051_A8E62931CEAA__INCLUDED_)
#define AFX_ETC_GDS_IS1893_2016_H__32C3FF7D_EE7F_4E09_8051_A8E62931CEAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_IS1893_2016.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcIS2016Util;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IS1893_2016 dialog
#include "MyChildDialog.h"
class CETC_GDS_IS1893_2016 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_IS1893_2016(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	void UpdateDampingWnd(CWnd* pDampingWnd);
	BOOL ISVALID();

	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit=TRUE);
	void Dlg2UtilData() override;

// Dialog Data
	//{{AFX_DATA(CETC_GDS_IS1893_2016)
	enum { IDD = IDD_ETC_GDS_IS1893_2016 };
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
	//{{AFX_VIRTUAL(CETC_GDS_IS1893_2016)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcIS2016Util> m_pSpfcUtil;

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_IS1893_2016)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEtcGdsIS1893_2016Damping();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_IS1893_2016_H__32C3FF7D_EE7F_4E09_8051_A8E62931CEAA__INCLUDED_)
