#pragma once

#include "SeisItemDlgNew.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MEdit.h"
using namespace mit::frx;
class CSeisItemDPT2018 : public CChildDialog
{
public:
    CSeisItemDPT2018(CWnd* pParent = nullptr);
    enum { IDD = IDD_ETC_SEIS_ITEM_DPT2018 };

public:
    virtual BOOL DestroyWindow();
    virtual void DoDataExchange(CDataExchange* pDX);

public:
    void SetData2Dlg(T_SEIS_DPT2018& data, BOOL bModify);
    BOOL SetDlg2Data(T_SEIS_DPT2018& data);

protected:
    virtual BOOL OnInitDialog();
    afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

private:
    afx_msg void OnChangeRegion();
    afx_msg void OnChangeCalcMethod();
    afx_msg void OnChangeCodeCalc();
    afx_msg void OnChangeSeisData();
    afx_msg void OnChangeRiskCategory();
    afx_msg void OnChangePeriodCalcMethod();
    afx_msg void OnSelchangeCmdZoneCmb();
    afx_msg void OnSelChangeCmdSsCmb();
    afx_msg void OnSelChangeCmdS1Cmb();
    afx_msg void OnSelChangeCmdIFCmb();
    afx_msg void OnSelChangeCmdMFMajorCmb();
    afx_msg void OnSelChangeCmdMFOrthoCmb();
    afx_msg void OnSelChangeCmdDampingCmb();
    afx_msg void OnBtnApproximatePeriod();
    afx_msg void OnChangePeriod();

private:
    void Data2Dlg();
    BOOL Dlg2Data();
    void InitData();

private:
    void InitFold();
    void InitUnit();
    void SetAlign();
    void InitCombo();

private:
    void SetRegion(UINT nRegion);
    void SetCalcMethod(UINT nMethod);
    void SetPeriodCalcMethod(UINT nCalcMethod);
    EN_DPT_SEIS_REGION GetRegion();
    EN_DPT_SEIS_METHOD GetCalcMethod();
    EN_DPT_SEIS_PERIOD GetPeriodCalcMethod();

    void SetComboData(MComboBox& rCombo, const double& dValue, int nDecimal = 2);
    void SetSeismicDesignCategory(const T_SEIS_DPT2018& cData);
    void UpdateFundamentalPeriod();

private:
    void UpdateSeisData(const T_SEIS_DPT2018& cData);
    void SetCtrlByRegion(UINT unRegion, bool bByCode);
    void SetCtrlByCalcMethod(bool bEnable);
    void SetCtrlPeriod(bool bEnable);
    void SetCtrlByPeriodCalcMethod(UINT nMethod);

private:
    double GetComboData(const MComboBox& cCombo);
    double CalcCu(const double& dSd1);
    double GetBuildingHeight();
    CString GetSelItem(const std::vector<std::pair<CString, DWORD>>& vList, int nSel);

private:
    CFoldGroupBox_Cross m_frmSeisPara;

    MButton m_rdoRegionBK; // Bangkok
    MButton m_rdoRegionEx; // Region except Bangkok
    MButton m_rdoCalcGrap; // By Graph 1.4.6~7
    MButton m_rdoCalcTabl; // By Table 1.4-4~5

    MComboBox m_cmbSeisZone;

    MComboBox m_cmbSiteClas;
    MButton   m_chkCodeCalc;
    MComboBox m_cmbSpectSss;
    CEditUnit m_edtSpectFaa;
    CEditUnit m_edtSpectSds;
    MComboBox m_cmbSpectS1s;
    CEditUnit m_edtSpectFvv;
    CEditUnit m_edtSpectSd1;
    CEditUnit m_edtSpectCuu; // Period Coefficient ( Cu )

    MComboBox m_cmbRiskCate; // Risk Category
    MComboBox m_cmbImpoFact; // Importance Factor

    MEdit     m_edtSDCatSds; // Seismic Design Category by Sds
    MEdit     m_edtSDCatSd1; // Seismic Design Category by Sd1
    MEdit     m_edtSDCatFin;

    MButton   m_rdoPeriAnal; // Analytical Period
    MButton   m_rdoPeriAppr; // Approximate Period
    CEditUnit m_edtAnalMajr;
    CEditUnit m_edtAnalOrth;
    CEditUnit m_edtApprMajr;
    CEditUnit m_edtApprOrth;
    CEditUnit m_edtPeriMajr; // Fundamental Period
    CEditUnit m_edtPeriOrth;
    MComboBox m_cmbFactMajr; // Response Modification Factor
    MComboBox m_cmbFactOrth;
    MComboBox m_cmbDampRato; // Damping Ratio

private:
    BOOL m_bInit;
    BOOL m_bModify;
    T_SEIS_DPT2018  m_Data;
    CSeisItemDlgNew* m_pParent;
};
