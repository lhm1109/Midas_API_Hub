#if !defined(__CmdAutoLoadCombCvlIRSPage_H__)
#define __CmdAutoLoadCombCvlIRSPage_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombCvlIRSPage.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlIRSPage dialog

class CCmdAutoLoadCombCvlDlg;
class CCmdAutoLoadCombCvlIRSPage : public CChildDialog
{
    // Construction
public:
    CCmdAutoLoadCombCvlIRSPage(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CCmdAutoLoadCombCvlIRSPage)
    enum { IDD = IDD_CMD_LCOMBDEFAULT_IRS_MAIN_PAGE };

    //IRS
    BOOL           m_bIRSDrl;
    BOOL           m_bIRSULSOPT;
    BOOL           m_bChkCrp;
    CEditUnit      m_dIRSULT;
    CEditUnit      m_dIRSSRV;

    CFoldGroupBox_Cross	m_chkLoadFactor;

    CArray<T_STLD_K, T_STLD_K> m_aIRSUlt;
    CArray<T_STLD_K, T_STLD_K> m_aIRSServ;

    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCmdAutoLoadCombCvlIRSPage)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
public:
    void OnCmdIRSEnableDisable();
    void OnCmdIRSDrlEnableDisable();

    int m_nConstLoad;

protected:
    CDBDoc* m_pDoc;
    CCmdAutoLoadCombCvlDlg* m_pParent;
    CArray<T_MVLD_K, T_MVLD_K> m_aMvldChar;
    CArray<T_MVLD_K, T_MVLD_K> m_aMvldFreq;

    void OnCmdIRSSetDefault();
    void OnBtnIRSDrlClicked();

    // Generated message map functions
    //{{AFX_MSG(CCmdAutoLoadCombCvlIRSPage)
    virtual BOOL OnInitDialog();

    afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADCOMBCVLSP35133302011PAGE_H__)
