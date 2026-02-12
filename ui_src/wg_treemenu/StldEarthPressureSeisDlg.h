#if !defined(__TM_STLD_EARTH_SEIS_DLG_H__)
#define __TM_STLD_EARTH_SEIS_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_cmd\CMEarthPressureBaseDlg.h"

#include "..\wg_db\wg_db_CobxWvep.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_CobxLdgr.h"
#include "..\wg_db\wg_db_CobxLagr.h"
#include "..\wg_db\CobxPosp.h"
#include "..\wg_db\CobxPosl.h"
#include "..\wg_db\wg_db_SelectCtrl.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CStldEarthPressureStaticDlg dialog

class CCMEarthPressureRootDlg;

class CStldEarthPressureSeisDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CStldEarthPressureSeisDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStldEarthPressureSeisDlg();

	enum { IDD = IDD_TM_STLD_EARTHP_DLG_SEIS };

	virtual void  GetMoveCtrlHolderID(CArray<UINT, UINT>& caCtrls, UINT& uiHolderID);
	virtual BOOL  GetHolderRect(CRect* pRect);

	virtual BOOL GetWvepKey(T_WVEP_K& WvepK) { return FALSE; }

protected:
	T_EPSE_D  m_Data;
	T_LAGR_K  m_LagrK;

	CSelectLC   m_cobxLoadCase;
	MComboBox   m_cobxLodingType;
	MComboBox   m_cobxDirection;
	MComboBox	m_cobxCodeName;
	CCobxLagr   m_cobxLagr;
	CCobxPosl   m_cobxSeisPsol;
	CCobxPosp   m_cobxSoilProp;
	CSelectEdit m_edElemList;
	CFormulaEditSpin   m_edtAngle;
	CEditUnit   m_edtScaleFactor;
	CMouseEdit	m_wndCoordXYZ;
	CTextUnit   m_wndCoordUnit;
	CEditUnit   m_edt2ndLayer;
	CTextUnit   m_2ndLayerUnit;
	CEditUnit   m_edtWidth;
	CTextUnit   m_untWidth;

	BOOL m_bCivil;

	int m_nEPSEMethod;
	int m_nSelectionType;
	BOOL m_bInitProfile;

	CArray<UINT, UINT> m_aCtrlOption;
	CArray<UINT, UINT> m_aCtrlEPSLMethod;
	CArray<UINT, UINT> m_aCtrlEPSLMethodRdo;
	CArray<UINT, UINT> m_aCtrlLayerMethod;
	CArray<UINT, UINT> m_aCtrlLayerParam;
	CArray<UINT, UINT> m_aCtrlDoubleCosine;
	CArray<UINT, UINT> m_aCtrlAddDelete;
	CArray<UINT, UINT> m_aCtrlLoadingType;
	CArray<UINT, UINT> m_aCtrlLoadingGrHide;
	CArray<UINT, UINT> m_aCtrlLoadingGrMove;
	CArray<UINT, UINT> m_aLastCtrlData;
protected:
	T_POSP_K m_OldPospKey;
	T_POSL_K m_OldPoslKey;
	int m_nOldLayerType;
	double m_dOldLayerLevel;
    double m_dOldScaleFactor;
protected:
	void InitCobxLoadingType();
	void InitCobxDirection(BOOL bFrame = FALSE);
	void InitCobxCodeName();
	int  GetSelectionType();
	int  GetDirection();
	void OnUnitChange();
	int  GetElementType();
	BOOL CheckEpse(CArray<T_EPSE_D, T_EPSE_D&>& rDataSet, BOOL bMsg);
	BOOL GetEpseAry(CArray<T_EPSE_D, T_EPSE_D&>& rDataSet);
	void GetMemberData(const T_KEY_LIST& aElemK, CArray<T_MEMB_D, T_MEMB_D&>& aDataMemb);
	void GetNodeListFromBottom(const CArray<T_ELEM_K, T_ELEM_K>& aElem, CArray<T_NODE_K, T_NODE_K>& aNode);
	void AlignControl();
	void SelChangeDir();

	virtual BOOL Dlg2Data(BOOL bWarning = TRUE);
	virtual BOOL Data2Dlg(BOOL bWarning = TRUE);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void Execute() { OnTmExecute(); };

	void UpdateBuffer();
	afx_msg void OnDeltaposCmdAngle(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnCmdBtnLoadGroup();
	afx_msg void OnCmdBtnLoadAreaGroup();
	afx_msg void OnCmdBtnLoadCase();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnCmdTargetChanged();
	afx_msg void OnSelChangeLagr();
	afx_msg void OnSelChangePOSL();
	afx_msg void OnSelChangeLoadingType();
	afx_msg void OnSelChangeDir();
	afx_msg void OnCmdSoilPropBtn();
	afx_msg void OnCmdOptionRdo();
	afx_msg void OnCmdLayerParamRdo();
	afx_msg void OnCmdSeismicLoadBtn();
	afx_msg void OnCmdPressureProfile();
	DECLARE_MESSAGE_MAP()
};
#endif // __TM_STLD_EARTH_SEIS_DLG_H__