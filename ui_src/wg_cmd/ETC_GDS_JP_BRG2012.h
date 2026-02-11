#if !defined(AFX_ETC_GDS_JP_BRG2012_H__08005CA7_BFBE_44B1_B335_9C4E88049701__INCLUDED_)
#define AFX_ETC_GDS_JP_BRG2012_H__08005CA7_BFBE_44B1_B335_9C4E88049701__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_JP_BRG2016.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_JP_BRG2012 dialog
#include "MyChildDialog.h"
class CSpfcJPBRG2012Util;

class CETC_GDS_JP_BRG2012 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_JP_BRG2012(T_SPFC_D* pParamData, int nCode, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	void UpdateDampingWnd(CWnd* pDampingWnd);
	BOOL ISVALID();
	void SetDefaultValue();  
	BOOL Dlg2Data();
	virtual void Data2Dlg();
	void SetPushOverData(BOOL bInit=TRUE);

// Dialog Data
	//{{AFX_DATA(CETC_GDS_JP_BRG2012)
	enum { IDD = IDD_ETC_GDS_JP_BRG2012 };
	CEditUnit	m_wndCz;
	CEditUnit	m_wndDamping;
	int		m_nMethod;
	int		m_nSoil;
	int		m_nZone;
	double	m_dMaxPeriod;
	int		m_nCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_JP_BRG2012)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:  
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcJPBRG2012Util> m_pSpfcUtil;

	void MakeSpectrumData2016(BOOL bOnlyCalc=FALSE);
	void MakeSpectrumData2017(BOOL bOnlyCalc=FALSE);

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_JP_BRG2012)
	afx_msg void OnEtcGdsZoneRdo();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_JP_BRG2012_H__08005CA7_BFBE_44B1_B335_9C4E88049701__INCLUDED_)
