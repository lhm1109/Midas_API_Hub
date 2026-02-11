#if !defined(AFX_ETC_GDS_IBC2000_H__81FB3990_77DF_47AE_B7F6_BBD0045075E1__INCLUDED_)
#define AFX_ETC_GDS_IBC2000_H__81FB3990_77DF_47AE_B7F6_BBD0045075E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CSpfcIBC2000Util;
// ETC_GDS_IBC2000.h : header file
//
class CDBDoc;
class CAttrCtrl;
class CUnitCtrl;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IBC2000 dialog
#include "MyChildDialog.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CETC_GDS_IBC2000 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_IBC2000(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	BOOL ISVALID();
	void SetValue();
	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit=TRUE);

	// Dialog Data
	//{{AFX_DATA(CETC_GDS_IBC2000)
	enum { IDD = IDD_ETC_GDS_IBC2000 };
	MComboBox	m_wndI;
	MComboBox	m_wndS1;
	MComboBox	m_wndSs;
	int		m_wndSDC;
	int		m_wndSC;
	CString	m_sSs;
	CString	m_sS1;
	CString	m_sI;
	double	m_dR;
	double	m_dMaxPeriod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_IBC2000)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private :
	CDBDoc* m_pDoc;
	CAttrCtrl* m_pAttrCtrl;
	CUnitCtrl* m_pUnitCtrl;

protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcIBC2000Util> m_pSpfcUtil;

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_IBC2000)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_IBC2000_H__81FB3990_77DF_47AE_B7F6_BBD0045075E1__INCLUDED_)
