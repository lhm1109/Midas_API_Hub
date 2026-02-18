// CPGShearConnDlg.h: interface for the CCPGShearConnDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "CPGShearConnTabIDlg.h"
#include "CPGShearConnTabJDlg.h"


#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "DgnTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\MIT_frx\MButton.h"

class CDBDoc;

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGShearConnTabIDlg;
class CCPGShearConnTabJDlg;

class __MY_EXT_CLASS__ CCPGShearConnDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CCPGShearConnDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_DGN_CPG_SHEAR_CONN_DLG };
	
	int	m_nOption;
	int m_nElemType;
	mit::frx::MButton m_Ctrl_BothIJ;
	CDlgTabCtrl	m_Tab;

	T_CPSC_TYPE m_I_Data;
	T_CPSC_TYPE m_J_Data;

	BOOL m_bBothIJ;
	int  m_nDgnCode;

private:
	CArray<UINT, UINT> m_aPositionCtrl, m_aElemTypeCtrl;

public:
	void Initial_SelectItem();
	void ResizeWindow();

protected:
	void InitCtrl();
	void AlignControl();
	void EnableDisableControls();
	void ShowHideControls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();	

// Implementation
protected:
	CDBDoc* m_pDoc;
	BOOL Dlg2Data(T_CPSC_TYPE* pTfType);

	CCPGShearConnTabIDlg* m_pSubDlg_I;
	CCPGShearConnTabJDlg* m_pSubDlg_J;

	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnCPGLoadAppAddDel();
	afx_msg void OnBothIJChk();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnCPGElemTypeRdo();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////