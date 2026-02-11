#if !defined(AFX_ETC_GDS_RUS_SP2016_H__INCLUDED_)
#define AFX_ETC_GDS_RUS_SP2016_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_RUS_SP268.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_RUS_SP2016 dialog
#include "MyChildDialog.h"

class CSpfcSP2016Util;

class CETC_GDS_RUS_SP2016 : public CMyChildDialog
{
// Construction
public:
    CETC_GDS_RUS_SP2016(T_SPFC_D* pParamData, BOOL bModify, CWnd* pParent = NULL);   // standard constructor
    void End();
    void EndNotClose(){};
    BOOL ISVALID();

    BOOL Dlg2Data();
    virtual void Data2Dlg();
    void SetPushOverData(BOOL bInit=TRUE);
    void Dlg2UtilData() override;

    double m_dMaxPeriod;
	BOOL m_bInit;
// Dialog Data
    //{{AFX_DATA(CETC_GDS_RUS_SP2016)
    enum { IDD = IDD_ETC_GDS_RUS_SP2016 };
    CComboBox	m_cmbRegion;
    CComboBox	m_cmbSoil;

	CEditUnit	m_editK1;
	CEditUnit	m_editK2;
	CEditUnit	m_editK3;
	CEditUnit	m_editK4;
	CEditUnit	m_editKPsi;

    CEditUnit m_editMaxPeriod;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CETC_GDS_RUS_SP2016)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
private :

protected:
    T_SPFC_D* m_pParamData;
    std::shared_ptr<CSpfcSP2016Util> m_pSpfcUtil;

    void InitZoneCombo();
    void InitSiteCombo();

	void GetFinalValue();

    // Generated message map functions
    //{{AFX_MSG(CETC_GDS_RUS_SP2016)
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeCmdZoneCmb();
    afx_msg void OnSelchangeCmdSiteCmb();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_RUS_SP2016_H__INCLUDED_)
