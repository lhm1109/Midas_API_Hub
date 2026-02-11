#pragma once

#include "..\mit_frx\MComboBox.h"
#include "..\wg_db\wg_db_EditUnit.h"
using namespace mit::frx;
class CSeisItemTaiwan2002Sub : public CDialogMove
{
public:
    CSeisItemTaiwan2002Sub(CWnd* pParent = NULL);
    enum { IDD = IDD_ETC_SEIS_ITEM_TAIWAN02_SUB };

private:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);

private:
    afx_msg void OnOk();
    afx_msg void OnCancel();
    afx_msg void OnSelchangeCmdTaiwan02SoilTypeCmb();
    afx_msg void OnSelchangeCmdTaiwan02SubzoneCmb();
    afx_msg void OnSelchangeCmdTaiwan02SpecAccelCmb();
    afx_msg void OnEditchangeCmdTaiwan02SpecAccelCmb();
    afx_msg void OnEditchangeCmdTaiwan02NSFactorEdt();
    DECLARE_MESSAGE_MAP()

public:
    static CString GetSeismicZoneName(int nCode);
    static CString GetSoilTypeName(int nCode);
    void SetModifyData(T_SEIS_TAIWAN02* pData) { m_pData = pData; };
    void SetSeismicZone(int nZone) { m_nSeisZone = nZone; };

private:
    void AlignCtrl();
    void InitSpecAccel();
    void InitSoilType();
    void InitTaipai();
    void CtrlManager();

    void Data2Dlg();
    BOOL Dlg2Data();
    void SetSiteMagnifyFactor();
    void SetChangeTaipei();

private:
    MComboBox m_cbxSds;
    MComboBox m_cbxSd1;
    MComboBox m_cbxSms;
    MComboBox m_cbxSm1;
    CEditUnit m_edtNdv;
    CEditUnit m_edtNda;
    CEditUnit m_edtNmv;
    CEditUnit m_edtNma;
    CEditUnit m_edtSeisZone;
    MComboBox m_cbxSoilType;
    CEditUnit m_edtFmv;
    CEditUnit m_edtFma;
    CEditUnit m_edtFdv;
    CEditUnit m_edtFda;
    MComboBox m_cbxSubZone_taipai;
    CEditUnit m_edtSds_T;
    CEditUnit m_edtSms_T;
    CEditUnit m_edtTm0;
    CEditUnit m_edtTd0;

    CArray<UINT, UINT> m_aCtrlMethod0;
    CArray<UINT, UINT> m_aCtrlMethod1;
    CArray<UINT, UINT> m_aCtrlMethod2;
    CArray<UINT, UINT> m_aCtrlMethod_SMF;

private:
    T_SEIS_TAIWAN02* m_pData;
    int m_nSeisZone;
};
