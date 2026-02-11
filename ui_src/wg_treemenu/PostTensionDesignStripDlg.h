#if !defined(__POSTTENSIONDESIGNSTRIPCOMMONDLG_H__)
#define __POSTTENSIONDESIGNSTRIPCOMMONDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditElem.h"
#include "..\wg_db\wg_db_EditNode.h"
#include "..\wg_db\wg_db_CobxAutoWidth.h"
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\mit_frx\MButton.h"
#include "..\wg_base\MChildFormView.h"

struct T_PTDS_COMM_DLG_INFO 
{
    CMenuBarChildDlg* pDlg;
    UINT nDlgID;
    UINT nTableID;
    void Init()
    {
        pDlg = nullptr;
        nDlgID = 0;
        nTableID = 0;
    }
    T_PTDS_COMM_DLG_INFO() { Init(); }
    T_PTDS_COMM_DLG_INFO(CMenuBarChildDlg* pDlg, UINT nDlgID, UINT nTableID)
    {
        this->pDlg     = pDlg;
        this->nDlgID   = nDlgID;
        this->nTableID = nTableID;
    }
};
typedef std::vector<T_PTDS_COMM_DLG_INFO> T_PTDS_COMM_DLG_LIST;

class CPostTensionDesignStripCommonDlg : public MChildFormView, public CDBUpdateConnector
{
protected:
    CPostTensionDesignStripCommonDlg();
    virtual ~CPostTensionDesignStripCommonDlg();
    DECLARE_DYNCREATE(CPostTensionDesignStripCommonDlg)
    enum { IDD = IDD_TM_DESIGNSTRIP_CONTATINER };

private:
    virtual void OnInitialUpdate();
    virtual void OnTabSelect(WPARAM wParam, LPARAM lParam);
    virtual void OnTabUnSelect(WPARAM wParam, LPARAM lParam);
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    virtual CWnd* GetDlgByID(int Mode);

private:
    afx_msg void OnTmClose();
    afx_msg void OnComboMenuSelect();
    afx_msg void OnTmExecute();
    afx_msg void OnViewStartNodeNum();
    afx_msg void OnViewStartElemNum();
    afx_msg void OnTmTableBtn();
    DECLARE_MESSAGE_MAP()

public:
    BOOL ShowChildDlgByID(int nID);

private:
    void ShowCurChildDlg(int nDlgIndex);
    void DeleteChildDlgs();
    void CreateChildDlgs();
    void InitContent();
    void InitComboMenu(int nInitSel=0);
    void ShowExecuteButton(UINT nShowHide);
    void OnTmExecAndClose(BOOL bExecute);

private:
    void UpdateBuffer();

private:
    void GetComboMenuItem(CStringArray& raMenuItem, int nMethod);

private:
    mit::frx::MButton m_wndTableBtn;
    CEditNode m_EditNode;
    CEditElem m_EditElem;
    mit::frx::MButton m_wndClose;
    mit::frx::MButton m_wndExecute;
    CPlaceHolder m_wndPlaceHolder;
    CCobxAutoWidth m_ComboMenu;
    CFoldGroupBox_Cross m_gbStartNum;

private:
    CStringArray m_acsComboMenu;
    T_PTDS_COMM_DLG_LIST m_vChildDlg;
    CRect m_RectPlaceHolder;
    int m_nCurChildDlg;
};

#endif
