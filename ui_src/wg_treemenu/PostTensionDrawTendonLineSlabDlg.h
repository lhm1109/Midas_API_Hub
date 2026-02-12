#if !defined(__POSTTENSIONDRAWTENDONLINESLAB__)
#define __POSTTENSIONDRAWTENDONLINESLAB__

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_base\NotifyCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"

class CPostTensionDrawTendonLineDlg;
class CPostTensionDrawTendonLineSlabDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
    CPostTensionDrawTendonLineSlabDlg(CWnd* pParent = NULL);
	virtual ~CPostTensionDrawTendonLineSlabDlg();
    virtual void Execute();

    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    virtual void UpdateBuffer();

    enum { IDD = IDD_TM_TENDON_LINE_SLAB };

    // Tendon Property
    CStatic m_stcTendonProp;
    mit::frx::MComboBox m_cmbTendonProp;
    mit::frx::MButton m_btnTendonProp;
    // Tendon Property
    CStatic m_stcTendonProf;
    mit::frx::MComboBox m_cmbTendonProf;
    mit::frx::MButton m_btnTendonProf;
    // Tendon Stressing
    CStatic m_stcTendonStrs;
    mit::frx::MComboBox m_cmbTendonStrs;
    mit::frx::MButton m_btnTendonStrs;
    // Layout Type
    int m_nLayoutType;
    // Direction
    int m_nDirection;
    // Arrangement
    int m_nArrangement;
    mit::frx::MEdit m_edtArrSpace;
    mit::frx::MEdit m_edtArrSpace2;
    // End Type
    int m_nEndType;
    // Method
    mit::frx::MComboBox m_cmbMethodLine1;
    CSelectEdit m_edtMethodLine1;
    mit::frx::MComboBox m_cmbMethodLine2;
    CSelectEdit m_edtMethodLine2;
    // Line Type
    int m_nLineShape;
    // Band Width, Tendon Line No
    mit::frx::MEdit m_edtBandWidth;
    mit::frx::MEdit m_edtTendonLineNo;
    mit::frx::MButton m_chkTendonLineNoAuto;
    // Draw
    int m_nSelectDraw;
    CSelectEdit m_edtDrawNodes;
    // Offset
    mit::frx::MEdit m_edtOffset;

    CArray<UINT, UINT> m_aCtrlBandedType;
    CArray<UINT, UINT> m_aCtrlEndType;
    CArray<UINT, UINT> m_aCtrlDraw;
    CArray<UINT, UINT> m_aCtrlLineShape;
    CArray<UINT, UINT> m_aCtrlArrangement;
    CArray<UINT, UINT> m_aCtrlMethod;
    CArray<UINT, UINT> m_aCtrlDlg;

    T_TDSL_D m_Data;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    void Dlg2Data();

    CDBDoc* m_pDoc;

    void UpdateControlsLayout();
    void ResizeParent(int nHeightDelta);

    virtual BOOL OnInitDialog();
    afx_msg void OnClickTendonPropBtn();
	afx_msg void OnClickTendonProfBtn();
	afx_msg void OnClickTendonStrsBtn();
    afx_msg void OnEndTypeRadioClicked();
    afx_msg void OnLayoutTypeRadioClicked();
    afx_msg void OnDirectionRadioClicked();
    afx_msg void OnArrangementRadioClicked();
    afx_msg void OnMethodLine1ComboChanged();
    afx_msg void OnMethodLine2ComboChanged();
    afx_msg void OnLineTypeRadioClicked();
    afx_msg void OnDrawRadioClicked();
    afx_msg void OnSetFocusMethodLine1Edit();
    afx_msg void OnKillFocusMethodLine1Edit();
    afx_msg void OnSetFocusMethodLine2Edit();
    afx_msg void OnKillFocusMethodLine2Edit();
    DECLARE_MESSAGE_MAP()

    void InitSelectionFilter();
    void InitCtrl();
    void SetNodeSelectCtrlCondition();
    void GetNodeInfoBySupportLine(CArray<UINT, UINT>& aNodeK);
    void GetNodeInfoBySupportingNodes(CString strNodes, CArray<UINT, UINT>& aNodeK);
    void GetSelectedDesignStripKeyList(CArray<T_DSTR_K, T_DSTR_K>& aKey);
	void GetSelectedSupportLineKeyList(CArray<T_SPTL_K, T_SPTL_K>& aKey);
    void SetNodeInfoByDesignStrip(T_NODE_K NodeIK, T_NODE_K NodeJK);
    void Execute4SupLine();
    void Execute4SupLine_Distributed();
    void Execute4SupNode();
    void Execute4SupNode_Distributed();
    void UpdateTendonPropertyList();
    void UpdateTendonProfileList();
    void UpdateTendonStressingList();

private:
    T_NODE_K FindNearestNodeAtSameHeight(const T_NODE_D& targetNode, double zTolerance = 1e-6, double distanceThreshold = 1000.0);
    CPostTensionDrawTendonLineDlg* m_pParentDlg;
    int m_nCurDistY = 0;
    int m_nPrevDistY = 0;
    int m_nCurLineShapeDistY = 0;
    int m_nPrevLineShapeDistY = 0;
    int m_nOriginalPlaceHolderHeight;
    int m_nOriginalDialogHeight;
    int m_nOriginalLineShapeBottom;
};

#endif
