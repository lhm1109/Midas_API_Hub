#if !defined(AFX_ETC_GDS_NSR2010_H_)
#define AFX_ETC_GDS_NSR2010_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_IBC2012.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcNSR2010Util;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_NSR2010 dialog
#include "MyChildDialog.h"

class CETC_GDS_NSR2010 : public CMyChildDialog
{
	// Construction
public:
	CETC_GDS_NSR2010(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	BOOL ISVALID();
	void SetValue();
	double GetResModCoef();

	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit=TRUE);
	void Dlg2UtilData() override;


	// Dialog Data
	//{{AFX_DATA(CETC_GDS_NSR2010)
	enum { IDD = IDD_ETC_GDS_NSR2010 };
	MComboBox	m_cmbAA;
	MComboBox m_cmbAV;
	MComboBox	m_cmbSiteClass;
	CEditUnit m_editFa;
	CEditUnit m_editFv;
	MComboBox	m_cmbR;
	MComboBox	m_cmbImportance;
	CEditUnit m_editMaxPeriod;
	CEditUnit m_editPhi;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_NSR2010)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
private :

protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcNSR2010Util> m_pSpfcUtil;

	void InitAaCombo();
	void InitAvCombo();
	void InitSiteCombo();
	void InitIFactorCombo();  
	void InitRCombo();

	void SetFa();
	void SetFv();

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_NSR2010)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdAaCmb();
	afx_msg void OnSelchangeCmdAvCmb();
	afx_msg void OnSelchangeCmdSiteCmb();
	afx_msg void OnChangeCmdAa();
	afx_msg void OnChangeCmdAv();	
	afx_msg void OnPhiBtn();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_NSR2010_H_)
