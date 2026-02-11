#if !defined(__CMDAUTOLOADCOMBCVLBD21PAGE_H__)
#define __CMDAUTOLOADCOMBCVLBD21PAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlBD21Page dialog

class CCmdAutoLoadCombCvlDlg;
class CCmdAutoLoadCombCvlBD21Page : public CChildDialog
{
    // Construction
public:
    CCmdAutoLoadCombCvlBD21Page(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    enum { IDD = IDD_CMD_LCOMBDEFAULT_BD_21_PAGE };

    MComboBox m_cmbBridgeType;
    int       m_nBD21D;
    CEditUnit m_edtBD21D_Max;
    CEditUnit m_edtBD21D_Min;
    int       m_nBD21DW;
    CEditUnit m_edtBD21DW_Max;
    CEditUnit m_edtBD21DW_Min;
    int       m_nBD21DC;
    CEditUnit m_edtBD21DC_Max;
    CEditUnit m_edtBD21DC_Min;
    int		  m_nBD21EV;
    CEditUnit m_edtBD21EV_Max;
    CEditUnit m_edtBD21EV_Min;
    int       m_nBD21EH;
    CEditUnit m_edtBD21EH_Max;
    CEditUnit m_edtBD21EH_Min;

    MComboBox m_MoveLcase;
    MComboBox m_MoveType;
    CListCtrl m_MovingList;

    CEditUnit m_edtBD21Gamma_ULS;
    CEditUnit m_edtBD21Gamma_SLS;

    CFoldGroupBox_Cross	m_chkLoadFactor;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCmdAutoLoadCombCvlAS51002017Page)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //	//}}AFX_VIRTUAL
    //
    //	// Implementation

public:
    void InitCombo();
    void SetDefault();

    void InitMVCombo();

    void InitMVListBox();
    BOOL CheckMovingCaseName(CString str,int& Index);
    void GetBD21LoadFactor();

    void Show_MoveComboBox(BOOL bCheck);

    void Initial_MVListBox();
    void OnCmdBD21Brdge();
    void OnCmdBD21MV();

    void OnCmdBD21EnableDisable();

    int m_nConstLoad;

    //
protected:
    CDBDoc* m_pDoc;

    CCmdAutoLoadCombCvlDlg* m_pParent;
    //
    //	// Generated message map functions
    virtual BOOL OnInitDialog();

    afx_msg void OnCmdBD21MVAddBtn();
    afx_msg void OnCmdBD21MVDelBtn();
    afx_msg void OnCmdBD21MVFactorBtn();

    afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
    //	//}}AFX_MSG
    //
    DECLARE_MESSAGE_MAP()

public:
    int m_nBD21BridgeType;

    int m_nBD21DNum;
    int m_nBD21DWNum;
    int m_nBD21DCNum;
    int m_nBD21EVNum;
    int m_nBD21EHNum;

    double m_dBD21DMax;
    double m_dBD21DMin;
    double m_dBD21DWMax;
    double m_dBD21DWMin;
    double m_dBD21DCMax;
    double m_dBD21DCMin;
    double m_dBD21EHMax;
    double m_dBD21EHMin;
    double m_dBD21EVMax;
    double m_dBD21EVMin;

    double m_dBD21GF3ULS;
    double m_dBD21GF3SLS;
    
    int m_nBD21MoveCase;

    int m_nBD21MoveType;

    double m_dBD21MVFLULSC1;
    double m_dBD21MVFLSLSC1;
    double m_dBD21MVFLULSC23;
    double m_dBD21MVFLSLSC23;

    CArray<UINT, UINT> m_aCtrlBD21;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADCOMBCVLBD21PAGE_H__)
