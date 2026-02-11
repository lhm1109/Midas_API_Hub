#if !defined(AFX_ETC_GDS_IBC2012_H__764B3BE8_32DC_43AF_9CE2_0E11FB2C9D56__INCLUDED_)
#define AFX_ETC_GDS_IBC2012_H__764B3BE8_32DC_43AF_9CE2_0E11FB2C9D56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_IBC2012.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcIBC2012Util;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IBC2012 dialog
#include "MyChildDialog.h"

class CETC_GDS_IBC2012 : public CMyChildDialog
{
	// Construction
public:
	CETC_GDS_IBC2012(T_SPFC_D* pParamData, CWnd* pParent = NULL, int codeType = D_SPFC_CODE_IBC2012);   // standard constructor
	void End();
	void EndNotClose() {};
	BOOL ISVALID();
	void SetValue();
	double GetResModCoef();

	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit = TRUE);

	// Dialog Data
		//{{AFX_DATA(CETC_GDS_IBC2012)
	enum { IDD = IDD_ETC_GDS_IBC2012 };
	MComboBox	m_cmbSs;
	MComboBox m_cmbS1;
	MComboBox	m_cmbSiteClass;
	CEditUnit m_editFa;
	CEditUnit m_editFv;
	CEditUnit	m_editSds;
	CEditUnit	m_editSd1;
	MComboBox	m_cmbR;
	MComboBox	m_cmbImportance;
	CEditUnit m_editMaxPeriod;
	CEditUnit m_editTL;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_IBC2012)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:	
	T_SPFC_IBC2012* GetSpfcCodeParam();

protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcIBC2012Util> m_pSpfcUtil;

	void InitSsCombo();
	void InitS1Combo();
	void InitSiteCombo();
	void InitIFactorCombo();  
	void InitRCombo();
	
	void SetFaFvSdsSd1();

	void GetFinalValue();
	void SetInitValueforPOSpectrum();
	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_IBC2012)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdSsCmb();
	afx_msg void OnSelchangeCmdS1Cmb();
	afx_msg void OnSelchangeCmdSiteCmb();
	afx_msg void OnChangeCmdFaEdit();
	afx_msg void OnChangeCmdFvEdit();  
	afx_msg void OnChangeCmdSs();
	afx_msg void OnChangeCmdS1();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_IBC2012_H__764B3BE8_32DC_43AF_9CE2_0E11FB2C9D56__INCLUDED_)
