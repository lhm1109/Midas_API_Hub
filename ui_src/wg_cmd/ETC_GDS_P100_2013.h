#if !defined(AFX_ETC_GDS_P100_2013_H__476A66EB_73C3_47D8_9DCA_9D28C784E0D1__INCLUDED_)
#define AFX_ETC_GDS_P100_2013_H__476A66EB_73C3_47D8_9DCA_9D28C784E0D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_P100_2013.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcPO2013Util;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_P100_2013 dialog
#include "MyChildDialog.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CETC_GDS_P100_2013 : public CMyChildDialog
{
	// Construction
public:
	CETC_GDS_P100_2013(T_SPFC_D* pParamData = NULL, CWnd* pParent = NULL, int nCase=0);   // standard constructor
	void End();
	void EndNotClose();
	void UpdateDampingWnd(CWnd* pDampingWnd);
	BOOL ISVALID();


	// constructor 에서 T_SPFC_D m_ParamData의 주소를 넘겨받음.
	BOOL Dlg2Data();
	void Data2Dlg();
	void Initialize();  
	void ShowDlgData();
	void SetPushOverData(BOOL bInit=TRUE);
	void Dlg2UtilData() override;

	// Dialog Data
	//{{AFX_DATA(CETC_GDS_P100_2013)
	enum { IDD = IDD_ETC_GDS_P100_2013 };
	CEditUnit	m_wndTd;
	CEditUnit	m_wndTc;
	CEditUnit	m_wndTb;
	MComboBox	m_SpecTypeCmb;
	MComboBox	m_cmbImportance;
	CEditUnit	m_wndDamping;
	CEditUnit	m_wndQ;
	CEditUnit	m_wndAg;
	double	m_dMaxPeriod;			
	int		m_nParaTypeRdo;
	int   m_nCase; // 0: 일반, 1: Elastic Sepctrum 삭제, 2: Design Spectrum 삭제
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_P100_2013)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_SPFC_D   *m_pParamData;
	T_SPFC_CODE m_CodeD;
	std::shared_ptr<CSpfcPO2013Util> m_pSpfcUtil;

	BOOL m_bSingapore;


	// Implementation
protected:
	CArray<UINT, UINT> m_aCtrlElastic, m_aCtrlDesign, m_aCtrlParaType;
	void CtrlManager();
	void AlignControl();
	void InitSpectrumTypeCombo();
	void InitIFactorCombo();
	void InitCtrl();
	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_P100_2013)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeEtcGdsP100_2013SpecTypeCmb();
	afx_msg void OnEtcGdsP100_2013ParatypeRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_P100_2013_H__476A66EB_73C3_47D8_9DCA_9D28C784E0D1__INCLUDED_)
