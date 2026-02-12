#if !defined(AFX_ETC_GDS_EURO1996_ELA_H__71E63C6D_D34F_4E44_A61E_F81C72E8CD4F__INCLUDED_)
#define AFX_ETC_GDS_EURO1996_ELA_H__71E63C6D_D34F_4E44_A61E_F81C72E8CD4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_EURO1996_ELA.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_EURO1996_ELA dialog
#include "MyChildDialog.h"

#include "HeaderPre.h"
class CSpfcEURO1996ELAUtil;

class __MY_EXT_CLASS__ CETC_GDS_EURO1996_ELA : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_EURO1996_ELA(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose();
	void UpdateDampingWnd(CWnd* pDampingWnd);
	BOOL ISVALID();
	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit=TRUE);
	void Dlg2UtilData() override;

// Dialog Data
	//{{AFX_DATA(CETC_GDS_EURO1996_ELA)
	enum { IDD = IDD_ETC_GDS_EURO1996_ELA };
	int		m_wndSc;
	double	m_dAg;
	double  m_dDamping;
	double	m_dMaxPeriod;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_EURO1996_ELA)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcEURO1996ELAUtil> m_pSpfcUtil;

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_EURO1996_ELA)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_EURO1996_ELA_H__71E63C6D_D34F_4E44_A61E_F81C72E8CD4F__INCLUDED_)
