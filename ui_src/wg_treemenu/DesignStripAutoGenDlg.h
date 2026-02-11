#if !defined(__DESIGNSTRIPAUTOGENDLG_H__)
#define __DESIGNSTRIPAUTOGENDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"

class CDesignStripAutoGenDlg : public CMenuBarChildDlg
{
public:
    CDesignStripAutoGenDlg(CWnd* pParent = NULL);
    virtual ~CDesignStripAutoGenDlg();

    virtual void Execute();

    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    virtual void UpdateBuffer();

    // Dialog Data
    enum { IDD = IDD_TM_DESIGN_STRIP_AUTO_GEN };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    CDBDoc* m_pDoc;

    virtual BOOL OnInitDialog();

private:
    afx_msg void OnChangeMethod();
    DECLARE_MESSAGE_MAP()

private:
    int nMethod;
};

#endif // !defined(__DESIGNSTRIPAUTOGENDLG_H__)