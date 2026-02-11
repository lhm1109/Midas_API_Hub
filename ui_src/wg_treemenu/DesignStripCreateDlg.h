#if !defined(__DESIGNSTRIPCREATEDLG_H__)
#define __DESIGNSTRIPCREATEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_cmd\CMGXGridWnd.h"

class CDesignStripCreateDlg : public CMenuBarChildDlg
{
public:
    CDesignStripCreateDlg(CWnd* pParent = NULL);
    virtual ~CDesignStripCreateDlg();

    virtual void Execute();

    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    virtual void UpdateBuffer();

    // Dialog Data
    enum { IDD = IDD_TM_DESIGN_STRIP_CREATE };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

    void Dlg2Data();
    void Data2Dlg();

    //void GetSupportingNodes();
    void InitCtrl();
    void CreateDesignStrip();

    void InitSelectionFilter();
    void ChangeSelectionFilter(EN_SELECT_ELEM_FILTER enFilter);

    CDBDoc* m_pDoc;

    virtual BOOL OnInitDialog();

    // Member variables for dialog controls
    int m_nArrDgnPoint; // 0: Center-Center, 1: Edge-Edge
    int m_nDgnPointNo;  // Num. of Design Section per Span

    UINT m_SptlK;

    T_DSTR_D m_Data;

    // Helper functions for new Design Strip algorithm
    void GetSupportLinesByDirection(int nDirection, CArray<T_SPTL_K, T_SPTL_K>& rSptlKeyList);

    void CalculateAdjacentDistances(
        const T_NODE_D& NodeD,
        int nDirection,
        const CArray<T_SPTL_K, T_SPTL_K>& aSameDirSptlKeys,
        T_SPTL_K CurrentSptlK,
        double& dDistanceTL,
        double& dDistanceBR);

    BOOL CreateDesignStripForSingleSptl(
        T_SPTL_K SptlKey,
        const CArray<T_SPTL_K, T_SPTL_K>& aSameDirSptlKeys,
        T_DSTR_D& rDstrData);

private:
    void CheckDesignStripSelectionChanged();
    void LoadSelectedDesignStripInfo();
    void MakeDesignSection(UINT StartNodeK, UINT EndNodeK, T_DGNSTRIP_SPAN_INFO& Data);

    static const UINT_PTR TIMER_CHECK_SELECTION = 1001;

private:
    afx_msg void OnDblclkTmSpanWidthList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSetfocusNodeList();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()
};

#endif // !defined(__DESIGNSTRIPCREATEDLG_H__)