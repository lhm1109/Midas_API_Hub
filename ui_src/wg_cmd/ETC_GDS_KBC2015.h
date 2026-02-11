#if !defined(AFX_ETC_GDS_KBC2015_H__764B3BE8_32DC_43AF_9CE2_0E11FB2C9D56__INCLUDED_)
#define AFX_ETC_GDS_KBC2015_H__764B3BE8_32DC_43AF_9CE2_0E11FB2C9D56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_KBC2015.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcKBC2016Util;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KBC2015 dialog
#include "MyChildDialog.h"

class CETC_GDS_KBC2015 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_KBC2015(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	BOOL ISVALID();
	void SetValue();
	double GetResModCoef();

	BOOL Dlg2Data();
	virtual void Data2Dlg();
	void SetPushOverData(BOOL bInit=TRUE);

// Dialog Data
	//{{AFX_DATA(CETC_GDS_KBC2015)
	enum { IDD = IDD_ETC_GDS_KBC2015 };
	MComboBox	m_cmbSeisZone;
	MComboBox m_cmbSeisZoneFactor;
	MComboBox	m_cmbSiteClass;
	CTextUnit	m_unitDepthToMR;
	CEditUnit m_editDepthToMR;
	CEditUnit m_editFa;
	CEditUnit m_editFv;
	CEditUnit	m_editSds;
	CEditUnit	m_editSd1;
	MComboBox	m_cmbR;
	MComboBox	m_cmbImportance;
	MButton	  m_btnAccRelieve;
	CEditUnit m_editMaxPeriod;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_KBC2015)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private :

protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcKBC2016Util> m_pSpfcUtil;

	void InitZoneCombo();
	void InitZoneFactorCombo();
	void InitSiteCombo();
	void InitIFactorCombo();  
	void InitRCombo();

	void SetComboValue();

	void SetFaFvSdsSd1();
	//void CtrlManager();

	void GetFinalValue();
	void SetInitValueforPOSpectrum();
	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_KBC2015)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdAccMethod();
	afx_msg void OnSelchangeCmdZoneCmb();
	afx_msg void OnSelchangeCmdZoneFactorCmb();
	afx_msg void OnSelchangeCmdSiteCmb();
	afx_msg void OnChangeCmdDepthToMREdit();
	afx_msg void OnChangeCmdFaEdit();
	afx_msg void OnChangeCmdFvEdit();
	afx_msg void OnChangeCmdSdsEdit();
	afx_msg void OnChangeCmdSd1Edit();
	afx_msg void OnChangeCmdZoneFactor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_KBC2015_H__764B3BE8_32DC_43AF_9CE2_0E11FB2C9D56__INCLUDED_)
