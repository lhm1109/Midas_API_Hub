#if !defined(__ETC_GDS_GB50111_2006_H__)
#define __ETC_GDS_GB50111_2006_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_GB50111_2006.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MButton.h"

class CSpfcGB50111_2006Util;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_GB50111_2006 dialog
#include "MyChildDialog.h"

class CETC_GDS_GB50111_2006 :  public CMyChildDialog
{
// Construction
public:
	CETC_GDS_GB50111_2006(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	//void UpdateAlphaWnd(CWnd* pAlphaWnd);
	BOOL ISVALID();
	BOOL IsValid_d(CEdit* pEdt);
	//void SetValue();
	//double m_dAlphaMax;

	CEditUnit m_edtTg;
	CEditUnit m_edtG;
	MButton   m_chkBridge;

	int			m_nEarthEff;
	int			m_nDivision; 
	int			m_nSFI;
	int			m_nSiteClass;

	double	m_dMaxPeriod;

	// MNET:2138-GSJI-20060526
	// constructor 에서 T_SPFC_D m_ParamData의 주소를 넘겨받음.
	BOOL Dlg2Data();
	void Data2Dlg();
	void Initialize();  
	void ShowDlgData();
	void Dlg2UtilData() override;
	
// Dialog Data
	//{{AFX_DATA(CETC_GDS_GB50111_2006)
	enum { IDD = IDD_ETC_GDS_GB50111_2006 };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_GB50111_2006)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_SPFC_D   *m_pParamData;
	T_SPFC_CODE m_CodeD;
	std::shared_ptr<CSpfcGB50111_2006Util> m_pSpfcUtil;

// Implementation
protected:  
	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_GB50111_2006)
	virtual BOOL OnInitDialog();
	afx_msg void OnCalculateTg();
	afx_msg void OnCalculateG();
	afx_msg void OnClickBtnBR();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ETC_GDS_GB50111_2006_H__)
