#if !defined(__TM_STLD_WIND_AREA_KBC2016_FRCCOEFAUTO_H__)
#define __TM_STLD_WIND_AREA_KBC2016_FRCCOEFAUTO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto : public CDialogMove
{
	// Construction
public:
	CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto(CWnd* pParent = NULL, int nInitStructType=-1, T_WVEP_K WvepK=NULL);   // standard constructor
	
	enum { IDD = IDD_TM_STLD_WINDP_DLG_AREA_KBC2016_FRCCOEFAUTO };

	int m_nInitStructType;
	T_WVEP_K m_WvepK;

	MComboBox	m_cmbStructureType;
	CString     m_strTargerLevel;
	CListCtrl	m_lstStory;

	// chimneys, Tanks, and similar structures
	int			m_rdoHorCrosSectType;
	CEditUnit	m_edtChimneysTSSDiaHc;
	CTextUnit	m_unitChimneysTSSDiaHc;
	CEditUnit	m_edtChimneysTSSDiaXDir;
	CTextUnit	m_unitChimneysTSSDia;
	CEditUnit	m_edtChimneysTSSDepthXDir;
	CTextUnit	m_unitChimneysTSSDepth;

	CEditUnit   m_edtChimneysHeight;
	CTextUnit   m_untChimneysHeight;
	CEditUnit   m_edtChimneysArea;
	CTextUnit   m_untChimneysArea;
	// open signs and lattice framework
	int			m_rdoSectMemType;
	CEditUnit	m_edtOpenSignsLFRatioXDir;
	CEditUnit	m_edtOpenSignsLFDiaXDir;
	CTextUnit	m_unitOpenSignsLFDia;

	// circle plan
	MComboBox	m_cmbRoughness;
	CEditUnit	m_edtDiaOfCicularPlan;
	CTextUnit	m_unitDiaOfCicularPlan;
	CEditUnit	m_edtHeightOfCicularPlan;
	CTextUnit	m_unitHeightOfCicularPlan;

	CEditUnit	m_edtCircleTSSDiaHc;
	CTextUnit	m_unitCircleTSSDiaHc;
	CEditUnit	m_edtCircleTSSDiaXDir;
	CTextUnit	m_unitCircleTSSDia;
	CEditUnit	m_edtCircleTSSDepthXDir;
	CTextUnit	m_unitCircleTSSDepth;

	// lattice towers
	int			m_rdoLTowersPlanType;
	int			m_rdoLTowersMembType;
	int			m_rdoLTowersWindDir;
	CEditUnit	m_edtLTowersRatioXDir;

	// fences on ground
	CEditUnit	m_edtFenceGrndRatioXDir;

	// Solid freestanding walls/signs
	CEditUnit	m_edtSolidFree_B;	// Horizontal dimension of wall or sign (B)
	CEditUnit	m_edtSolidFree_H;	// Height of the wall or sign (h)
	CEditUnit	m_edtSolidFree_S;	// Vertical dimension of wall or sign (s)
	CEditUnit	m_edtSolidFree_R;	// Ratio of solid area to gross area

	CTextUnit	m_untSolidFree_B;
	CTextUnit	m_untSolidFree_H;
	CTextUnit	m_untSolidFree_S;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:
	static const int COLCOUNT;
	int			m_nExposureCategory;
	double  m_dTotalHeight;
	T_WDPR_D m_Data;
	T_WDPR_KBC_AUTOCALC m_DataAutoCalc;
	CMap<int, int, int, int&> m_mapIndex;

public:
	void SetData(T_WDPR_D* pData);
	void GetData(T_WDPR_D* pData);

private:
	CDBDoc*  m_pDoc;
	//
	CArray<UINT, UINT> m_aChimneysTSS;
	CArray<UINT, UINT> m_aOpenSignsLF;
	CArray<UINT, UINT> m_aCirclePlan;
	CArray<UINT, UINT> m_aLatticeTowers;
	CArray<UINT, UINT> m_aFencesOnGrnd;
	CArray<UINT, UINT> m_aLTowersPlanRdo;
	CArray<UINT, UINT> m_aHorCrsSectTypeShowHideCtrl;
	CArray<UINT, UINT> m_aSectMemsTypeShowHideCtrl;
	CArray<UINT, UINT> m_aSolidFree;

	CArray<UINT, UINT> m_aCirclePlan_ASCE7;
	CArray<UINT, UINT> m_aCirclePlan_KBC;

	CArray<UINT, UINT> m_aKDS2021_Octagon;
	CArray<UINT, UINT> m_aKDS2021_HorCrsSectOther;
	CArray<UINT, UINT> m_aOctagonParam;

private:
	void AlignControl();
	void SetDlgByCombo();
	void SetLstHeaderTitle();
	void MakeLstItem();
	void InsertItem(const T_WDPR_KBC_AUTOCALC& WdprKBC, const int& nIndex);
	void ShowCtrlByHorCrsSectType();
	void ShowCtrlBySectMemsType();
	void EnableCtrlByPlanType();

protected:
	void InitCtrls();
	void Data2Dlg(const T_WDPR_KBC_AUTOCALC& WdprKBC);
	BOOL Dlg2Data(T_WDPR_KBC_AUTOCALC& WdprKBC);

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnSelchangeTypeCombo();
	afx_msg void OnSelectAll();
	afx_msg void OnTgtStorAddReplace();
	afx_msg void OnSelStorDelete();
	afx_msg void OnCmdGenerate();
	afx_msg void OnSelchangeStoryListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStoryListKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdoClikedChimneysTSS();
	afx_msg void OnRdoClikedOpenSignsLF();
	afx_msg void OnRdoClikedLTowersPlan();

	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

#endif
