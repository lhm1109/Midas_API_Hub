#if !defined(__POSTTENSIONTENDONPROFILEDLG_H__)
#define __POSTTENSIONTENDONPROFILEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PostTensionTendonProfileDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\wg_base\MChildFormView.h"
#include "..\wg_base\MenuBarChildDlg.h"

class CPostTensionTendonProfileDlg : public MChildFormView
{
protected:
    CPostTensionTendonProfileDlg();
    DECLARE_DYNCREATE(CPostTensionTendonProfileDlg)

public:
    enum { IDD = IDD_TM_TENDON_PROFILE };

    mit::frx::MButton    m_wndTableBtn;
    CPlaceHolder         m_wndPlaceHolder;
    mit::frx::MComboBox  m_ComboMenu;

public:
    CStringArray m_ComboMenuStr;
    CArray<CMenuBarChildDlg*, CMenuBarChildDlg*> m_ChildDlgs;
    CArray<int, int>         m_DlgIDs;
    CArray<UINT, UINT>       m_TableIDs;    // Command IDs for table button
    int                      m_nCurChildDlg;
    CRect                    m_RectPlaceHolder;
    int                      m_nOrgSlabPlaceHolderHeight;

public:
    int  _GetHeightFromFont();
    BOOL ShowChildDlgByID(int nID);
    void ShowCurChildDlg(int nDlgIndex);
    void DeleteChildDlgs();
    void CreateChildDlgs();
    void InitContent();
    void InitComboMenu();
    void AdjustComboListBox();
    void SetHolderSize(int nHeight);
    void ShowExecuteButton(UINT nShowHide);
    virtual void OnTabSelect(WPARAM wParam, LPARAM lParam);
    virtual void OnTabUnSelect(WPARAM wParam, LPARAM lParam);
    virtual CWnd* GetDlgByID(int Mode);
    void OnTmExecAndClose(BOOL bExecute);
    int  GetOrgSlabPlaceHolderSize();

public:
    virtual void OnInitialUpdate();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual ~CPostTensionTendonProfileDlg();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    afx_msg void OnTmClose();
    afx_msg void OnComboMenuSelect();
    afx_msg void OnTmExecute();
    afx_msg void OnTmTableBtn();

    DECLARE_MESSAGE_MAP()
};

#endif // !defined(__POSTTENSIONTENDONPROFILEDLG_H__)
