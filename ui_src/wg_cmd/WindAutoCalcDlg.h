#if !defined(__WINDAUTOCALC_H__)
#define __WINDAUTOCALC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindAutoCalc.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MComboBox.h"
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CWindAutoCalcDlg dialog
#include "HeaderPre.h"

class CLinkedKeyMgr;
class __MY_EXT_CLASS__ CWindAutoCalcDlg : public CDialogMove
{
	// Construction
public:
	CWindAutoCalcDlg(CWnd* pParent = NULL, T_WIND_D& WindD = T_WIND_D(), int nInitStructType=-1);   // standard constructor
	virtual ~CWindAutoCalcDlg();
	
	// Dialog Data
	//{{AFX_DATA(CWindAutoCalcDlg)
	enum { IDD = IDD_ETC_WIND_AUTOCALC };
	int m_nInitStructType;
	MComboBox	m_cmbStructureType;
	CEditUnit	m_edtCfx;
	CEditUnit	m_edtCfy;
	CString m_strTargerStory;
	CListCtrl	m_lstStory;
	// chimneys, Tanks, and similar structures
	int			m_rdoHorCrosSectType;
	CEditUnit	m_edtChimneysTSSDiaXDir;
	CEditUnit	m_edtChimneysTSSDiaYDir;
	CTextUnit	m_unitChimneysTSSDia;
	CEditUnit	m_edtChimneysTSSDepthXDir;
	CEditUnit	m_edtChimneysTSSDepthYDir;
	CTextUnit	m_unitChimneysTSSDepth;

	CEditUnit   m_edtChimneysHeight;
	CTextUnit   m_untChimneysHeight;
	CEditUnit   m_edtChimneysArea;
	CTextUnit   m_untChimneysArea;

	// solid signs
	int			m_rdoInstallType;
	CEditUnit	m_edtSolSignsHeightXDir;
	CEditUnit	m_edtSolSignsHeightYDir;
	CTextUnit	m_unitSolSignsHeight;
	CEditUnit	m_edtSolSignsBreathXDir;
	CEditUnit	m_edtSolSignsBreathYDir;
	CTextUnit	m_unitSolSignsBreath;
	CString		m_txtSolSignsHeight;
	CString		m_txtSolSignsBreath;
	// open signs and lattice framework
	int			m_rdoSectMemType;
	CEditUnit	m_edtOpenSignsLFRatioXDir;
	CEditUnit	m_edtOpenSignsLFRatioYDir;
	CEditUnit	m_edtOpenSignsLFDiaXDir;
	CEditUnit	m_edtOpenSignsLFDiaYDir;
	CTextUnit	m_unitOpenSignsLFDia;
	//  truss towers
	int			m_rdoPlanType;
	int			m_rdoMemType;
	CEditUnit	m_edtTrusTowersRatioXDir;
	CEditUnit	m_edtTrusTowersRatioYDir;
	// circle plan
	MComboBox	m_cmbRoughness;
	CEditUnit	m_edtDiaOfCicularPlan;
	CTextUnit	m_unitDiaOfCicularPlan;
	CEditUnit	m_edtHeightOfCicularPlan;
	CTextUnit	m_unitHeightOfCicularPlan;
	// solid walls and solid signs
	int			m_rdoSolidWLSignsType;
	CEditUnit	m_edtSolidWLSignsBreadthXDir;
	CEditUnit	m_edtSolidWLSignsBreadthYDir;
	CTextUnit	m_unitSolidWLSignsBreadth;
	CEditUnit	m_edtSolidWLSignsVerticalXDir;
	CEditUnit	m_edtSolidWLSignsVerticalYDir;
	CTextUnit	m_unitSolidWLSignsVertical;
	CEditUnit	m_edtSolidWLSignsHeightXDir;
	CEditUnit	m_edtSolidWLSignsHeightYDir;
	CTextUnit	m_unitSolidWLSignsHeight;
	CEditUnit	m_edtSolidWLSignsRatioXDir;
	CEditUnit	m_edtSolidWLSignsRatioYDir;
	// lattice towers
	int			m_rdoLTowersPlanType;
	int			m_rdoLTowersMembType;
	int			m_rdoLTowersWindDir;
	CEditUnit	m_edtLTowersRatioXDir;
	CEditUnit	m_edtLTowersRatioYDir;
	CEditUnit	m_edtFenceGrndRatioXDir;
	CEditUnit	m_edtFenceGrndRatioYDir;
	//}}AFX_DATA
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMElsPlsMatlAddDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
private:
	enum enmHorCrossSectType
	{
		ENM_T_SQUARE_NORMAL = 0,
		ENM_T_SQUARE_DIA,
		ENM_T_HEX_OCTA,
		ENM_T_ROUND,
		ENM_T_OCTAGON
	};
	enum enmInstallType
	{
		ENM_T_AT_GR_LEVEL = 0,
			ENM_T_ABOVE_GR_LEVEL
	};
	enum enmSectMemType
	{
		ENM_T_FLAT_SIDED_MEMS = 0,
			ENM_T_ROUNDED_MEMS
	};
	enum enmPlanType
	{
		ENM_T_PLAN_SQUARE = 0,
			ENM_T_PLAN_TRIANGLE
	};
	enum enmMemType
	{
		ENM_T_STEEL_RND_MEMS = 0,
			ENM_T_SQUARE_CRSSECT_MEMS,
			ENM_T_OTHER_TYPE
	};

