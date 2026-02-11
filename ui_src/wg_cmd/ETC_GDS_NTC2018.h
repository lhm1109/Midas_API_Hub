#if !defined(AFX_ETC_GDS_NTC2018_H__476A66EB_73C3_47D8_9DCA_9D28C784E0D1__INCLUDED_)
#define AFX_ETC_GDS_NTC2018_H__476A66EB_73C3_47D8_9DCA_9D28C784E0D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_NTC2018.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcNTC2018Util;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_NTC2018 dialog
#include "MyChildDialog.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CETC_GDS_NTC2018 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_NTC2018(T_SPFC_D* pParamData = NULL, CWnd* pParent = NULL, int nCase=0);   // standard constructor
	void End();
	void EndNotClose();
	void UpdateDampingWnd(CWnd* pDampingWnd);
	BOOL ISVALID();
	void SetDefaultValue();

	// constructor 에서 T_SPFC_D m_ParamData의 주소를 넘겨받음.
	BOOL Dlg2Data();
	void Dlg2Data(T_SPFC_NTC2018& NTC2018);
	void Data2Dlg();
	void Initialize();  
	void ShowDlgData();
	void SetPushOverData(BOOL bInit=TRUE);

// Dialog Data
	//{{AFX_DATA(CETC_GDS_NTC2018)
	enum { IDD = IDD_ETC_GDS_NTC2018 };
	CEditUnit	m_wndTd;
	CEditUnit	m_wndTc;
	CEditUnit	m_wndTb;
	CEditUnit	m_wndS;
	MComboBox	m_SpecTypeCmb;
	MComboBox	m_GroundTypeCmb;
	CEditUnit	m_wndAg;
	CEditUnit	m_wndFo;
	CEditUnit	m_wndTcStar;
	CEditUnit	m_wndDamping;
	CEditUnit	m_wndQ;
	double	m_dMaxPeriod;			
	int		m_nParaTypeRdo;
	int   m_nCase; // 0: 일반, 1: Elastic Sepctrum 삭제, 2: Design Spectrum 삭제
	//}}AFX_DATA

	enum EN_SPEC_TYPE
	{
		EN_HORI_ELASTIC, EN_VERT_ELASTIC, EN_HORI_DESIGN, EN_VERT_DESIGN
	};
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_NTC2018)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_SPFC_D   *m_pParamData;
	T_SPFC_CODE m_CodeD;
	std::shared_ptr<CSpfcNTC2018Util> m_pSpfcUtil;

// Implementation
protected:
	CArray<UINT, UINT> m_aCtrlElastic, m_aCtrlDesign;
	void CtrlManager();
	void AlignControl();
	void InitSpectrumTypeCombo();
	void InitGroundTypeCombo();
	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_NTC2018)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeEtcGdsNTC2018SpecTypeCmb();
	afx_msg void OnEtcGdsNTC2018ParatypeRdo();
	afx_msg void OnSelchangeEtcGdsNTC2018GroundCmb();
	afx_msg void OnChangeCmdAgEdit();
	afx_msg void OnChangeCmdAmpEdit();
	afx_msg void OnChangeCmdTcStarEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_NTC2018_H__476A66EB_73C3_47D8_9DCA_9D28C784E0D1__INCLUDED_)
