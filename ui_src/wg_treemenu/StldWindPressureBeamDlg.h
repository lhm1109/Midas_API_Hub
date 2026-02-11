#if !defined(__TM_STLD_WIND_BEAM_DLG_H__)
#define __TM_STLD_WIND_BEAM_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindPressureDlgArea.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_cmd\CMWindPressureBaseDlg.h"

#include "..\wg_db\wg_db_CobxWvep.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_CobxLdgr.h"
#include "..\wg_db\wg_db_CobxLagr.h"
#include "..\wg_db\wg_db_SelectCtrl.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "wg_treemenures.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureBeamDlg dialog

class CCMWindPressureRootDlg;

class CStldWindPressureBeamDlg : public CCMWindPressureBaseDlg, public CDBUpdateConnector
{
// Construction
public:
	CStldWindPressureBeamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStldWindPressureBeamDlg();

	enum { IDD = IDD_TM_STLD_WINDP_DLG_BEAM };

	virtual void  GetMoveCtrlHolderID( CArray<UINT, UINT>& caCtrls, UINT& uiHolderID);
	virtual BOOL  GetHolderRect( CRect* pRect );
	virtual CWnd* GetBaseDlg() { return (CWnd*)m_pRootDlg; }

	virtual BOOL GetWvepKey(T_WVEP_K& WvepK) { return m_cobxWvep.GetSelectedWvep(WvepK); }

	virtual void Execute() override;

protected:
	T_WDPR_D  m_Data;
	T_LAGR_K  m_LagrK;
	CBCGPStatic	  m_wndHolder;
	
	//CCobxLdgr   m_cobxLdgr;
	CSelectLC   m_cobxLoadCase;
	MComboBox   m_cobxDirection;
	MComboBox	  m_cobxCodeName;
	CCobxWvep   m_cobxWvep;
	CEditUnit   m_edtAngle;
	CEditUnit   m_edtScaleFactor;
	CArray<T_ELEM_K, T_ELEM_K> m_arElemK;
	CCMWindPressureRootDlg*    m_pRootDlg;
	
protected:
	void InitCobxCodeName();
	void InitCobxDirection();
	int  GetDirection();
	void OnUnitChange();
	void ChangeCodeDlg();

	BOOL CheckWdpr(CArray<T_WDPR_D, T_WDPR_D&>& rDataSet, BOOL bMsg);
	BOOL GetWdprAry(CArray<T_WDPR_D, T_WDPR_D&>& rDataSet, BOOL bProfile);

	virtual BOOL Dlg2Data(BOOL bWarning = TRUE);
	virtual BOOL Data2Dlg(BOOL bWarning = TRUE);  
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	
	afx_msg void OnDeltaposCmdAngle(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdBtnLoadAreaGroup();
	afx_msg void OnCmdBtnLoadCase();
	afx_msg void OnChangeAngle();
	afx_msg void OnSelchangeCmdCodeName();
	afx_msg void OnVelocityBtn();
	afx_msg void OnCmdPressureProfile();
	afx_msg void OnSelChangeDir();
	afx_msg void OnSelChangeVelocity();

	DECLARE_MESSAGE_MAP()
};

#endif
