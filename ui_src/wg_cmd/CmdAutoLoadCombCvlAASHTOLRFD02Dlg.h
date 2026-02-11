#if !defined(__CMDAUTOLOADCOMBCVLAASHTOLRFD02DLG_H__)
#define __CMDAUTOLOADCOMBCVLAASHTOLRFD02DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombCvlAASHTOLRFD02Dlg.h : header file
//
#include "..\wg_base\FoldGroupBox.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlAASHTOLRFD02Dlg dialog

class CCmdAutoLoadCombCvlDlg;
class CCmdAutoLoadCombCvlAASHTOLRFD02Dlg : public CChildDialog
{
    // Construction
public:
    CCmdAutoLoadCombCvlAASHTOLRFD02Dlg(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CCmdAutoLoadCombCvlAASHTOLRFD02Dlg)
    enum { IDD = IDD_CMD_LCOMBDEFAULT_AASHTO_LRFD02_DLG };
    CEditUnit	m_wndSettlement;
    CEditUnit	m_wndModifier;
    MButton	    m_wndCulvertChk;

    int		m_nDcRdo;
    int		m_nDdRdo;
    int		m_nDeformationRdo;
    int		m_nDwRdo;
    int		m_nEhTypeRdo;
    int		m_nEh1Rdo;
    int		m_nEh2Rdo;
    int		m_nEsRdo;
    int		m_nEvTypeRdo;
    int		m_nEv2Rdo;
    int		m_nEv3Rdo;
    int		m_nEv4Rdo;
    int		m_nEv5Rdo;
    int		m_nEv6Rdo;

    CFoldGroupBox_Cross	m_chkLoadFactor;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCmdAutoLoadCombCvlAASHTOLRFD02Dlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
public:
    void InitCombo();
    void GetAASHTOLRFD02LoadFactor();
    void OnCmdAASHTOLRFD02EnableDisable();

    int m_nConstLoad;

protected:
    CDBDoc* m_pDoc;
    CString m_strCodeName;

    CCmdAutoLoadCombCvlDlg* m_pParent;
    // Generated message map functions
    //{{AFX_MSG(CCmdAutoLoadCombCvlAASHTOLRFD02Dlg)
    virtual BOOL OnInitDialog();

    afx_msg void OnCmdAASHTOLRFD02EhRdo();
    afx_msg void OnCmdAASHTOLRFD02EvRdo();

    afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
public:

    int m_nAshLr98DcNum;
    int m_nAshLr98DdNum;
    int m_nAshLr98DwNum;
    int m_nAshLr98EhNum;
    int m_nAshLr98ElNum;
    int m_nAshLr98EvNum;
    int m_nAshLr98EsNum;
    double m_dAshLr98Dc1;
    double m_dAshLr98Dc2;
    double m_dAshLr98Dd1;
    double m_dAshLr98Dd2;
    double m_dAshLr98Dw1;
    double m_dAshLr98Dw2;
    double m_dAshLr98Eh1;
    double m_dAshLr98Eh2;
    double m_dAshLr98El1;
    double m_dAshLr98El2;
    double m_dAshLr98Ev1;
    double m_dAshLr98Ev2;
    double m_dAshLr98Es1;
    double m_dAshLr98Es2;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADCOMBCVLAASHTOLRFD02DLG_H__)
