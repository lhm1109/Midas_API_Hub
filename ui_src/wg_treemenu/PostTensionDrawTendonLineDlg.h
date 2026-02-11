#if !defined(__POSTTENSIONDRAWTENDONLINE__)
#define __POSTTENSIONDRAWTENDONLINE__

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditElem.h"
#include "..\wg_db\wg_db_EditNode.h"
#include "..\wg_db\EditTdbm.h"

#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\wg_base\MChildFormView.h"

#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"

class CPostTensionDrawTendonLineDlg : public MChildFormView
{
protected:
	CPostTensionDrawTendonLineDlg();
	DECLARE_DYNCREATE(CPostTensionDrawTendonLineDlg)

public:
	enum { IDD = IDD_TM_TENDON };
	mit::frx::MButton	m_wndTableBtn;
	CEditNode	m_EditNode;
	CEditTdbm	m_EditTdbm;
	//mit::frx::MButton	m_wndClose;
	//mit::frx::MButton	m_wndExecute;
	CPlaceHolder	m_wndPlaceHolder;
	mit::frx::MComboBox	m_ComboMenu;
	CFoldGroupBox_Cross	m_gbStartNum;

public:
	CStringArray m_ComboMenuStr;
	CArray<CMenuBarChildDlg*, CMenuBarChildDlg*> m_ChildDlgs;
	CArray<int, int>                               m_DlgIDs;
	CArray<UINT, UINT>                            m_TableIDs;     // 콤보박스 옆에 있는 _T("...")버튼 클릭시에 Main에 날릴 CommandID, (아마도 Table을 보여주라는 명령)
	int m_nCurChildDlg;
	CRect m_RectPlaceHolder;

	int m_nOrgSlabPlaceHolderHeight;

public:
	int  _GetHeightFromFont();
	BOOL ShowChildDlgByID(int nID);
	void ShowCurChildDlg(int nDlgIndex);
	void DeleteChildDlgs();
	void CreateChildDlgs();
	void InitContent();
	void InitComboMenu();
	void AdjustComboListBox();
	void SetHolderSize(int nHeight);
	void ShowExecuteButton(UINT nShowHide);
	virtual void OnTabSelect(WPARAM wParam, LPARAM lParam);
	virtual void OnTabUnSelect(WPARAM wParam, LPARAM lParam);
	virtual CWnd* GetDlgByID(int Mode);
	void OnTmExecAndClose(BOOL bExecute);
	int GetOrgSlabPlaceHolderSize();

public:
	virtual void OnInitialUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual ~CPostTensionDrawTendonLineDlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	afx_msg void OnTmClose();
	afx_msg void OnComboMenuSelect();
	afx_msg void OnTmExecute();
	afx_msg void OnViewStartNodeNum();
	afx_msg void OnViewStartElemNum();
	afx_msg void OnTmTableBtn();

	DECLARE_MESSAGE_MAP()
};

#endif