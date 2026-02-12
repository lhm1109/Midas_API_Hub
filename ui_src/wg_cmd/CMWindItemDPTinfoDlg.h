#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;

class CCMWindItemDPTinfoDlg : public CDialogMove
{
public:
    CCMWindItemDPTinfoDlg(CWnd* pParent = nullptr);
    enum { IDD = IDD_ETC_WIND_ITEM_DPT_INFO_DLG };

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()

private:
    afx_msg void OnButtonOK();
    afx_msg void OnButtonCancel();

public:
    void SetData(const T_WIND_DPT2007* pData);

private:
    void InitUnit();
    void SetAlign();
    void InitCombo();
    bool Data2Dlg();
    bool Dlg2Data();

private:
    void SetEnableStructType(bool bEnable);

private:
    MComboBox m_cmbStrtType;
    CEditUnit m_edtSizeMajr;
    CEditUnit m_edtSizeOrth;
    CTextUnit m_untSizeMajr;
    CTextUnit m_untSizeOrth;

    CEditUnit m_edtFreqMajr;
    CEditUnit m_edtFreqOrth;
    CEditUnit m_edtDampMajr;
    CEditUnit m_edtDampOrth;

private:
    T_WIND_DPT2007* m_pData;
};
