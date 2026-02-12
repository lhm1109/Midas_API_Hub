#if !defined(__POSTTENSIONTENDONELEMDELETEDLG_H__)
#define __POSTTENSIONTENDONELEMDELETEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PostTensionTendonElemDeleteDlg.h : header file
//

#include "..\wg_base\MenuBarChildDlg.h"
#include "..\wg_base\wg_base_ControlEx.h"

class CPostTensionTendonElemDeleteDlg : public CMenuBarChildDlg
{
public:
    CPostTensionTendonElemDeleteDlg(CWnd* pParent = NULL);
    virtual ~CPostTensionTendonElemDeleteDlg();
    virtual void Execute();

    enum { IDD = IDD_TM_TENDON_ELEM_DEL };
    int m_nElemType;

    CDBDoc* m_pDoc;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnClickElemTypeRadio();
    DECLARE_MESSAGE_MAP()

    void Execute4Beam();
    void Execute4Slab();
    void Execute4Wall();
    void GetSelectedTendonLineKeyList(UINT enTendonType, CArray<UINT, UINT>& aKey);

    void InitSelectionFilter();
    void ChangeSelectionFilter(EN_SELECT_ELEM_FILTER enFilter);
};

#endif // !defined(__POSTTENSIONTENDONELEMDELETEDLG_H__)
