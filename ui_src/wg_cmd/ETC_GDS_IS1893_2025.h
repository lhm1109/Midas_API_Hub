#if !defined(AFX_ETC_GDS_IS1893_2025_H__32C3FF7D_EE7F_4E09_8051_A8E62931CEAA__INCLUDED_)
#define AFX_ETC_GDS_IS1893_2025_H__32C3FF7D_EE7F_4E09_8051_A8E62931CEAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcIS2025Util;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IS1893_2025 dialog
#include "MyChildDialog.h"
class CETC_GDS_IS1893_2025 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_IS1893_2025(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	void UpdateDampingWnd(CWnd* pDampingWnd);
	BOOL ISVALID();

	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit=TRUE);
	void ShowDlgData();
	void Dlg2UtilData() override;

// Dialog Data
	//{{AFX_DATA(CETC_GDS_IS1893_2025)
	enum { IDD = IDD_ETC_GDS_IS1893_2025 };
	MComboBox   m_cmbSpecType;
	MComboBox   m_cmbEqZone;
	MComboBox	m_cmbRetPeriod;
	CEditUnit	m_wndZoneFact;
	MComboBox	m_cmbSiteClass;
	MComboBox	m_cmbImportanceFact;
	CEditUnit m_edtDamping;
	MComboBox	m_cmbRespReduFact;
	double m_dMaxPeriod;
	double m_dZoneFact;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_IS1893_2025)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	void InitSpectrumTypeCombo();
	void InitEqZoneCombo();
	void InitRetPeriodCombo();
	void InitSiteClassCombo();
	// Implementation
protected:
	T_SPFC_D* m_pParamData;
	T_SPFC_CODE m_CodeD;
	std::shared_ptr<CSpfcIS2025Util> m_pSpfcUtil;

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_IS1893_2025)
	virtual BOOL OnInitDialog();
	void Initialize();
	afx_msg void OnChangeEtcGdsIS1893_2016Damping();
	afx_msg	void OnRetPeriodChange();
	afx_msg void OnSpecTypeChange();
	afx_msg void OnZoneFactorChange();
	afx_msg void OnEqZoneChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_IS1893_2025_H__32C3FF7D_EE7F_4E09_8051_A8E62931CEAA__INCLUDED_)
