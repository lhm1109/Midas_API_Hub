#if !defined(__POSTTENSIONTENDONPROFILEMATCHDLG_H__)
#define __POSTTENSIONTENDONPROFILEMATCHDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PostTensionTendonProfileMatchDlg.h : header file
//

#include "..\wg_base\MenuBarChildDlg.h"
#include "..\wg_base\wg_base_ControlEx.h"

#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"

class _TendonProfileMatch
{
public:
    CArray<UINT, UINT> m_SourceKeyList; // 기준 텐던
    CArray<UINT, UINT> m_TargetKeyList; // 적용 대상 텐던
};

class CPostTensionTendonProfileMatchDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
    CPostTensionTendonProfileMatchDlg(CWnd* pParent = NULL);
    virtual ~CPostTensionTendonProfileMatchDlg();
    virtual void Execute();

	void ExecuteBeam();
	void ExecuteSlab();

    enum { IDD = IDD_TM_TENDON_MATCH_PROFILE };

    int m_nElemType;
    mit::frx::MComboBox	m_ComboTendon;
    mit::frx::MComboBox	m_ComboStressing;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    virtual void UpdateBuffer();
    virtual BOOL OnInitDialog();

    void SetInitComboBoxString();
    void SetInitCmbStringProfile();
    void SetInitCmbStringStressing();

    void InitSelectionFilter();
    void ChangeSelectionFilter(EN_SELECT_ELEM_FILTER enFilter);

    CDBDoc* m_pDoc;

private:
    void GetSelectedTendonLineKeyList(CArray<T_TDBM_K, T_TDBM_K>& aKey);
    void GetSelectedTendonSlabKeyList(CArray<T_TDSL_K, T_TDSL_K>& aKey);

	afx_msg void OnClickElemTypeRadio();
    afx_msg void OnClickTendonProfileBtn();
    afx_msg void OnClickTendonStressingBtn();
    DECLARE_MESSAGE_MAP()
};

#endif // !defined(__POSTTENSIONTENDONPROFILEMATCHDLG_H__)
