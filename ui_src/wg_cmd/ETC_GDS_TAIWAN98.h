#if !defined(__ETC_GDS_TAIWAN98_H__)
#define __ETC_GDS_TAIWAN98_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CETC_GDS_TAIWAN98.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "ETC_GDS_TAIWAN02.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcTAIWAN98Util;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_TAIWAN98 dialog
#include "MyChildDialog.h"

class CETC_GDS_TAIWAN98 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_TAIWAN98(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	  
	BOOL ISVALID();
	void End();
	void EndNotClose(){};
	void SetValue();  
	BOOL Dlg2Data();

// Dialog Data
	//{{AFX_DATA(CETC_GDS_TAIWAN98)
	enum { IDD = IDD_ETC_GDS_TAIWAN98 };
	MComboBox m_cmbSpecCategory;
	MComboBox	m_cmbSpecType;
	MComboBox	m_cmbSeisZone;
	MComboBox	m_cmbSoilType;
	MComboBox	m_cmbSubZone;
	MComboBox	m_cmbSds;
	MComboBox	m_cmbSd1;
	MComboBox	m_cmbSms;
	MComboBox	m_cmbSm1;
	CEditUnit	m_edtFmv;
	CEditUnit	m_edtFma;
	CEditUnit	m_edtFdv;
	CEditUnit	m_edtFda;	
	CEditUnit	m_edtNmv;
	CEditUnit	m_edtNma;
	CEditUnit	m_edtNdv;
	CEditUnit	m_edtNda;	
	CEditUnit	m_edtSds_t;  
	CEditUnit	m_edtSms_t;
	CEditUnit	m_edtTm0;
	CEditUnit	m_edtTd0;	
	MComboBox	m_cmbI;
	MComboBox	m_cmbAlpha;
	MComboBox	m_cmbR;
	CEditUnit	m_edtDamping;
	CEditUnit	m_edtMaxPeriod;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_TAIWAN98)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

public:
	void SetPushOverData(BOOL bInit=TRUE);

// Implementation
protected:  
	void SetInitValueforPOSpectrum();  

	//Dialog Setting
	void InitSpecAcc();
	void InitSiteMagnifyFactor();
	void InitTaipai();
	void InitIFactorCombo();
	void InitAyFactorCombo();
	void InitRCombo();
	
	void SetSpectralData();
	void SetSiteMagnifyFactor();
	void SetChangeTaipai();
	void GetFinalValue();
	void CtrlManager();          // MNET:2410-GSJI-20060908 Modify~  
	void AlignCtrl();
	
	CArray<UINT, UINT> m_aCtrlMethod0;
	CArray<UINT, UINT> m_aCtrlMethod1;
	CArray<UINT, UINT> m_aCtrlMethod2;
	CArray<UINT, UINT> m_aCtrl1, m_aCtrl2;    // MNET:2410-GSJI-20060829

	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcTAIWAN98Util> m_pSpfcUtil;

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_TAIWAN98)  
	afx_msg void OnSelchangeCmdTaiwan02SeisZoneCmb();
	afx_msg void OnSelchangeCmdTaiwan02SpecTypeCmb();
	afx_msg void OnEditchangeCmdTaiwan02SpecAccCmb();
	afx_msg void OnSelchangeCmdTaiwan02SpecAccCmb();
	afx_msg void OnSelchangeCmdTaiwan02SubzoneCmb();
	afx_msg void OnSelchangeCmdTaiwan02SoilTypeCmb();
	afx_msg void OnEditchangeCmdTaiwan02NSFactorEdt();
	afx_msg void OnEditchangeCmdTaiwan02SMFactorEdt();
	afx_msg void OnEditchangeCmdTaiwan02TaipaiEdt();
	afx_msg void OnSelSpecCategoryCmb();                   // MNET:2410-GSJI-20060829
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ETC_GDS_TAIWAN98_H__)


