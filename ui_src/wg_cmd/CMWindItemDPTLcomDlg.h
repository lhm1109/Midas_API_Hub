#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

class CCMWindItemDPTLcomDlg : public CDialogMove
{
public:
    CCMWindItemDPTLcomDlg(CWnd* pParent = nullptr);
    enum { IDD = IDD_ETC_WIND_ITEM_DPT_LCOM_DLG };

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()

private:
    afx_msg void OnButtonOK();
    afx_msg void OnButtonCancel();

public:
    void SetData(EN_DPT_WIND_METHOD enMethod, const T_WIND_DPT2007_VIBR* pData);

private:
    void InitUnit();
    void SetAlign();

private:
    bool Data2Dlg();
    bool Dlg2Data();

private:
    void SetEnableCtrl();
    void GetVibrFlag(UINT nMethod, const T_WIND_DPT2007_VIBR& cVibr, bool bUse[EN_DPT_WIND_VIBR_NUM]);

private:
    CEditUnit m_edtFreqAlng;
    CEditUnit m_edtFreqCros;
    CEditUnit m_edtFreqTors;

    CEditUnit m_edtMassAlng;
    CEditUnit m_edtMassCros;
    CEditUnit m_edtMassTors;
    CTextUnit m_untMassAlng;
    CTextUnit m_untMassCros;
    CTextUnit m_untMassTors;

    CEditUnit m_edtDampAlng;
    CEditUnit m_edtDampCros;
    CEditUnit m_edtDampTors;

private:
    T_WIND_DPT2007_VIBR* m_pData;
    EN_DPT_WIND_METHOD m_enCalcMethod;
};
