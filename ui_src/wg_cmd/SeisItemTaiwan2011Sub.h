#pragma once

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;
class CSeisItemTaiwan2011Sub : public CDialogMove
{
public:
    CSeisItemTaiwan2011Sub(CWnd* pParent = NULL);
    enum { IDD = IDD_ETC_SEIS_ITEM_TAIWAN11_SUB };

public:
    static CString GetSeismicZoneName(int nCode);
    static CString GetSoilTypeName(int nCode);
    void SetModifyData(T_SEIS_TAIWAN02* pData) { m_pData = pData; }
    void SetSeismicZone(int nZone) { m_nSeisZone = nZone; }

private:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

private:
    void InitSpecAccel();
    void InitSoilType();
    void InitTaipai();
    void CtrlManager();

    void AlignCtrl();
    void Data2Dlg();
    BOOL Dlg2Data();
    void SetSiteMagnifyFactor();
    void SetChangeTaipai();
    void ChangeCaption();

private:
    afx_msg void OnOk();
    afx_msg void OnCancel();
    afx_msg void OnSelChangeSoilType();
    afx_msg void OnSelChangeSubZone();
    afx_msg void OnSelChangeSpecAccel();
    afx_msg void OnEdtChangeSpecAccel();
    afx_msg void OnEdtChangeNSFactorEdt();
    DECLARE_MESSAGE_MAP()

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
    MComboBox m_cbxSubZone_taipei;
    CEditUnit m_edtSds_T;
    CEditUnit m_edtSms_T;
    CEditUnit m_edtTm0;
    CEditUnit m_edtTd0;

private:
    int m_nSeisZone;
    T_SEIS_TAIWAN02* m_pData;

    CArray<UINT, UINT> m_aCtrlMethod0;
    CArray<UINT, UINT> m_aCtrlMethod1;
    CArray<UINT, UINT> m_aCtrlMethod2;
    CArray<UINT, UINT> m_aCtrlMethod_SMF;
};
