#if !defined(AFX_ETC_GDS_NBC95_H__529AF0EA_4678_45C0_B3F0_13175A35C501__INCLUDED_)
#define AFX_ETC_GDS_NBC95_H__529AF0EA_4678_45C0_B3F0_13175A35C501__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_NBC95.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_NBC95 dialog
#include "MyChildDialog.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcNBC95Util;
using namespace mit::frx;

class CETC_GDS_NBC95 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_NBC95(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	BOOL ISVALID();
	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit=TRUE);

// Dialog Data
	//{{AFX_DATA(CETC_GDS_NBC95)
	enum { IDD = IDD_ETC_GDS_NBC1995 };
	MComboBox	m_wndZoneRatio;
	MComboBox	m_wndVeloZone;
	MComboBox	m_wndModification;
	MComboBox	m_wndImportance;
	MComboBox	m_wndFoundation;
	MComboBox	m_wndAcceZone;
	double	m_dMaxPeriod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_NBC95)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcNBC95Util> m_pSpfcUtil;
	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_NBC95)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_NBC95_H__529AF0EA_4678_45C0_B3F0_13175A35C501__INCLUDED_)
