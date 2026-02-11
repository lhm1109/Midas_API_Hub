#if !defined(__ETC_GDS_CH_GB_T_51408_2021_H__)
#define __ETC_GDS_CH_GB_T_51408_2021_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_CH_JTG_T_2231_01_2020.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

#include "MyChildDialog.h"

class CSpfcCHGB2021Util;

class CETC_GDS_CH_GB_T_51408_2021 : public CMyChildDialog
{

//Construction
public:
	CETC_GDS_CH_GB_T_51408_2021(T_SPFC_D* pParamData, CWnd* pParent = NULL);
	void End();
	void EndNotClose() {};
	void UpdateDampingWnd(CWnd* pDampingWnd);
	BOOL ISVALID();
	void SetValue();

	// constructor 俊辑 T_SPFC_D m_ParamData狼 林家甫 逞败罐澜.
	BOOL Dlg2Data();
	void Data2Dlg();
	void Initialize();
	void ShowDlgData();
	void SetPushOverData(BOOL bInit = TRUE);
	void Dlg2UtilData() override;

	enum { IDD = IDD_ETC_GDS_CH_GB_T_51408_2021 };
	int			m_wndER;
	int			m_wndSDC;
	int			m_wndSFI;
	int			m_wndSC;
	CEditUnit m_wndTg;
	double	m_dXi;
	double	m_dMaxPeriod;
	int			m_nLForce;
	double  m_dMaxEQE;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_CH_GB_T_51408_2021)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_SPFC_D   *m_pParamData;
	T_SPFC_CODE m_CodeD;
	std::shared_ptr<CSpfcCHGB2021Util> m_pSpfcUtil;

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_CH_GB_T_51408_2021)
	virtual BOOL OnInitDialog();
	afx_msg void OnCalculateTg();
	afx_msg void OnSFIClicked();
	afx_msg void OnERClicked();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void UpdateMaxEQE();
};
#endif