#if !defined(AFX_ETC_GDS_KISTEC2013_H__764B3BE8_32DC_43AF_9CE2_0E11FB2C9D56__INCLUDED_)
#define AFX_ETC_GDS_KISTEC2013_H__764B3BE8_32DC_43AF_9CE2_0E11FB2C9D56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_KISTEC2013.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KISTEC2013 dialog
#include "MyChildDialog.h"

class CETC_GDS_KISTEC2013 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_KISTEC2013(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	void MakeSpectrumData(BOOL bOnlyCalc=FALSE);
	void MakePOSpectrumData(double dDamping,double SRa,double SRv,CArray <double,double>& aPeriod,CArray <double,double>& aAccel);// for Pushover Curve
	BOOL ISVALID();
	void SetValue();
	double GetResModCoef();
	void UpdateDlgData();

	BOOL Dlg2Data();
	virtual void Data2Dlg();

	int    m_nZone;
	double m_dZoneFactor;
	int    m_nSite;
	double m_dT0;
	double m_dTs;
	double m_dFa;
	double m_dFv;
	double m_dSds;
	double m_dSd1;
	double m_dR;
	double m_dI;
	double m_dC;
	BOOL   m_bRelieve;
 	double m_dMaxPeriod;
// Dialog Data
	//{{AFX_DATA(CETC_GDS_KISTEC2013)
	enum { IDD = IDD_ETC_GDS_KISTEC2013 };
	MComboBox	m_cmbSeisZone;
	MComboBox m_cmbSeisZoneFactor;
	MComboBox	m_cmbSiteClass;
	CEditUnit m_editFa;
	CEditUnit m_editFv;
	CEditUnit	m_editSds;
	CEditUnit	m_editSd1;
	CEditUnit	m_editC;
	MComboBox	m_cmbR;
	MComboBox	m_cmbImportance;
	MButton	  m_btnAccRelieve;
	CEditUnit m_editMaxPeriod;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_KISTEC2013)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private :

protected:
	T_SPFC_D* m_pParamData;

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
	//{{AFX_MSG(CETC_GDS_KISTEC2013)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdAccMethod();
	afx_msg void OnSelchangeCmdZoneCmb();
	afx_msg void OnSelchangeCmdZoneFactorCmb();
	afx_msg void OnSelchangeCmdSiteCmb();
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

#endif // !defined(AFX_ETC_GDS_KISTEC2013_H__764B3BE8_32DC_43AF_9CE2_0E11FB2C9D56__INCLUDED_)
