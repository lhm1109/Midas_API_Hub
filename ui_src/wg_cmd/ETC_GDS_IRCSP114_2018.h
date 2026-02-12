#if !defined(AFX_ETC_GDS_IRCSP114_2018_H__32C3FF7D_EE7F_4E09_8051_A8E62931CEAA__INCLUDED_)
#define AFX_ETC_GDS_IRCSP114_2018_H__32C3FF7D_EE7F_4E09_8051_A8E62931CEAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_IRCSP114_2018.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcIRC2018Util;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IRCSP114_2018 dialog
#include "MyChildDialog.h"
class CETC_GDS_IRCSP114_2018 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_IRCSP114_2018(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	void UpdateDampingWnd(CWnd* pDampingWnd);
	BOOL ISVALID();


	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit=TRUE);
	void Dlg2UtilData() override;

// Dialog Data
	//{{AFX_DATA(CETC_GDS_IRCSP114_2018)
	enum { IDD = IDD_ETC_GDS_IRCSP114_2018 };
	MComboBox	m_wndIs02RCmb;
	MComboBox	m_wndIs02ImportanceCmb;
	CEditUnit m_wndIs02Damping;
	CEditUnit m_wndIs02DampingFactor;
	CEditUnit m_wndUserDefSeisZone;
	int		m_nZone;
	int		m_nSoil;
	double m_dMaxPeriod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_IRCSP114_2018)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcIRC2018Util> m_pSpfcUtil;

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_IRCSP114_2018)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEtcGdsIs2002Damping();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedEtcGdsIs2002ZoneRdo3();
	afx_msg void OnBnClickedEtcGdsIs2002ZoneRdo5();
	afx_msg void SetCorrectRadButS(int iZF);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_IRCSP114_2018_H__32C3FF7D_EE7F_4E09_8051_A8E62931CEAA__INCLUDED_)
