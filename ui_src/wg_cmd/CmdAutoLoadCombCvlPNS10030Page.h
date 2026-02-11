#if !defined(__CMDAUTOLOADCOMBCVLPNS10030PAGE_H__)
#define __CMDAUTOLOADCOMBCVLPNS10030PAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombCvlPNS10030Page.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlPNS10030Page dialog

class CCmdAutoLoadCombCvlDlg;
class CCmdAutoLoadCombCvlPNS10030Page : public CChildDialog
{
    // Construction
public:
    CCmdAutoLoadCombCvlPNS10030Page(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CCmdAutoLoadCombCvlPNS10030Page)
    enum { IDD = IDD_CMD_LCOMBDEFAULT_PNS_10030_PAGE };

    BOOL           m_bPNS10030BaseChk;
    BOOL           m_bPNS10030DAddChk;
    BOOL           m_bPNS10030DUniqChk;
    CEditUnit      m_dPNS10030DMaxEdt;
    CEditUnit      m_dPNS10030DwMaxEdt;
    CEditUnit      m_dPNS10030PsMaxEdt;
    CEditUnit      m_dPNS10030HsMaxEdt;
    CEditUnit      m_dPNS10030DMinEdt;
    CEditUnit      m_dPNS10030DwMinEdt;
    CEditUnit      m_dPNS10030PsMinEdt;
    CEditUnit      m_dPNS10030HsMinEdt;

    CFoldGroupBox_Cross	m_chkLoadFactor;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCmdAutoLoadCombCvlPNS10030Page)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
public:
    void InitCombo();
    void SetDefault();
    void GetPNS10030LoadFactor();
    void OnCmdPNS10030EnableDisable();

    int m_nConstLoad;

protected:
    CDBDoc* m_pDoc;
    CCmdAutoLoadCombCvlDlg* m_pParent;

    // Generated message map functions
    //{{AFX_MSG(CCmdAutoLoadCombCvlPNS10030Page)
    virtual BOOL OnInitDialog();

    afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
public:
    //PN-85/S-10030 (2016-2-12 ADD)
    int            m_nPNS10030DNum;
    int            m_nPNS10030DcNum;
    int            m_nPNS10030DwNum;
    int            m_nPNS10030PsNum;
    int            m_nPNS10030WpNum;
    int            m_nPNS10030DMaxRdo;
    int            m_nPNS10030DwMaxRdo;
    int            m_nPNS10030PsMaxRdo;
    int            m_nPNS10030HsMaxRdo;

    double           m_dPNS10030DMax;
    double           m_dPNS10030DMin;
    double           m_dPNS10030DcMax;
    double           m_dPNS10030DcMin;
    double           m_dPNS10030DwMax;
    double           m_dPNS10030DwMin;
    double           m_dPNS10030PsMax;
    double           m_dPNS10030PsMin;
    double           m_dPNS10030WpMax;
    double           m_dPNS10030WpMin;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADCOMBCVLSP35133302011PAGE_H__)
