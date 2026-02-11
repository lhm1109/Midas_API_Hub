#if !defined(__DESIGNSTRIPMODIFYSECTIONDLG_H__)
#define __DESIGNSTRIPMODIFYSECTIONDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"

class CDesignStripModifySectionDlg : public CMenuBarChildDlg
{
public:
    CDesignStripModifySectionDlg(CWnd* pParent = NULL);
    virtual ~CDesignStripModifySectionDlg();

    virtual void Execute();

    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    virtual void UpdateBuffer();

    // Dialog Data
    enum { IDD = IDD_TM_DESIGN_STRIP_MODIFY_SECTION };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    void InitCtrl();
    void InitListCtrlHeaders();

    // List Control Update Functions
    void UpdateDesignStripList();
    void UpdateSpanList();
    void UpdateDesignSectionList();

    // Data Transfer Functions
    void Dlg2Data();
    void Data2Dlg();

    // Selection Filter Functions
    void InitSelectionFilter();
    void ChangeSelectionFilter(EN_SELECT_ELEM_FILTER enFilter);

    // Highlight Functions
    void NotifyDesignSectionHighlight();
    void ClearDesignSectionHighlight();

protected:
    CDBDoc* m_pDoc;

    // List Controls
    CListCtrl m_wndDesignStripList;
    CListCtrl m_wndSpanList;
    CListCtrl m_wndDesignSectionList;

    // Current Selection Keys
    T_DSTR_K m_nSelectedDstrK;
    int m_nSelectedSpanIdx;
    int m_nSelectedSectionIdx;

    // In-place Edit
    CEdit* m_pEditCtrl;
    int m_nEditItem;
    int m_nEditSubItem;

    // Modified Data
    struct MODIFIED_DSPS_DATA {
        T_DSPS_K DspsK;
        T_DSPS_D DspsD;
        BOOL bModified;
    };
    CArray<MODIFIED_DSPS_DATA, MODIFIED_DSPS_DATA&> m_aModifiedData;

private:
    void OnDesignStripSelectionChanged();
    void OnSpanSelectionChanged();
    void OnDesignSectionSelectionChanged();

    // In-place Edit Functions
    void StartEdit(int nItem, int nSubItem);
    void EndEdit(BOOL bSave = TRUE);
    void SaveModifiedData(int nItem, int nSubItem, const CString& strValue);
    void AddOrUpdateModifiedData(T_DSPS_K DspsK, const T_DSPS_D& DspsD);
    BOOL CheckUnsavedChanges();

private:
    afx_msg void OnClickDesignStripList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickSpanList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickDesignSectionList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDblClickDesignSectionList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnKillFocusEdit();
    DECLARE_MESSAGE_MAP()
};

#endif // !defined(__DESIGNSTRIPMODIFYSECTIONDLG_H__)
