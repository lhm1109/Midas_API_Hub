#pragma once

#include "../wg_base/DialogMove.h"
#include "../wg_db/TextUnit.h"
#include "../wg_db/EditUnit.h"

class CDBDoc;

class CCMTlgtEurocodeTrainTypeBDlg final :
    public CDialogMove
{
public:
    CCMTlgtEurocodeTrainTypeBDlg(CDBDoc* pDoc, CWnd* pParent);
    virtual ~CCMTlgtEurocodeTrainTypeBDlg() override;

    enum { IDD = IDD_ETC_TLGT_EU_TRAIN_TYPE_B_DLG };

public:
    bool SetSuperControl(CWnd* pControl);
    bool GetOptionValueNU(UINT& runForceNumber, double& rdSpace);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

private:
    bool IsValid(CString& str);
    void SetForceAndClear(CEditUnit& pEditUnit);
    void SetInitPosbySuperControl();

    DECLARE_MESSAGE_MAP()
    afx_msg void OnOK();
    afx_msg void OnCancel();

private:
    CEditUnit m_EdtForceNumb;
    CEditUnit m_EdtSpace;
    CTextUnit m_StcSpaceUnit;

    CWnd* m_SuperControl;
};