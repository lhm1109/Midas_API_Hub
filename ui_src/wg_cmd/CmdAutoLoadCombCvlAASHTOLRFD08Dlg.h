#if !defined(__CMDAUTOLOADCOMBCVLAASHTOLRFD08Dlg_H__)
#define __CMDAUTOLOADCOMBCVLAASHTOLRFD08Dlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombCvlAASHTOLRFD08Dlg.h : header file
//
#include "..\wg_base\FoldGroupBox.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlAASHTOLRFD08Dlg dialog

class CCmdAutoLoadCombCvlDlg;
class CCmdAutoLoadCombCvlAASHTOLRFD08Dlg : public CChildDialog
{
    // Construction
public:
    CCmdAutoLoadCombCvlAASHTOLRFD08Dlg(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CCmdAutoLoadCombCvlAASHTOLRFD08Dlg)
    enum { IDD = IDD_CMD_LCOMBDEFAULT_AASHTO_LRFD08_DLG };
    // AASHTO LRFD 08 
    CEditUnit m_dLrfd08ModifierEdt;
    int       m_nLrfd08DCRdo;
    int       m_nLrfd08DDTypeRdo;
    int       m_nLrfd08DD1Rdo;
    int       m_nLrfd08DD2Rdo;
    int       m_nLrfd08DD3Rdo;
    int       m_nLrfd08DWRdo;
    int       m_nLrfd08EHTypeRdo;
    int       m_nLrfd08EH1Rdo;
    int       m_nLrfd08EH2Rdo;
    int       m_nLrfd08EVTypeRdo;
    int       m_nLrfd08EV2Rdo;
    int       m_nLrfd08EV3Rdo;
    int       m_nLrfd08EV4Rdo;
    int       m_nLrfd08EV5Rdo;
    int       m_nLrfd08EV6Rdo;
    int       m_nLrfd08ESRdo;
    MComboBox m_nPSCRSHTypeCmb;
    CEditUnit m_dLrfd08PSEdt;
    CEditUnit m_dLrfd08CRSHEdt;
    CEditUnit m_dLrfd08SettlementEdt;
    MButton	  m_bLrfd08CulvertChk;
    int   	  m_nLrfd08DeformationRdo;

    CFoldGroupBox_Cross	m_chkLoadFactor;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCmdAutoLoadCombCvlAASHTOLRFD08Dlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
public:
    void InitCombo();
    void GetAASHTOLRFD08LoadFactor();
    void OnCmdAASHTOLRFD08EnableDisable();

    int m_nConstLoad;

protected:
    CDBDoc* m_pDoc;
    CString m_strCodeName;

    CCmdAutoLoadCombCvlDlg* m_pParent;
    // Generated message map functions
    //{{AFX_MSG(CCmdAutoLoadCombCvlAASHTOLRFD08Dlg)
    virtual BOOL OnInitDialog();
    // AASHTO LRFD 08
    afx_msg void OnCmdAASHTOLRFD08DDTypeRdo();
    afx_msg void OnCmdAASHTOLRFD08EHTypeRdo();
    afx_msg void OnCmdAASHTOLRFD08EVTypeRdo();
    afx_msg void OnSelchangeCmdAASHTOLRFD08PSCRSHCmb();
    afx_msg void OnCmdAASHTOLRFD08PSCRSHBtn();

    afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
public:

    int m_nAshLr08DcNum;
    int m_nAshLr08DdNum;
    int m_nAshLr08DwNum;
    int m_nAshLr08EhNum;
    int m_nAshLr08ElNum;
    int m_nAshLr08EvNum;
    int m_nAshLr08EsNum;
    int m_nAshLr08PsNum;
    int m_nAshLr08CrNum;
    int m_nAshLr08ShNum;

    double m_dAshLr08Dc1;
    double m_dAshLr08Dc2;
    double m_dAshLr08Dd1;
    double m_dAshLr08Dd2;
    double m_dAshLr08Dw1;
    double m_dAshLr08Dw2;
    double m_dAshLr08Eh1;
    double m_dAshLr08Eh2;
    double m_dAshLr08El1;
    double m_dAshLr08El2;
    double m_dAshLr08Ev1;
    double m_dAshLr08Ev2;
    double m_dAshLr08Es1;
    double m_dAshLr08Es2;
    double m_dAshLr08Ps1;
    double m_dAshLr08Ps2;
    double m_dAshLr08Cr1;
    double m_dAshLr08Cr2;
    double m_dAshLr08Sh1;
    double m_dAshLr08Sh2;


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADCOMBCVLAASHTOLRFD08Dlg_H__)
