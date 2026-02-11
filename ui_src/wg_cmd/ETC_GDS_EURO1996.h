#if !defined(AFX_ETC_GDS_EURO1996_H__9F49EBF9_755C_405A_950A_F2C5B7CE62A9__INCLUDED_)
#define AFX_ETC_GDS_EURO1996_H__9F49EBF9_755C_405A_950A_F2C5B7CE62A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_EURO1996.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_EURO1996 dialog
#include "MyChildDialog.h"

#include "HeaderPre.h"
class CSpfcEURO1996Util;

class __MY_EXT_CLASS__ CETC_GDS_EURO1996 : public CMyChildDialog
{
// Construction
public:
	void End();
	void EndNotClose();

	void SetValue();

	CETC_GDS_EURO1996(T_SPFC_D* pParamData,CWnd* pParent = NULL);   // standard constructor
	BOOL ISVALID();
	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit=TRUE);
	void Dlg2UtilData() override;

// Dialog Data
	//{{AFX_DATA(CETC_GDS_EURO1996)
	enum { IDD = IDD_ETC_GDS_EURO1996 };
	CComboBox	m_wndQ0;
	int		m_wndSc;
	int		m_wndKd;
	int		m_wndKr;
	double	m_dKw;
	double	m_dAlpha;
	double	m_dMaxPeriod;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_EURO1996)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcEURO1996Util> m_pSpfcUtil;

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_EURO1996)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_EURO1996_H__9F49EBF9_755C_405A_950A_F2C5B7CE62A9__INCLUDED_)
