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

class CETC_EP_KDS2019 : public CMyChildDialog
{
// Construction
public:
	void End();
	void EndNotClose(){};

	CETC_EP_KDS2019(T_POSL_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	~CETC_EP_KDS2019(); 
	
	mit::frx::MComboBox	m_cmbSeisZone;
	mit::frx::MComboBox	m_cmbSeisZoneFactor;
	mit::frx::MComboBox	m_cmbSiteClass;
	mit::frx::MComboBox	m_cmbUseGroup;
	mit::frx::MComboBox	m_cmbImportance;
	mit::frx::MComboBox	m_cmbRx;

	CEditUnit	m_editFa;
	CEditUnit	m_editFv;
	CEditUnit	m_editSds;
	CEditUnit	m_editSd1;
	CEditUnit	m_editEPGA;
	CEditUnit	m_editKae;

protected:
	T_POSL_D* m_pData;
// Dialog Data
	//{{AFX_DATA(CETC_GDS_KS)
	enum { IDD = IDD_ETC_EP_KDS2019 };
	CArray<UINT, UINT> m_aCtrlRadioMethod;
	CArray<UINT, UINT> m_aCtrlEquMethod;
	CArray<UINT, UINT> m_aCtrlRdmMethod;
	CArray<UINT, UINT> m_aCtrlDisableRdm;
	CArray<UINT, UINT> m_aCtrlEnableRdm;
	//}}AFX_DATA
	BOOL Dlg2Data();
	void Data2Dlg();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_KS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void InitZoneCombo();
	void InitZoneFactorCombo();
	void InitSiteCombo();
	void InitImportanceFactorCombo();
	void InitUseGroupCombo();
	void InitRxRyCombo();

	void SetComboValue();
	void SetFaFvSdsSd1();
	// 	void SetSeisDesignCategory();
	void SetImportanceFactor();
	void SetEquivalentData();
	void SetEquivalentData_Calc();
	void SetRDMData();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_KS)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdZoneCmb();
	afx_msg void OnSelchangeCmdZoneFactorCmb();
	afx_msg void OnSelchangeCmdSiteCmb();
	afx_msg void OnSelchangeCmdUseGroupCmb();
	afx_msg void OnChangeCmdZoneFactor();
	afx_msg void OnChangeCmdFaEdit();
	afx_msg void OnCmdMethodChange();
	afx_msg void OnCmdMethodBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

//#endif // !defined(AFX_ETC_GDS_KS_H__E39085CE_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_)
