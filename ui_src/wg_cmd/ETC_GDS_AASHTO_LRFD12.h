#if !defined(__ETC_GDS_AASHTO_LRFD12_H__)
#define __ETC_GDS_AASHTO_LRFD12_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_AASHTO_LRFD12.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_AASHTO_LRFD12 dialog
#include "MyChildDialog.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcAASHTOLRFD12Util;
using namespace mit::frx;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CETC_GDS_AASHTO_LRFD12 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_AASHTO_LRFD12(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor

	void End();
	void InitCombo();
	BOOL ISVALID();
	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit=TRUE);

// Dialog Data
	//{{AFX_DATA(CETC_GDS_AASHTO_LRFD12)
	enum { IDD = IDD_ETC_GDS_AASHTO_LRFD12 };
	
	int m_nSiteClasee;
	MComboBox m_cmbPGA;
	MComboBox m_cmbSS;
	MComboBox m_cmbS1;
	MComboBox m_cmbRMF;
	CEditUnit m_edtMaxPeriod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_AASHTO_LRFD12)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcAASHTOLRFD12Util> m_pSpfcUtil;

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_AASHTO_LRFD12)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ETC_GDS_AASHTO_LRFD12_H__)
