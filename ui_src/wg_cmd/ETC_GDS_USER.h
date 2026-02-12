#if !defined(AFX_ETC_GDS_USER_H__9F49EBF9_755C_405A_950A_F2C5B7CE62A9__INCLUDED_)
#define AFX_ETC_GDS_USER_H__9F49EBF9_755C_405A_950A_F2C5B7CE62A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_USER.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_USER dialog
#include "MyChildDialog.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CETC_GDS_USER : public CMyChildDialog
{
// Construction
public:
	void End();
	void EndNotClose();
	void MakeSpectrumData(BOOL bOnlyCalc=FALSE);
	void MakePOSpectrumData(double dDamping,double SRa,double SRv,CArray <double,double>& aPeriod,CArray <double,double>& aAccel);// for Pushover Curve
	CETC_GDS_USER(T_SPFC_K SpfcK=0, CWnd* pParent = NULL);   // standard constructor
	BOOL ISVALID();
	UINT GetSpfcK() { return m_SpfcK; }
	BOOL GetGravity_UserDefinedSpec(double& dGrav);
// Dialog Data
	//{{AFX_DATA(CETC_GDS_USER)
	enum { IDD = IDD_ETC_GDS_USER };
	MComboBox	m_DsgnSpec;
	//}}AFX_DATA
protected:
	T_SPFC_K m_SpfcK;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_USER)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<T_SPFC_K,T_SPFC_K> m_arSpfcKey;
	void ChangeComboWidth();

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_USER)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickBtnShowSpectrum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_USER_H__9F49EBF9_755C_405A_950A_F2C5B7CE62A9__INCLUDED_)
