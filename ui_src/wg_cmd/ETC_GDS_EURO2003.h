#if !defined(AFX_ETC_GDS_EURO2003_H__476A66EB_73C3_47D8_9DCA_9D28C784E0D1__INCLUDED_)
#define AFX_ETC_GDS_EURO2003_H__476A66EB_73C3_47D8_9DCA_9D28C784E0D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_EURO2003.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;
class CSpfcEURO2004Util;
/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_EURO2003 dialog
#include "MyChildDialog.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CETC_GDS_EURO2003 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_EURO2003(T_SPFC_D* pParamData = NULL, CWnd* pParent = NULL, int nCase=0);   // standard constructor
	void End();
	void EndNotClose();
	void UpdateDampingWnd(CWnd* pDampingWnd);
	BOOL ISVALID();
	void SetDefaultValue();


	// constructor 에서 T_SPFC_D m_ParamData의 주소를 넘겨받음.
	BOOL Dlg2Data();
	void Dlg2Data(T_SPFC_EURO2004& EURO2004);
	void Data2Dlg();
	void SetNationalAnnex(int nNAIndex); // 0; recommemded,  1: Singapore   2: Malaysia
	void Initialize();  
	void ShowDlgData();
	void SetPushOverData(BOOL bInit=TRUE);
	void SetPushOverDataMal(BOOL bInit=TRUE);
	void Dlg2UtilData() override;

// Dialog Data
	//{{AFX_DATA(CETC_GDS_EURO2003)
	enum { IDD = IDD_ETC_GDS_EURO2003 };
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

	MComboBox	m_RegionMalCmb;

	double	m_dMaxPeriod;			
	int		m_nParaTypeRdo;
	int   m_nCase; // 0: 일반, 1: Elastic Sepctrum 삭제, 2: Design Spectrum 삭제
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_EURO2003)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_SPFC_D   *m_pParamData;
	T_SPFC_CODE m_CodeD;
	std::shared_ptr<CSpfcEURO2004Util> m_pSpfcUtil;
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
	//{{AFX_MSG(CETC_GDS_EURO2003)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeEtcGdsEuro2003SpecTypeCmb();
	afx_msg void OnEtcGdsEuro2003ParatypeRdo();
	//afx_msg void OnChangeTsEdit();
	afx_msg void OnSelchangeEtcGdsEuro2003GroundCmb();
	afx_msg void OnSelchangeEtcGdsEuro2003RegionCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_EURO2003_H__476A66EB_73C3_47D8_9DCA_9D28C784E0D1__INCLUDED_)
