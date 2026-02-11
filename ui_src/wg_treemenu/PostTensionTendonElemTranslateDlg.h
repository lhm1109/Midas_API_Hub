#if !defined(__POSTTENSIONTENDONELEMTRANSLATEDLG_H__)
#define __POSTTENSIONTENDONELEMTRANSLATEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PostTensionTendonElemTranslateDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"

#define TENDONELEMTRANS_XAXIS 0
#define TENDONELEMTRANS_YAXIS 1
#define TENDONELEMTRANS_ZAXIS 2
#define TENDONELEMTRANS_ARBIT 3

class _TendonElemTranslate
{
public:
    BOOL m_bIsCopy;
    int  m_NTimes;
    int  m_nOption;
    int  m_nNodeInc;
    double m_EQ_UX, m_EQ_UY, m_EQ_UZ;
    double m_EQ_WX, m_EQ_WY, m_EQ_WZ;
    int  m_nUEQ_Axis;
    CArray<double, double> m_UEQ_DistList;
    double m_UEQ_UX, m_UEQ_UY, m_UEQ_UZ;
    double m_UEQ_WX, m_UEQ_WY, m_UEQ_WZ;
    BOOL m_bAttrNodeCopy;
    BOOL m_bAttrElemCopy;
    int  m_nMatlInc;
    int  m_nSectInc;
    int  m_nThikInc;
    BOOL m_bMatlIncRep;
    BOOL m_bSectIncRep;
    BOOL m_bThikIncRep;
    BOOL m_bIntstNode;
    BOOL m_bIntstElem;
    BOOL m_bDelFreeNode;
};

class CPostTensionTendonElemTranslateDlg : public CMenuBarChildDlg
{
public:
    CPostTensionTendonElemTranslateDlg(CWnd* pParent = NULL);
    virtual void Execute();

    BOOL GetParameter(_TendonElemTranslate& ET);
    void MoveElem(_TendonElemTranslate& ET);
    void CopyElem(_TendonElemTranslate& ET);

    CArray<UINT, UINT> m_EnableOnMove;
    CArray<UINT, UINT> m_NodeInc;
    CArray<UINT, UINT> m_EqualDistance;
    CArray<UINT, UINT> m_UEqualDistance;
    CArray<UINT, UINT> m_aIterNum;
    CArray<UINT, UINT> m_aIterNum2;

    enum { IDD = IDD_TM_TENDON_ELEM_TRANSLATE };
    CFormulaEditSpin m_wndIterNum2;
    CMouseEdit m_wndDirVector;
    CMouseEdit m_wndEQDist;
    mit::frx::MEdit m_wndUEQDist;
    CFormulaEditSpin m_wndIterNum;
    CFormulaEditSpin m_wndNodeInc;
    CTextUnit m_DistUnit3;
    CTextUnit m_DistUnit2;
    CTextUnit m_DistUnit1;
    int m_nCopyOrMove;
    int m_nOption;
    int m_nChooseAxis;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnTmCopyRadio();
    afx_msg void OnTmMoveRadio();
    afx_msg void OnTmAxisRadios();
    afx_msg void OnDeltaposTmIterNumSpin(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnTmNodeIncRadio();
    afx_msg void OnTmEqualDistRadio();
    afx_msg void OnTmUnequalDistRadio();
    afx_msg void OnDeltaposTmIncSpin(NMHDR* pNMHDR, LRESULT* pResult);
    DECLARE_MESSAGE_MAP()
};

#endif // !defined(__POSTTENSIONTENDONELEMTRANSLATEDLG_H__)
