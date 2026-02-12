#if !defined(AFX_ETC_GDS_UBC97_H__E39085CB_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_)
#define AFX_ETC_GDS_UBC97_H__E39085CB_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_UBC97.h : header file
//
#include "MyChildDialog.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcUBC97Util;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_UBC97 dialog

class CETC_GDS_UBC97 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_UBC97(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	BOOL ISVALID();
	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit=TRUE);

// Dialog Data
	//{{AFX_DATA(CETC_GDS_UBC97)
	enum { IDD = IDD_ETC_GDS_UBC97 };
	MComboBox	m_cmbNC;
	MComboBox	m_cmbIF;
	MEdit	m_edtCloseDist;
	MComboBox	m_SourceTypeCmb;
	MComboBox m_SoilTypeCmb;
	MComboBox m_SeisZoneCmb;
	MEdit   m_wndCa;
	MEdit   m_wndCv;
	int     m_nOption;
	CString	m_sIF;
	CString	m_sNC;
	MEdit	  m_wndCloseDist;
	double	m_dMaxPeriod;
	int     m_nSoilType;
	int     m_nSeisZone;
	int     m_nSourceType;
	CString m_sDist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_UBC97)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcUBC97Util> m_pSpfcUtil;

	void AutoCalcSeisCoef();
	void CtrlManager();
	void GetFinalValue();

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_UBC97)	
	virtual BOOL OnInitDialog();
 	afx_msg void OnSelchangeSoiltype();
	afx_msg void OnSelchangeSeisZone();	
	afx_msg void OnSelchangeSourceType();
	afx_msg void OnUbc97AutoRdo();
	afx_msg void OnChangeDist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_UBC97_H__E39085CB_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_)
