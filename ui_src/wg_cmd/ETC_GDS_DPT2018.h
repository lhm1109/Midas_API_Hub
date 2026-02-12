#pragma once

#include "MyChildDialog.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CETC_GDS_DPT2018 : public CMyChildDialog
{
public:
    CETC_GDS_DPT2018(T_SPFC_D* pParamData, CWnd* pParent = NULL);
    enum { IDD = IDD_ETC_GDS_DPT2018 };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

public:
    virtual void Data2Dlg() override final;
    virtual BOOL Dlg2Data() override final;
    virtual BOOL ISVALID() override final;
    virtual void MakeSpectrumData(BOOL bOnlyCalc = FALSE) override final;
    virtual void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& radPeriod, CArray <double, double>& radAccel) override final;

private:
    afx_msg void OnChangeRegion();
    afx_msg void OnChangeCalcMethod();
    afx_msg void OnChangeCodeCalc();
    afx_msg void OnChangeSeisData();
    afx_msg void OnChangeRiskCategory();
	afx_msg void OnSelChangeCmdSsCmb();
	afx_msg void OnSelChangeCmdS1Cmb();
	afx_msg void OnSelChangeCmdIFCmb();
	afx_msg void OnSelChangeCmdMFCmb();
	afx_msg void OnSelChangeCmdDampingCmb();
    DECLARE_MESSAGE_MAP()

private:
    void SetAlign();
    void InitCombo();
    void InitData();
    void InitDataForPushover();

private:
    bool GetDlgData();
    void SetRegion(UINT nRegion);
    void SetCalcMethod(UINT nMethod);
    EN_DPT_SEIS_REGION GetRegion();
    EN_DPT_SEIS_METHOD GetCalcMethod();

private:
    void SetComboData(MComboBox& rCombo, const double& dValue, int nDecimal = 2);
    void SetCtrlByRegion(UINT unRegion, bool bByCode);
    void SetCtrlByCalcMethod(bool bEnable);
    double GetComboData(const MComboBox& cCombo);

private:
    void UpdateSeisData(const T_SPFC_DPT2018& cData);
    bool CheckData(const T_SPFC_DPT2018& cData);
    CString GetDescription(const T_SPFC_DPT2018& cData);

private:
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

	MComboBox m_cmbRiskCate; // Risk Category
	MComboBox m_cmbImpoFact; // Importance Factor
	MComboBox m_cmbModiFact; // Response Modification Factor
	MComboBox m_cmbDampRato; // Damping Ratio

private:
    T_SPFC_D* m_pParamData;
    T_SPFC_DPT2018 m_Data;
};
