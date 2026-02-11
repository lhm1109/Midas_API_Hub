#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
using namespace mit::frx;

class CCMWindItemDlg;
class CCMWindItemDPT2007 : public CChildDialog
{
public:
    CCMWindItemDPT2007(CWnd* pParent = NULL);
    enum { IDD = IDD_ETC_WIND_ITEM_DPT2007 };

public:
    virtual BOOL DestroyWindow();
    virtual void DoDataExchange(CDataExchange* pDX);

public:
    void SetData2Dlg(T_WIND_DPT2007& data, BOOL bModify, T_KEY KeyWAorWT);
    BOOL SetDlg2Data(T_WIND_DPT2007& data);
    void GetWnatData(T_WNAT_D& WnatD) { WnatD.arWnatStor.Copy(m_WnatD.arWnatStor); }

protected:
    virtual BOOL OnInitDialog();
    afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

private:
    afx_msg void OnChangeMethod();
    afx_msg void OnChangeTopographicEffect();
    afx_msg void OnButtonGustFactor();
    afx_msg void OnButtonWindVibration();

private:
    void InitDefaultData();
    void Data2Dlg();
    BOOL Dlg2Data();
    void UpdateGridData(UINT nMethod);

private:
    void AlignCtrl();
    void InitFold();
    void InitUnit();
    void SetAlign();
    void InitCombo(UINT nMethod);
    void InitComboTerrain(UINT nMethod);

private:
    void GetStoryData(T_CMD_WIND_STORY_D& rStoryD);
    void GetStoryMass(double& rdMassX, double& rdMassY, double& rdMassXY, double& rdTotal);

private:
    EN_DPT_WIND_METHOD GetCalcMethod();
    void SetCalcMode(UINT nMethod);
    void SetCtrlByCalcMethod(UINT nMethod);
    void SetEnableTopographic(BOOL bTopographic);
    void SetEnableAdditional_Ecc(UINT nMethod);
    void SetEnableGustFactor(UINT nMethod);

private:
    CFoldGroupBox_Cross m_frmWindPara;

    MComboBox m_cmbWindZone;
    CEditUnit m_edtWindVelo;
    MComboBox m_cmbTrrnCate;
    MComboBox m_cmbImpoFact;

    MButton   m_chkTopoEfft;
    MComboBox m_cmbHillShap;
    MComboBox m_cmbBldgLoca;
    CEditUnit m_edtHillHght;
    CEditUnit m_edtHillLeng;
    CEditUnit m_edtBldgDist;
    CTextUnit m_untHillHght;
    CTextUnit m_untHillLeng;
    CTextUnit m_untBldgDist;

    CEditUnit m_edtGustMajr;
    CEditUnit m_edtGustOrth;

    MButton   m_chkWindCros;
    MButton   m_chkWindTors;
    MButton   m_chkWindResp;

    CArray<UINT, UINT> m_aCtrlExRadio;
    CArray<UINT, UINT> m_aCtrlEyRadio;

private:
    CCMWindItemDlg* m_pParent;
    T_WIND_DPT2007 m_Data;
    bool m_bInit;
    bool m_bModify;

    T_WNAT_D m_WnatD;
    T_KEY m_KeyWAorWT;
};
