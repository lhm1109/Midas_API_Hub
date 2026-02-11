#if !defined(AFX_ETC_GDS_KSCE2019_H__INCLUDED_)
#define AFX_ETC_GDS_KSCE2019_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_KSCE2019.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcKSCE2019Util;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KSCE2019 dialog
#include "MyChildDialog.h"

class CETC_GDS_KSCE2019 : public CMyChildDialog
{
	// Construction
public:
	CETC_GDS_KSCE2019(T_SPFC_D* pParamData, BOOL bUseTs, CWnd* pParent = NULL);   // standard constructor
	void End();
	//void EndNotClose(){};
	BOOL ISVALID();
	//void SetValue();
	//double GetResModCoef();

	BOOL Dlg2Data();
	virtual void Data2Dlg();
	void SetPushOverData(BOOL bInit=TRUE);
	double GetTs() const;

	// Dialog Data
	//{{AFX_DATA(CETC_GDS_KSCE2019)
	enum { IDD = IDD_ETC_GDS_KSCE2019 };
	MComboBox	m_cmbSeisZone;
	MComboBox	m_cmbImportance;
	//MComboBox m_cmbSeisZoneFactor;	
	MComboBox	m_cmbSiteClass;

	CEditUnit m_editFa;
	CEditUnit m_editFv;
	MButton	  m_btnAccRelieve;
	CEditUnit m_editMaxPeriod;
	CEditUnit m_edtTs;
	CTextUnit m_untTs;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_KSCE2019)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
private :

protected:
	T_SPFC_D* m_pParamData;
	BOOL m_bUseTs;
	std::shared_ptr<CSpfcKSCE2019Util> m_pSpfcUtil;

	void InitUnit();
	void InitZoneCombo();
	void InitIFactorCombo();  
	void InitSiteCombo();
	void AlignControls();
	void ShowHideControls();

	void SetFaFv();

	void GetFinalValue();
	void SetInitValueforPOSpectrum();
	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_KSCE2019)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdZoneCmb();
	afx_msg void OnSelchangeCmImpFactorCmb();
	afx_msg void OnEditchangeCmImpFactorCmb();
	afx_msg void OnSelchangeCmdSiteCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_KSCE2019_H__INCLUDED_)
