#if !defined(AFX_ETC_GDS_SANS_H__476A66EB_73C3_47D8_9DCA_9D28C784E0D1__INCLUDED_)
#define AFX_ETC_GDS_SANS_H__476A66EB_73C3_47D8_9DCA_9D28C784E0D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_SANS.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_SANS dialog
#include "MyChildDialog.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CETC_GDS_SANS : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_SANS(T_SPFC_D* pParamData = NULL, CWnd* pParent = NULL, int nCase=0);   // standard constructor
	void End();
	void EndNotClose();
	void UpdateDampingWnd(CWnd* pDampingWnd);
	void MakeSpectrumData(BOOL bOnlyCalc=FALSE);
	void MakePOSpectrumData(double dDamping,double SRa,double SRv,CArray <double,double>& aPeriod,CArray <double,double>& aAccel);// for Pushover Curve	
	BOOL ISVALID();
	void SetDefaultValue();

	int    m_nSpecType;
	int    m_nGroundType;
	double m_dAg;

	double m_dS;
	double m_dTb;
	double m_dTc;
	double m_dTd;
	double m_dDamping;
	double m_dQ;
	double m_dBeta;
	double m_dImportance;

	int    m_nRegion;
	

	// constructor 에서 T_SPFC_D m_ParamData의 주소를 넘겨받음.
	BOOL Dlg2Data();
	void Dlg2Data(T_SPFC_SANS2010& EURO2004);
	void Data2Dlg();
	void SetNationalAnnex(int nNAIndex); // 0; recommemded,  1: Singapore   2: Malaysia
	void Initialize();  
	void ShowDlgData();
	void SetPushOverData(BOOL bInit=TRUE);
	void SetPushOverDataMal(BOOL bInit=TRUE);

// Dialog Data
	//{{AFX_DATA(CETC_GDS_SANS)
	enum { IDD = IDD_ETC_GDS_SANS };
	CEditUnit	m_wndTd;
	CEditUnit	m_wndTc;
	CEditUnit	m_wndTb;
	CEditUnit	m_wndS;
	MComboBox	m_SpecTypeCmb;
	MComboBox	m_GroundTypeCmb;
	MComboBox	m_cmbImportance;
	CEditUnit	m_wndDamping;
	CEditUnit	m_wndBeta;
	CEditUnit	m_wndQ;
	CEditUnit	m_wndAg;

// 	CEditUnit	m_wndTd_Mal;
// 	CEditUnit	m_wndTc_Mal;
// 	CEditUnit	m_wndTb_Mal;
// 	CEditUnit	m_wndS_Mal;
// 	CEditUnit	m_wndTs_Mal;
	MComboBox	m_RegionMalCmb;
// 	CComboBox	m_cmbImportance_Mal;
// 	CEditUnit	m_wndDamping_Mal;
// 	CEditUnit	m_wndBeta_Mal;
// 	CEditUnit	m_wndQ_Mal;

	double	m_dMaxPeriod;			
	int		m_nParaTypeRdo;
	int   m_nCase; // 0: 일반, 1: Elastic Sepctrum 삭제, 2: Design Spectrum 삭제
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_SANS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_SPFC_D   *m_pParamData;
	T_SPFC_CODE m_CodeD;
	//BOOL m_bSingapore;
	int m_nNAIndex;

// Implementation
protected:
	CArray<UINT, UINT> m_aCtrlElastic, m_aCtrlDesign, m_aCtrlParaType;
	CArray<UINT, UINT> m_aCtrlRegionType, m_aCtrlMove;
	CArray<UINT, UINT> m_aCtrlGround;
	void CtrlManager();
	
	void InitCtrlArray();
	void AlignControl();
	void InitSpectrumTypeCombo();
	void InitGroundTypeCombo();
	void InitRegionCombo();
	void InitIFactorCombo(BOOL bInit=FALSE);
	void AlignControlByNA();
	//
	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_SANS)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeEtcGdsSANSSpecTypeCmb();
	afx_msg void OnEtcGdsSANSParatypeRdo();
	afx_msg void OnChangeTsEdit();
	afx_msg void OnSelchangeEtcGdsSANSGroundCmb();
	afx_msg void OnSelchangeEtcGdsSANSRegionCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_SANS_H__476A66EB_73C3_47D8_9DCA_9D28C784E0D1__INCLUDED_)
