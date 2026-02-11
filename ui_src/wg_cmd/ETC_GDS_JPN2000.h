#if !defined(AFX_ETC_GDS_JPN2000_H__65C0E356_96C6_4995_9C4F_2F377FFEF781__INCLUDED_)
#define AFX_ETC_GDS_JPN2000_H__65C0E356_96C6_4995_9C4F_2F377FFEF781__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_JPN2000.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_JPN2000 dialog
#include "MyChildDialog.h"

class CSpfcJPN2000Util;

class CETC_GDS_JPN2000 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_JPN2000(T_SPFC_D* pParamData,CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	BOOL ISVALID();
	void SetValue();
	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit=TRUE);
	void Dlg2UtilData() override;

// Dialog Data
	//{{AFX_DATA(CETC_GDS_JPN2000)
	enum { IDD = IDD_ETC_GDS_JPN2000 };
	int		m_wndZ;
	int		m_wndTc;
	double	m_dC0;
	double	m_dMaxPeriod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_JPN2000)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcJPN2000Util> m_pSpfcUtil;

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_JPN2000)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_JPN2000_H__65C0E356_96C6_4995_9C4F_2F377FFEF781__INCLUDED_)
