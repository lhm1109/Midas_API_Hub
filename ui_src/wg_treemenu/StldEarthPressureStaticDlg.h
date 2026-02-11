#if !defined(__TM_STLD_EARTH_STATIC_DLG_H__)
#define __TM_STLD_EARTH_STATIC_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_CobxWvep.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_CobxLdgr.h"
#include "..\wg_db\wg_db_CobxLagr.h"
#include "..\wg_db\CobxPosp.h"
#include "..\wg_db\wg_db_SelectCtrl.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CStldEarthPressureStaticDlg dialog

class CStldEarthPressureStaticDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CStldEarthPressureStaticDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStldEarthPressureStaticDlg();

	enum { IDD = IDD_TM_STLD_EARTHP_DLG_STATIC };

	virtual void  GetMoveCtrlHolderID(CArray<UINT, UINT>& caCtrls, UINT& uiHolderID);
	virtual BOOL  GetHolderRect(CRect* pRect);

protected:
	T_EPST_D  m_Data;
	T_LAGR_K  m_LagrK;

	CSelectLC   m_cobxLoadCase;
	MComboBox   m_cobxDirection;
	CCobxLagr   m_cobxLagr;
	CCobxPosp   m_cobxSoilProp;
	CSelectEdit m_edElemList;
	CEditUnit   m_edtAngle;
	CEditUnit   m_edtScaleFactor;
	CMouseEdit	m_wndCoordXYZ;
	CTextUnit   m_wndCoordUnit;
	CEditUnit   m_edtSurchargeLoad;
	CTextUnit   m_SurchargeLoadUnit;
	CEditUnit   m_edtWaterLevel;
	CTextUnit   m_WaterLevelUnit;

	int m_nSelectionType;
	int m_nEarthPressureType;

	double m_dScaleFactor;
	double m_dSurchargeLoad;
	double m_dWaterLevel;
	T_POSP_K m_PospK;
	BOOL m_bInitProfile;

	CArray<UINT, UINT> m_aCtrlOption;
	CArray<UINT, UINT> m_aCtrlAddDelete;

protected:
	void InitCobxDirection();
	int  GetSelectionType();
	int  GetDirection();
	void OnUnitChange();
	int  GetElementType();

	virtual void Execute() { OnTmExecute(); };

	BOOL CheckEpst(CArray<T_EPST_D, T_EPST_D&>& rDataSet, BOOL bMsg);
	BOOL GetEpstAry(CArray<T_EPST_D, T_EPST_D&>& rDataSet);

	virtual BOOL Dlg2Data(BOOL bWarning = TRUE);
	virtual BOOL Data2Dlg(BOOL bWarning = TRUE);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void UpdateBuffer();

	afx_msg void OnDeltaposCmdAngle(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnCmdBtnLoadGroup();
	afx_msg void OnCmdBtnLoadAreaGroup();
	afx_msg void OnCmdBtnLoadCase();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnCmdTargetChanged();
	afx_msg void OnCmdPressureProfile();
	afx_msg void OnSelChangeLagr();
	afx_msg void OnSelChangeDir();
	afx_msg void OnCmdSoilPropBtn();
	afx_msg void OnCmdOptionRdo();
	DECLARE_MESSAGE_MAP()
};
#endif
