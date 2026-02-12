#pragma once

#include "..\wg_db\wg_db_EditUnit.h"

class CSeisItemDPTPeriodDlg : public CDialogMove
{
public:
    CSeisItemDPTPeriodDlg(CWnd* pParent = nullptr);
    enum { IDD = IDD_ETC_SEIS_ITEM_DPT_PERIOD_DLG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

private:
    afx_msg void OnChangeMajorEQ();
    afx_msg void OnChangeOrthoEQ();
    afx_msg void OnBtnCalcMajor();
    afx_msg void OnBtnCalcOrtho();
    afx_msg void OnOK();
    afx_msg void OnCancel();
    DECLARE_MESSAGE_MAP()

public:
    void SetData(const double& dH);
    bool GetPeriod(double& rdMajorT, double& rdOrthoT);

private:
    void InitUnit();
    void SetAlign();
    void Data2Dlg();

private:
    void SetMethod(bool bMajor, UINT nMethod);
    UINT GetMethod(bool bMajor);
    void SetCtrlByMethod(bool bMajor, UINT nMethod);
    double CalcPeriod(bool bMajor);
    double CalcPeriod(UINT nMethod, const double& dH, const double& dN);

private:
    CEditUnit m_edtMajorH;
    CEditUnit m_edtMajorN;
    CEditUnit m_edtMajorT;

    CEditUnit m_edtOrthoH;
    CEditUnit m_edtOrthoN;
    CEditUnit m_edtOrthoT;

private:
    double m_dHeight;
    double m_dDefaultN;
    double m_dMajorT;
    double m_dOrthoT;
};
