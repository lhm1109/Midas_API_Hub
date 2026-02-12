#if !defined(__TM_STLD_WIND_AREA_KBC2009_FRCCOEFAUTO_H__)
#define __TM_STLD_WIND_AREA_KBC2009_FRCCOEFAUTO_H__

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
// CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto : public CDialogMove
{
	// Construction
public:
	CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto(CWnd* pParent = NULL);   // standard constructor
	
	enum { IDD = IDD_TM_STLD_WINDP_DLG_AREA_KBC2009_FRCCOEFAUTO };

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

	// solid signs
	int			m_rdoInstallType;
	CEditUnit	m_edtSolSignsHeightXDir;
	CTextUnit	m_unitSolSignsHeight;
	CEditUnit	m_edtSolSignsBreathXDir;
	CTextUnit	m_unitSolSignsBreath;
	CString		m_txtSolSignsHeight;
	CString		m_txtSolSignsBreath;

	// open signs and lattice framework
	int			m_rdoSectMemType;
	CEditUnit	m_edtOpenSignsLFRatioXDir;
	CEditUnit	m_edtOpenSignsLFDiaXDir;
	CTextUnit	m_unitOpenSignsLFDia;

	//  truss towers
	int			m_rdoPlanType;
	int			m_rdoMemType;
	CEditUnit	m_edtTrusTowersRatioXDir;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:
	static const int COLCOUNT;
	double   m_dTotalHeight;
	T_WDPR_D m_Data;
	T_WDPR_KBC_AUTOCALC m_DataAutoCalc;

public:
	void SetData(T_WDPR_D* pData);
	void GetData(T_WDPR_D* pData);

private:
	CDBDoc*  m_pDoc;
	//
	CArray<UINT, UINT> m_aChimneysTSS;
	CArray<UINT, UINT> m_aSolSigns;
	CArray<UINT, UINT> m_aOpenSignsLF;
	CArray<UINT, UINT> m_aTrusTowers;
	CArray<UINT, UINT> m_aHorCrsSectTypeShowHideCtrl;
	CArray<UINT, UINT> m_aSectMemsTypeShowHideCtrl;

private:
	void AlignControl();
	void SetDlgByCombo();
	void SetLstHeaderTitle();
	void MakeLstItem();
	void InsertItem(const T_WDPR_KBC_AUTOCALC& WdprKBC2009, const int& nIndex);
	void ShowCtrlByHorCrsSectType();
	void ShowCtrlByInstallType();
	void ShowCtrlBySectMemsType();
	void MakeTgtEditStory();

protected:
	void InitCtrls();
	void Data2Dlg(const T_WDPR_KBC_AUTOCALC& WdprKBC2009);
	BOOL Dlg2Data(T_WDPR_KBC_AUTOCALC& WdprKBC2009);

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
	afx_msg void OnRdoClikedSolSigns();
	afx_msg void OnRdoClikedOpenSignsLF();

	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

#endif
