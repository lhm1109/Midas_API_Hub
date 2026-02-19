// CPGFATIGUEPARAMDLG.h: interface for the CCPGFatigueParamDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPGFATIGUEPARAMDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_CPGFATIGUEPARAMDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "DgnTabCtrl.h"

#include "CPGFatigueParamTabIDlg.h"
#include "CPGFatigueParamTabJDlg.h"
#include "CPGFatigueParamTabIDlg_CSA.h"
#include "CPGFatigueParamTabJDlg_CSA.h"
#include "CPGFatigueParamTabIDlg_AASHTO19.h"

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

class CCPGFatigueParamTabIDlg;
class CCPGFatigueParamTabJDlg;
class CCPGFatigueParamTabIDlg_CSA;
class CCPGFatigueParamTabJDlg_CSA;
class CCPGFatigueParamTabIDlg_AASHTO19;
class __MY_EXT_CLASS__ CCPGFatigueParamDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CCPGFatigueParamDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_DGN_CPG_FATIGUE_PARAM_DLG };
	
	int	m_nOption;
	int m_nElemType;
	mit::frx::MButton m_Ctrl_BothIJ;
	CDlgTabCtrl	m_Tab;

	T_CPFP_TYPE m_I_Data;
	T_CPFP_TYPE m_J_Data;

	BOOL m_bBothIJ;
	int  m_nDgnCode;

private:
	CArray<UINT, UINT> m_aPositionCtrl, m_aElemTypeCtrl;

public:
	void Initial_SelectItem();
	void ResizeWindow();
	void SetDgnCode(int nDgnCode);
	int  GetDgnCode();
	void SubDlgCtrlEnableDisable();

private:
	void InitCtrl();
	void AlignControl();
	void EnableDisableControls();
	void ShowHideControls();
	BOOL Dlg2Data(T_CPFP_TYPE* pTfType);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();	

private:
	void ControlsEnableDisableSubDlgI(int nCode);
	void ControlsEnableDisableSubDlgJ(int nCode);
	void ResizeWindowSubDlgI(int nCode);
	void ResizeWindowSubDlgJ(int nCode);

// Implementation
protected:
	CDBDoc* m_pDoc;
	CCPGFatigueParamTabIDlg* m_pSubDlg_I;
	CCPGFatigueParamTabJDlg* m_pSubDlg_J;
	CCPGFatigueParamTabIDlg_CSA* m_pSubDlg_I_CSA;
	CCPGFatigueParamTabJDlg_CSA* m_pSubDlg_J_CSA;
	CCPGFatigueParamTabIDlg_AASHTO19* m_pSubDlg_I_AASHTO19;
	CCPGFatigueParamTabIDlg_AASHTO19* m_pSubDlg_J_AASHTO19;

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

#endif // !defined(AFX_CPGFATIGUEPARAMDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
