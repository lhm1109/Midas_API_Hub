#if !defined(__SUPPORTLINECREATEDLG_H__)
#define __SUPPORTLINECREATEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\mit_frx\MButton.h"
#include "..\wg_base\MouseEdit.h"
#include "..\wg_db\wg_db_DBDoc.h"

class CSupportLineCreateDlg : public CMenuBarChildDlg
{
public:
    CSupportLineCreateDlg(CWnd* pParent = NULL);
    virtual ~CSupportLineCreateDlg();
    virtual void Execute();

    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    virtual void UpdateBuffer();

    enum { IDD = IDD_TM_SUPPORT_LINE_CREATE };

    // Supporting Nodes
    CMouseEdit m_edtSupNode;

    // Guide Line 1 (Start)
    CMouseEdit m_edtGuideLine1;
    mit::frx::MButton m_chkGuideLine1Ortho;

    // Guide Line 2 (End)
    CMouseEdit m_edtGuideLine2;
    mit::frx::MButton m_chkGuideLine2Ortho;

	int m_nDirection; // 0: X-Dir., 1: Y-Dir.

    T_SPTL_D m_Data;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    CDBDoc* m_pDoc;

    virtual BOOL OnInitDialog();

    afx_msg void OnSetFocusSupNodeEdit();
    afx_msg void OnKillFocusSupNodeEdit();
    afx_msg void OnSetFocusGuideLine1Edit();
    afx_msg void OnKillFocusGuideLine1Edit();
    afx_msg void OnSetFocusGuideLine2Edit();
    afx_msg void OnKillFocusGuideLine2Edit();

    LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
    LRESULT OnMouseEditEscape(WPARAM wParam, LPARAM lParam);
    LRESULT OnMViewCancel(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

    void InitSelectionFilter();
    void InitCtrl();

    void Dlg2Data();

    // Guide Line으로부터 Node 추출/생성
    T_NODE_K GetNodeByGuideLine(CArray<int, int>& aGuideNodeK, bool bOrtho);

private:
    // 유틸리티 함수들
    UINT FindNodeByCoord(double x, double y, double z, double tolerance = 0.001);
    double DistancePointToSegment(const T_NODE_D& point, const T_NODE_D& seg1, const T_NODE_D& seg2);
    void ProjectPointToLine(const T_NODE_D& point, const T_NODE_D& lineStart, const T_NODE_D& lineEnd,
                            T_NODE_D& projPoint, bool& isInsideSegment);
    bool FindLineIntersection(const T_NODE_D& line1Start, const T_NODE_D& line1End,
                              const T_NODE_D& line2Start, const T_NODE_D& line2End,
                              T_NODE_D& intersection);
};

#endif
