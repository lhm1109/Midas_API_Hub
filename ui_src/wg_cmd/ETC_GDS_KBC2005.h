#if !defined(AFX_ETC_GDS_KBC2005_H__764B3BE8_32DC_43AF_9CE2_0E11FB2C9D56__INCLUDED_)
#define AFX_ETC_GDS_KBC2005_H__764B3BE8_32DC_43AF_9CE2_0E11FB2C9D56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_KBC2005.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcKBC2005Util;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KBC2005 dialog
#include "MyChildDialog.h"

class CETC_GDS_KBC2005 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_KBC2005(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	BOOL ISVALID();
	void SetValue();
	BOOL Dlg2Data();
	virtual void Data2Dlg();
	void SetPushOverData(BOOL bInit=TRUE);

// Dialog Data
	//{{AFX_DATA(CETC_GDS_KBC2005)
	enum { IDD = IDD_ETC_GDS_KBC2005 };
	MComboBox	m_cmbSeisZone;
	MComboBox	m_cmbSiteClass;
	CEditUnit	m_editSds;
	CEditUnit	m_editSd1;
	MComboBox	m_cmbR;
	MComboBox	m_cmbImportance;
	MButton	  m_btnAccRelieve;
	CEditUnit m_editMaxPeriod;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_KBC2005)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private :

protected:
	CArray<UINT, UINT> m_aCtrlRadioAcc;
	CArray<UINT, UINT> m_aCtrlPerCode, m_aCtrlUserDefined;  

	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcKBC2005Util> m_pSpfcUtil;
protected:
	void InitZoneCombo();
	void InitSiteCombo();
	void InitIFactorCombo();  
	void InitRCombo();

	void SetSdsSd1();
	void CtrlManager();

	void GetFinalValue();
	void SetInitValueforPOSpectrum();
	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_KBC2005)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdAccMethod();
	afx_msg void OnSelchangeCmdSiteCmb();
	afx_msg void OnSelchangeCmdZoneCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_KBC2005_H__764B3BE8_32DC_43AF_9CE2_0E11FB2C9D56__INCLUDED_)
