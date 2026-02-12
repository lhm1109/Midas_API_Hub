#if !defined(__ETC_GDS_CH_CJJ166_2011_H__)
#define __ETC_GDS_CH_CJJ166_2011_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_CH_CJJ166_2011.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MillustViewer.h"

class CSpfcCHCJJ2011Util;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_CH_CJJ166_2011 dialog
#include "MyChildDialog.h"

class CETC_GDS_CH_CJJ166_2011 :  public CMyChildDialog
{
// Construction
public:
	CETC_GDS_CH_CJJ166_2011(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	BOOL ISVALID();
	BOOL IsValid_d(CEdit* pEdt);

	CEditUnit m_edtDampingRatio;
	CEditUnit m_edtTg;
	CEditUnit m_edtEta1;
	CEditUnit m_edtEta2;
	CEditUnit m_edtGamma;
	CEditUnit m_edtCi;
	CEditUnit m_edtA;
	CEditUnit m_edtSMax;
	CEditUnit m_edtMaxPeriod;

	MButton m_chkE2_005;
	MButton m_chkVertAccSpec;

	MillustViewer m_ImgViewer;

	// constructor 에서 T_SPFC_D m_ParamData의 주소를 넘겨받음.
	BOOL Dlg2Data();
	void Data2Dlg();
	void Initialize();  
	void ShowDlgData();
	void Dlg2UtilData() override;
	void Button2UtilData();
	void UtilData2Button();
	
// Dialog Data
	//{{AFX_DATA(CETC_GDS_CH_CJJ166_2011)
	enum { IDD = IDD_ETC_GDS_CH_CJJ166_2011};

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_CH_CJJ166_2011)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_SPFC_D   *m_pParamData;
	T_SPFC_CODE m_CodeD;
	std::shared_ptr<CSpfcCHCJJ2011Util> m_pSpfcUtil;

// Implementation
protected:
	CArray<UINT, UINT> m_aSeisType;
	CArray<UINT, UINT> m_aZoningMap;
	CArray<UINT, UINT> m_aSiteTpye;
	CArray<UINT, UINT> m_aSeismicIntensity;
	CArray<UINT, UINT> m_aSeismicE;

protected:  
	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_CH_CJJ166_2011)
	virtual BOOL OnInitDialog();
	afx_msg void OnCalcOutputData();
	afx_msg void OnChaneInputData();
	afx_msg void OnChaneOutputData();
	afx_msg void OnClickVertAccSpec();
	afx_msg void OnClickE2_005();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ETC_GDS_CH_CJJ166_2011_h__)
