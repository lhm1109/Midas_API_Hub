#if !defined(AFX_ETC_GDS_CH2002_H__71ED3C24_0435_4112_9E91_5435AD95C0A7__INCLUDED_)
#define AFX_ETC_GDS_CH2002_H__71ED3C24_0435_4112_9E91_5435AD95C0A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_CH2002.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_CH2002 dialog
#include "MyChildDialog.h"

class CSpfcCH2002Util;

class CETC_GDS_CH2002 :  public CMyChildDialog
{
// Construction
public:
	CETC_GDS_CH2002(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	void UpdateDampingWnd(CWnd* pDampingWnd);
	BOOL ISVALID();
	void SetValue();
	
	// MNET:2138-GSJI-20060526
	// constructor 에서 T_SPFC_D m_ParamData의 주소를 넘겨받음.
	BOOL Dlg2Data();
	void Data2Dlg();
	void Initialize();  
	void ShowDlgData();
	void SetPushOverData(BOOL bInit=TRUE);
	void Dlg2UtilData() override;

// Dialog Data
	//{{AFX_DATA(CETC_GDS_CH2002)
	enum { IDD = IDD_ETC_GDS_CH2002 };
	int			m_wndER;
	int			m_wndSDC; 
	int			m_wndSFI;
	int			m_wndSC;
	CEditUnit m_wndTg;
	double	m_dXi;
	double	m_dMaxPeriod;
	double  m_dMaxEQE;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_CH2002)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_SPFC_D   *m_pParamData;
	T_SPFC_CODE m_CodeD;
	std::shared_ptr<CSpfcCH2002Util> m_pSpfcUtil;

// Implementation
protected:  
	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_CH2002)
	virtual BOOL OnInitDialog();
	afx_msg void OnCalculateTg();
	afx_msg void OnSFIClicked();
	afx_msg void OnERClicked();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void UpdateMaxEQE();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_CH2002_H__71ED3C24_0435_4112_9E91_5435AD95C0A7__INCLUDED_)
