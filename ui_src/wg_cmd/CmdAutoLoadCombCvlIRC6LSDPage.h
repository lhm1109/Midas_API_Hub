#if !defined(__CMDAUTOLOADCOMBCVLIRC6LSDPAGE_H__)
#define __CMDAUTOLOADCOMBCVLIRC6LSDPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombCvlIRC6LSDPage.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlIRC6LSDPage dialog

class CCmdAutoLoadCombCvlDlg;
class CCmdAutoLoadCombCvlIRC6LSDPage : public CChildDialog
{
    // Construction
public:
    CCmdAutoLoadCombCvlIRC6LSDPage(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CCmdAutoLoadCombCvlIRC6LSDPage)
    enum { IDD = IDD_CMD_LCOMBDEFAULT_IRC6LSD_PAGE };

    BOOL           m_bEURO0BaseChk;
    BOOL           m_bEURO0DAddChk;
    BOOL           m_bEURO0DUniqChk;
    CEditUnit      m_dEURO0DMaxEdt;
    CEditUnit      m_dEURO0DwMaxEdt;
    CEditUnit      m_dEURO0PsMaxEdt;
    CEditUnit      m_dEURO0HsMaxEdt;
    CEditUnit      m_dEURO0DMinEdt;
    CEditUnit      m_dEURO0DwMinEdt;
    CEditUnit      m_dEURO0PsMinEdt;
    CEditUnit      m_dEURO0HsMinEdt;

    CFoldGroupBox_Cross	m_chkLoadFactor;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCmdAutoLoadCombCvlIRC6LSDPage)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
public:
    void OnCmdIRC6LSDEnableDisable();
	void OnCmdIRC6TrafficEnableDisable();

    int m_nConstLoad;

protected:
    CDBDoc* m_pDoc;
    CCmdAutoLoadCombCvlDlg* m_pParent;

    // Generated message map functions
    //{{AFX_MSG(CCmdAutoLoadCombCvlIRC6LSDPage)
    virtual BOOL OnInitDialog();

    //IRC6LSD
    afx_msg void OnCmdIRC6_LSDLVarbAllChk();
    afx_msg void OnCmdIRC6_LSDLVarbTraffChk();
    afx_msg void OnCmdIRC6_LSDLVarbWindChk();
    afx_msg void OnCmdIRC6_LSDLVarbThermChk();
    afx_msg void OnCmdIRC6_LSDLVarbSnowChk();
    afx_msg void OnCmdIRC6_LSDLVarbConstChk();
    afx_msg void OnCmdIRC6_LSDLVarbTraffRoadBtn();
    afx_msg void OnCmdIRC6_LSDLVSpclVehChkBox();
    afx_msg void OnCmdIRC6_LSDSpclVehBtn();
    afx_msg void OnCmdIRC6_IRC1122020();

    afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
public:

    BOOL  m_nIRC6LVarTraffType;
    BOOL  m_bSpclVeh;

    BOOL  m_bIRC6_LSDLVarAll;
    BOOL  m_bIRC6_LSDLVarTraff;
    BOOL  m_bIRC6_LSDLVarWind;
    BOOL  m_bIRC6_LSDLVarTherm;
    BOOL  m_bIRC6_LSDLVarSnow;
    BOOL  m_bIRC6_LSDLVarConst;
    BOOL  m_bIRC6_IRC1122020;

    CArray<T_MVLD_K, T_MVLD_K> m_aMvldChar;
    CArray<T_MVLD_K, T_MVLD_K> m_aMvldFreq;

    CArray<T_MVLDid_K, T_MVLDid_K> m_aMvldSPV;
    CArray<T_MVLDid_K, T_MVLDid_K> m_aMvldSPVSel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADCOMBCVLSP35133302011PAGE_H__)
