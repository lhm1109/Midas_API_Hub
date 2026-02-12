#if !defined(__ETC_GDS_DPWH_LRFD_2013_H__)
#define __ETC_GDS_DPWH_LRFD_2013_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_DPWH_LRFD_2013.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_DPWH_LRFD_2013 dialog
#include "MyChildDialog.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcPH2013Util;
using namespace mit::frx;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CETC_GDS_DPWH_LRFD_2013 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_DPWH_LRFD_2013(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor

	void End();
	void InitUnit();
	void InitCombo();
	void SetValue();
	BOOL ISVALID();
	BOOL Dlg2Data();
	void Data2Dlg();
	void SetPushOverData(BOOL bInit=TRUE);
	void Dlg2UtilData() override;

	
// Dialog Data
	//{{AFX_DATA(CETC_GDS_DPWH_LRFD_2013)
	enum { IDD = IDD_ETC_GDS_DPWH_LRFD_2013 };
	
	int m_nGroundType;
	CEditUnit m_edtPGA;
	CEditUnit m_edtSS;
	CEditUnit m_edtS1;
	MComboBox m_cmbRMF;
	CEditUnit m_edtMaxPeriod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_DPWH_LRFD_2013)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcPH2013Util> m_pSpfcUtil;

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_DPWH_LRFD_2013)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ETC_GDS_DPWH_LRFD_2013_H__)
