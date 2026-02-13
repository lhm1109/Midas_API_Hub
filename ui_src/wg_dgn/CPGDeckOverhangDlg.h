// CPGDeckOverhangDlg.h: interface for the CCPGDeckOverhangDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPGDECKOVERHANGDLG_H__)
#define AFX_CPGDECKOVERHANGDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "DgnTabCtrl.h"
#include "CPGDeckOverhangPage.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"

using namespace mit::frx;

class CDBDoc;

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGDeckOverhangPage;

class __MY_EXT_CLASS__ CCPGDeckOverhangDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CCPGDeckOverhangDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_DGN_CPG_DECK_OVERHANG_DLG };
	
	int		m_nOption;
	int     m_nElemType;
	BOOL    m_bBoth;
	CDlgTabCtrl	m_Tab;

	T_DOHL_TYPE m_Data_I;
	T_DOHL_TYPE m_Data_J;

	int  m_nDgnCode;
		
public:
	void Initial_SelectItem();
	void SetDgnCode(int nDgnCode);
	int  GetDgnCode();
	void SubDlgCtrlEnableDisable();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();	

protected:
	CDBDoc* m_pDoc;

	void InitCtrl();
	void AlignControl();
	void EnableDisableControls();
	void ShowHideControls();
	BOOL Dlg2Data(T_DOHL_TYPE* pDohlType);

	T_DOHL_D m_Data;

	CCPGDeckOverhangPage* m_pSubDlg_I;
	CCPGDeckOverhangPage* m_pSubDlg_J;

	CArray<UINT, UINT> m_aElemTypeCtrl, m_aPositionCtrl;

	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnCPGDOHLAddDel();
	afx_msg void OnDgnCPGElemTypeRdo();
	afx_msg void OnBothIJChk();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

#endif // !defined(AFX_CPGDECKOVERHANGDLG_H__)
