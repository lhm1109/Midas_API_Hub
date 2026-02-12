#pragma once

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditElem.h"
#include "..\wg_db\wg_db_EditNode.h"
#include "..\wg_db\wg_db_CobxAutoWidth.h"
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\wg_base\MChildFormView.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
struct T_SEIS_COMM_DLG_INFO 
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
	T_SEIS_COMM_DLG_INFO() { Init(); }
	T_SEIS_COMM_DLG_INFO(CMenuBarChildDlg* pDlg, UINT nDlgID, UINT nTableID)
	{
		this->pDlg     = pDlg;
		this->nDlgID   = nDlgID;
		this->nTableID = nTableID;
	}
};
typedef std::vector<T_SEIS_COMM_DLG_INFO> T_SEIS_COMM_DLG_LIST;

class CSeisCommonDlg : public MChildFormView, public CDBUpdateConnector
{
protected:
	CSeisCommonDlg();
	virtual ~CSeisCommonDlg();
	DECLARE_DYNCREATE(CSeisCommonDlg)
	enum { IDD = IDD_TM_SEISCOMMON_CONTATINER };

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
	MButton	m_wndTableBtn;
	CEditNode m_EditNode;
	CEditElem m_EditElem;
	//MButton	m_wndClose;
	//MButton	m_wndExecute;
	CPlaceHolder m_wndPlaceHolder;
	CCobxAutoWidth m_ComboMenu;
	CFoldGroupBox_Cross	m_gbStartNum;

private:
	CStringArray m_acsComboMenu;
	T_SEIS_COMM_DLG_LIST m_vChildDlg;
	CRect m_RectPlaceHolder;
	int m_nCurChildDlg;
};
