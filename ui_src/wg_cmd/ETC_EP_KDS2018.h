// #if !defined(AFX_ETC_GDS_KS_H__E39085CE_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_)
// #define AFX_ETC_GDS_KS_H__E39085CE_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_KS.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MComboBox.h"
#include "MyChildDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KS dialog

class CETC_EP_KDS2018 : public CMyChildDialog
{
// Construction
public:

	void EndNotClose(){};
	CETC_EP_KDS2018(T_POSL_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	~CETC_EP_KDS2018();

	BOOL Dlg2Data();
	void Data2Dlg();

	int    m_nZone;
	double m_dZoneFactor;
	double m_dSeismicRiskFactor;
	int    m_nSite;
	double m_dFa;
	double m_dFv;
	double m_dDampingRatio;

protected:
	T_POSL_D* m_pData;
// Dialog Data
	//{{AFX_DATA(CETC_GDS_KS)
	enum {IDD = IDD_ETC_EP_KDS2018};
	mit::frx::MComboBox	m_cmbSeisZone;
	mit::frx::MComboBox	m_cmbImportance;
	mit::frx::MComboBox	m_cmbSiteClass;
	CEditUnit	m_editDampingRatio;

	CEditUnit m_editFa;
	CEditUnit m_editFv;
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
	void OnSelchangeCmdZoneCmb();
	void OnSelchangeCmdImpCmb();
	void OnSelchangeCmdSiteCmb();
	void OnEditchangeCmdImpCmb();
	void OnSelchangeCmdDampingEdit();
	void SetFaFv();
	void InitZoneCombo();
	void InitImportanceCombo();
	void InitSiteCombo();
	double CalZonFactor(int nZone, double dl);
	void SetPushOverData(BOOL bInit/*=TRUE*/);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

//#endif // !defined(AFX_ETC_GDS_KS_H__E39085CE_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_)
