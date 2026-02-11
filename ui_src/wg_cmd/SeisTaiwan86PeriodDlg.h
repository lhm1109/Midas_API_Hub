#pragma once

#include "..\wg_db\wg_db_EditUnit.h"

enum DlgModeTaiwan : UINT
{
    EN_DLG_MODE_TAIWAN_1999 = 1,
    EN_DLG_MODE_TAIWAN_2006,
    EN_DLG_MODE_TAIWAN_2011,
    EN_DLG_MODE_TAIWAN_2022,
};

class CSeisTaiwan86PeriodDlg : public CDialogMove
{
public:
    CSeisTaiwan86PeriodDlg(int nMode, bool bHideDirY, CWnd* pParent = NULL);
    enum { IDD = IDD_ETC_SEIS_CALCPERIOD_TAIWAN86 };

public:
    BOOL CalcPeriod(double dHn, double dAc, double& dPeriod, int nMethod);

private:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

private:
    void OnOK();
    void AlignDialog();
	void HideCtrlDirY();
    void ChangeCaption();

private:
    afx_msg void OnRdoXmethodChange();
    afx_msg void OnRdoYmethodChange();
    DECLARE_MESSAGE_MAP()

public:
    int	m_nMethodX;
    int	m_nMethodY;

    double m_dPeriodX;
    double m_dPeriodY;
    double m_dHnX;
    double m_dHnY;
    double m_dAcX;
    double m_dAcY;

private:
    CEditUnit m_wndAcY;
    CEditUnit m_wndAcX;
    CEditUnit m_wndHnY;
    CEditUnit m_wndHnX;

private:
    int m_nMode; // EN_DLG_MODE_TAIWAN_XXXX
	bool m_bHideDirY;
};
