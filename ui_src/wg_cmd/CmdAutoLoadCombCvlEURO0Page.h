#if !defined(__CMDAUTOLOADCOMBCVLEURO0PAGE_H__)
#define __CMDAUTOLOADCOMBCVLEURO0PAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombCvlEURO0Page.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlEURO0Page dialog

class CCmdAutoLoadCombCvlDlg;
class CCmdAutoLoadCombCvlEURO0Page : public CChildDialog
{
    // Construction
public:
    CCmdAutoLoadCombCvlEURO0Page(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CCmdAutoLoadCombCvlEURO0Page)
    enum { IDD = IDD_CMD_LCOMBDEFAULT_EURO0_PAGE };

    BOOL      m_bEURO0BaseChk;
    BOOL      m_bEURO0DAddChk;
    BOOL      m_bEURO0DUniqChk;
    CEditUnit m_dEURO0DMaxEdt;
    CEditUnit m_dEURO0DwMaxEdt;
    CEditUnit m_dEURO0PsMaxEdt;
    CEditUnit m_dEURO0HsMaxEdt;
    CEditUnit m_dEURO0DMinEdt;
    CEditUnit m_dEURO0DwMinEdt;
    CEditUnit m_dEURO0PsMinEdt;
    CEditUnit m_dEURO0HsMinEdt;
    int       m_nEuroPerm;
    int       m_nEuroPres;
    int       m_nEuroSettle;

    CFoldGroupBox_Cross	m_chkLoadFactor;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCmdAutoLoadCombCvlEURO0Page)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
public:
    void OnCmdEURO0EnableDisable();
    void OnCmdEuroTrafficEnableDisable();
    void GetEuroLoadFactor();

    int m_nConstLoad;

protected:
    CDBDoc* m_pDoc;
    CCmdAutoLoadCombCvlDlg* m_pParent;

    // Generated message map functions
    //{{AFX_MSG(CCmdAutoLoadCombCvlEURO0Page)
    virtual BOOL OnInitDialog();

    afx_msg void OnCmdEC4LVarbAllChk();
    afx_msg void OnCmdEC4LVarbTraffChk();
    afx_msg void OnCmdEC4LVarbWindChk();
    afx_msg void OnCmdEC4LVarbThermChk();
    afx_msg void OnCmdEC4LVarbSnowChk();
    afx_msg void OnCmdEC4LVarbConstChk();
    afx_msg void OnCmdEC4LVarbTraffRoadBtn();

    afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
public:

    int   m_nEC4Permanent;
    int   m_nEC4Prestress;
    int   m_nEC4Settlement;
    int   m_nEC4LVarTraffType;

    BOOL  m_bEC4LVarAll;
    BOOL  m_bEC4LVarTraff;
    BOOL  m_bEC4LVarWind;
    BOOL  m_bEC4LVarTherm;
    BOOL  m_bEC4LVarSnow;
    BOOL  m_bEC4LVarConst;

    CArray<T_MVLD_K, T_MVLD_K> m_aMvldChar;
    CArray<T_MVLD_K, T_MVLD_K> m_aMvldFreq;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADCOMBCVLSP35133302011PAGE_H__)
