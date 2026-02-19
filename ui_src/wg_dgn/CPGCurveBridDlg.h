// CPGCurveBridDlg.h: interface for the CCPGCurveBridDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPGCurveBridDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_CPGCurveBridDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "DgnTabCtrl.h"
#include "CPGCurveBridTabIDlg.h"
#include "CPGCurveBridTabJDlg.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\MIT_frx\MButton.h"

class CDBDoc;

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGCurveBridTabIDlg;
class CCPGCurveBridTabJDlg;

class __MY_EXT_CLASS__ CCPGCurveBridDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CCPGCurveBridDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_DGN_CPG_CURV_BRID_DLG };
	
	int	m_nOption;
	int m_nElemType;
	CDlgTabCtrl	m_Tab;
	mit::frx::MButton m_Ctrl_BothIJ;

	T_CPCB_TYPE m_I_Data;
	T_CPCB_TYPE m_J_Data;

	BOOL m_bBothIJ;
	int  m_nDgnCode;	

private:
	CArray<UINT, UINT> m_aPositionCtrl, m_aElemTypeCtrl;
		
public:
	void InitCtrl();
	void AlignControl();
	void EnableDisableControls();
	void ShowHideControls();
	void Initial_SelectItem();
	void ResizeWindow();
	void SetDgnCode(int nDgnCode);
	int	GetDgnCode();
	void SubDlgCtrlEnableDisable();
	BOOL Dlg2Data(T_CPCB_TYPE* pTfType);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();	

protected:
	CDBDoc* m_pDoc;
	CCPGCurveBridTabIDlg* m_pSubDlg_I;
	CCPGCurveBridTabJDlg* m_pSubDlg_J;

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

#endif // !defined(AFX_CPGCurveBridDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
