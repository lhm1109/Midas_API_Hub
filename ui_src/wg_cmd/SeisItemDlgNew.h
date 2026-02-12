#pragma once

#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
using namespace mit::frx;
class CSeisItemCodeMgr;
class CWindSeisAddLoadGrid;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CSeisItemDlgNew : public CDialogMove, public CDBUpdateConnector
{
public:
    CSeisItemDlgNew(CWnd* pParent = NULL);
    virtual ~CSeisItemDlgNew();
    enum { IDD = IDD_ETC_SEIS_ITEM_NEW };

public:
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    virtual void UpdateBuffer();
    virtual void DoDataExchange(CDataExchange* pDX);

public:
    void SetParamData(T_SEIS_K Key, T_SEIS_D &ParamData);
    void AddAdditionalLoad(T_STOR_K StartStorKey, T_STOR_K EndStorKey, double dblX, double dblY, double dblRZ);
    void AlignCtrl2Param(int nChildBottom);
    BOOL IsSaveFoldedInfo() { return m_bSaveFoldedInfo; }

protected:
    void Data2Dlg();
    BOOL Dlg2Data();
    void InitCodeNameCobx();
    void MakeItemEx();
    BOOL EnableNationalAnnex();
    void AdjustComboListBox();
    void EnableCtrlOfTorsion_Accidental(const int nCodeType, BOOL &bEnable); // MNET:2410-GSJI-20060906
    void EnableCtrlOfTorsion_Inherent(const int nCodeType, BOOL &bEnable); // MNET:5319-HSSHIM-20121101
    BOOL AddToDB();
protected:
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnCmdApply();
    virtual void OnCancel();
    afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnImport();
    afx_msg void OnCmdBtnProfile();
    afx_msg void OnCmdLoadcaseBtn();
    afx_msg void OnSelchangeCmdCodeName();
    afx_msg void OnSelchangeCmdNationalAnnex();
    DECLARE_MESSAGE_MAP()

private:
    int GetMoveDistNA();

public:
//     CBCGPStatic	m_wndHolder;

protected:
    BOOL m_bModify;
    BOOL m_bSaveFoldedInfo;
    CDBDoc* m_pDoc;
    T_SEIS_K m_Key, m_OldKey;
    T_SEIS_D m_Data;

    CSeisItemCodeMgr* m_pCodeMgr;
    CWindSeisAddLoadGrid* m_pGridWnd;

    CArray<UINT, UINT> m_aCtrlExRadio;
    CArray<UINT, UINT> m_aCtrlEyRadio;
    CArray<UINT, UINT> m_aMoveCtrlID;
    CArray<BOOL, BOOL> m_aCodeEnable;

private:
    MButton	m_chkInherentTorsionAmplify;
    MButton	m_chkAccidentTorsionAmplify;
    CSelectLC m_wndLoadCase;
    MComboBox m_wndCodeName;
    MComboBox m_cmbNationalAnnex;
    MEdit	  m_wndDescription;
    CEditUnit m_wndScaleGY;
    CEditUnit m_wndScaleGX;
};

#include "HeaderPost.h"