	static const int COLCOUNT;

	// Implementation
private:
	T_WIND_D m_WindD;
	double m_dQz;
	CMap<T_STOR_K, T_STOR_K, T_WNAT_STOR, T_WNAT_STOR&> m_mapWnatStor;
	CMap<T_STOR_K, T_STOR_K, double, double> m_mapQZ;
	CArray<T_STOR_K, T_STOR_K> m_aTgtStorK;
	CLinkedKeyMgr* m_pLinkedKeyMgr_stor;
	double m_dBaseLevel;

public:
	void SetData(T_WNAT_D& WnatD);
	void GetData(T_WNAT_D& WnatD);

private:
	CDBDoc*  m_pDoc;
	//
	CArray<UINT, UINT> m_aChimneysTSS;
	CArray<UINT, UINT> m_aSolSigns;
	CArray<UINT, UINT> m_aOpenSignsLF;
	CArray<UINT, UINT> m_aTrusTowers;
	CArray<UINT, UINT> m_aHorCrsSectTypeShowHideCtrl;
	CArray<UINT, UINT> m_aSectMemsTypeShowHideCtrl;
	CArray<UINT, UINT> m_aCirclePlan;
	CArray<UINT, UINT> m_aSolWallSigns;
	CArray<UINT, UINT> m_aLatticeTowers;
	CArray<UINT, UINT> m_aFencesOnGrnd;
	CArray<UINT, UINT> m_aLTowersPlanRdo;

	CArray<UINT, UINT> m_aKDS2021_Octagon;
	CArray<UINT, UINT> m_aKDS2021_HorCrsSectOther;
	CArray<UINT, UINT> m_aOctagonParam;
private:
	void AlignControl();
	void SetDlgByComb();
	void SetLstHeaderTitle();
	void MakeLstItem();
	void InsertItem(const T_WNAT_STOR& WnatStorD);
	void ShowCtrlByHorCrsSectType();
	void ShowCtrlByInstallType();
	void ShowCtrlBySectMemsType();
	void EnableCtrlByPlanType();
	void MakeTgtEditStory();
	double GetUpperStorLevel(UINT StorK);

protected:
	void InitCtrls();
	void Data2Dlg(const T_WNAT_STOR& WnatStorD);
	void Dlg2Data(T_WNAT_STOR& WnatStorD);
	
	// Generated message map functions
	//{{AFX_MSG(CCMElsPlsMatlAddDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeTypeCombo();
	afx_msg void OnSelectStoryDlgCall();
	afx_msg void OnTgtStorAddReplace();
	afx_msg void OnSelStorDelete();
	afx_msg void OnSelchangeStoryListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStoryListKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdoClikedChimneysTSS();
	afx_msg void OnRdoClikedSolSigns();
	afx_msg void OnRdoClikedOpenSignsLF();
	afx_msg void OnRdoClikedLTowersPlan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__WINDAUTOCALC_H__)