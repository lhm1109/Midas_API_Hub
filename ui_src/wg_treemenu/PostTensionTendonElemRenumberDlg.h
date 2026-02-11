#if !defined(__POSTTENSIONTENDONELEMRENUMBERDLG_H__)
#define __POSTTENSIONTENDONELEMRENUMBERDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PostTensionTendonElemRenumberDlg.h : header file
//

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MComboBox.h"

class CPostTensionTendonElemRenumberDlg : public CMenuBarChildDlg
{
public:
    CPostTensionTendonElemRenumberDlg(CWnd* pParent = NULL);
    virtual void Execute();

    enum { IDD = IDD_TM_TENDON_ELEM_RENUMBER };
    mit::frx::MComboBox m_SortMethodCmb;
    mit::frx::MComboBox m_RefAxisCmb;
    mit::frx::MComboBox m_PolAxisCmb;
    mit::frx::MComboBox m_CynOrderCmb1;
    mit::frx::MComboBox m_CynOrderCmb2;
    mit::frx::MComboBox m_CynOrderCmb3;
    mit::frx::MComboBox m_CarOrderCmb1;
    mit::frx::MComboBox m_CarOrderCmb2;
    mit::frx::MComboBox m_CarOrderCmb3;
    CMouseEdit m_wndRot1stPoint;
    CMouseEdit m_wndRot2ndPoint;
    CMouseEdit m_wndPol1stPoint;
    CMouseEdit m_wndPol2ndPoint;
    CTextUnit m_wnd1stPointUnit;
    CTextUnit m_wnd2ndPointUnit;
    CTextUnit m_wnd1stPointUnit2;
    CTextUnit m_wnd2ndPointUnit2;
    int m_SelType;
    CString m_tmStartNode;
    CString m_tmStartElement;
    int m_nRotAxis;
    int m_nPolAxis;
    CArray<UINT, UINT> m_aCtrlCartesian, m_aCtrlCylinder;
    CArray<UINT, UINT> m_aCtrlRefAxisGlobal, m_aCtrlRefAxisTwoPnt;
    CArray<UINT, UINT> m_aCtrlPolAxisGlobal, m_aCtrlPolAxisTwoPnt;
    //CArray<UINT, UINT> m_aCtrlButton;
    CString m_szCmb[6];

    // Additional member variables
    CDBDoc* m_pDoc;
    T_NODE_K StartNode;
    T_ELEM_K StartElem;
    double m_org[3], m_rot[3], m_pol[3];

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    void SetInitCombString();
    void AlignControl();
    void SetFrameSize();
    void SetEditControl();
    void ExecuteCartesian();
    void ExecuteCylindrical();
    BOOL CheckOrthogonal();
    BOOL GetRot1stPoint(double &Px, double &Py, double &Pz);
    BOOL GetRot2ndPoint(double &Px, double &Py, double &Pz);
    BOOL GetPol1stPoint(double &Px, double &Py, double &Pz);
    BOOL GetPol2ndPoint(double &Px, double &Py, double &Pz);
    int  GetOrderOpt();
    afx_msg void OnSelchangeCynAxisOrder();
    afx_msg void OnSelchangeCynOrder1st();
    afx_msg void OnSelchangeCynOrder2nd();
    afx_msg void OnSelchangeCarOrder1st();
    afx_msg void OnSelchangeCarOrder2nd();
    afx_msg void OnSelchangeSortMethodCmb();
    afx_msg void OnTmCylinderAxisRdo();
    //afx_msg void OnTmExecute();
    //afx_msg void OnTmClose();
    DECLARE_MESSAGE_MAP()
};

#endif // !defined(__POSTTENSIONTENDONELEMRENUMBERDLG_H__)
