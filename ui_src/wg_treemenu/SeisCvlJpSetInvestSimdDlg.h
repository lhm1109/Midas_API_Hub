#if !defined(__SEIS_CVL_JP_SET_INVEST_SIMD_DLG_H__)
#define __SEIS_CVL_JP_SET_INVEST_SIMD_DLG_H__

#define COMBO_CALC_TYPE_FROM_RESULT		_LS(IDS_IDD_TM_SIMD_IDC_TM_SIMD_CALC_TYPE_CMB_FROM_RESULT)
#define COMBO_CALC_TYPE_FROM_MPHI_Dy	_LS(IDS_IDD_TM_SIMD_IDC_TM_SIMD_CALC_TYPE_CMB_FROM_MPhi_Dy)
#define COMBO_CALC_TYPE_FROM_MPHI_Dpy	_LS(IDS_IDD_TM_SIMD_IDC_TM_SIMD_CALC_TYPE_CMB_FROM_MPhi_Dpy)
#define COMBO_CALC_TYPE_INPUT_VALUE		_LS(IDS_IDD_TM_SIMD_IDC_TM_SIMD_CALC_TYPE_CMB_INPUT_VALUE)

#define COMBO_DIRECTION_SEISMIC			_LS(IDS_IDD_TM_SIMD_IDC_TM_SIMD_DIR_SEISMIC)
#define COMBO_DIRECTION_ELEM_COORDINATE	_LS(IDS_IDD_TM_SIMD_IDC_TM_SIMD_DIR_ELEM_COORDINATE)
#define COMBO_DIRECTION_USER_DEFINE		_LS(IDS_IDD_TM_SIMD_IDC_TM_SIMD_DIR_USER_DEFINE)

#define COMBO_DEFAULT _T("")

#define COMBO_CALC_METHOD_NEXCO			_LS(IDS_IDC_TM_SIMD_CALC_METHOD_NEXCO)
#define COMBO_CALC_METHOD_JBEC					_LS(IDS_IDC_TM_SIMD_CALC_METHOD_JBEC)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSeisCvlJpSetInvestSimdDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CDBDoc;
class CSeisCvlJpSetInvestMethodTimeGrid;
/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestSimdDlg dialog

class CSeisCvlJpSetInvestSimdDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
	friend class CSeisCvlJpSetInvestSirdDlg; //static 함수 사용용도

private:
	int			m_nDesignCode;
	int			m_nOption;
	int			m_nGroupType;		//그룹 종류 enum T_SIGR_D::eSubType
	int			m_nMatlType_plus;	//교각의 재료 타입 : enum T_SIGR_LOWER_D::eMaterialType
	int			m_nMatlType_minus;	//교각의 재료 타입 : enum T_SIGR_LOWER_D::eMaterialType
	MComboBox	m_caseName;
	MComboBox	m_groupName;
	MComboBox	m_direction;
	MEdit		m_angle;
	MComboBox	m_calcType;
	MComboBox	m_this_plus;
	MComboBox	m_groupName_plus;
	MComboBox	m_colmGroup_plus;
	MComboBox	m_stepDeltaY_plus;
	MComboBox	m_stepDeltaLs_plus;
	MEdit		m_lengthDeltaA_plus;
	MComboBox	m_this_minus;
	MComboBox	m_groupName_minus;
	MComboBox	m_colmGroup_minus;
	MComboBox	m_stepDeltaY_minus;
	MComboBox	m_stepDeltaLs_minus;
	MEdit		m_lengthDeltaA_minus;
	MEdit		m_alpha;
	BOOL		m_considerAlpha;
	MComboBox	m_calcMethod;
	BOOL m_bMethodTime;
	CSeisCvlJpSetInvestMethodTimeGrid* m_pGrid;

	CDBDoc* m_pDoc;

	// Construction
public:
	CSeisCvlJpSetInvestSimdDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSeisCvlJpSetInvestSimdDlg();

	// Dialog Data
		//{{AFX_DATA(CSeisCvlJpSetInvestSimdDlg)
	enum { IDD = IDD_TM_SIMD };

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCvlJpSetInvestSimdDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void OnInitDialog_H14();
	void OnInitDialog_H24();
	void OnInitDialog_H29();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSeisCvlJpSetInvestSimdDlg)
	afx_msg void OnChangedCaseCmb();
	afx_msg void OnChangedGroupCmb();
	afx_msg void OnChangedDirectionCmb();
	afx_msg void OnChangedCalcTypeCmb();
	afx_msg void OnCalcAutoBtn();
	afx_msg void OnChangedCasePulsCmb();
	void OnChangedCasePulsCmb_H14();
	void OnChangedCasePulsCmb_H24();
	void OnChangedCasePulsCmb_H29();
	afx_msg void OnChangedCaseMinusCmb();
	void OnChangedCaseMinusCmb_H14();
	void OnChangedCaseMinusCmb_H24();
	void OnChangedCaseMinusCmb_H29();
	afx_msg void OnChangedGroupPulsCmb();
	afx_msg void OnChangedGroupMinusCmb();
	afx_msg void OnChangedColumnPulsCmb();
	afx_msg void OnCheckedConsiderAlphaChk();
	afx_msg void OnClickMethodTime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual void Execute();

private:
	void InitGrid();
	void SetGrid();
	void GetGird(OUT T_SIMD_D& rData);
	void InitControl();
	void InitCaseNameCtrl();
	void InitGroupNameCtrl();
	void InitDirectionCtrl();
	void InitCalcMethodCtrl();
	void UpdateDirectionCmb();
	void UpdateAngleCtrl();
	void UpdateCalcTypeCtrl();
	void InitAnalysisCaseCtrl(MComboBox& rComboBox);
	void InitGroupLowNameCtrl(MComboBox& rComboBox, OUT int& rMatlType);
	void UpdateAnalysisCaseCtrl();
	void UpdateCalcAutoBtn();
	void InitColmGroupCtrl(const MComboBox& groupNameCmb, MComboBox& rComboBox);
	void InitColmGroupCtrl_H14(const MComboBox& groupNameCmb, MComboBox& rComboBox);
	void InitColmGroupCtrl_H24(const MComboBox& groupNameCmb, MComboBox& rComboBox);
	void InitColmGroupCtrl_H29(const MComboBox& groupNameCmb, MComboBox& rComboBox);
	void UpdateGroupLowNameCtrl(MComboBox& rComboBox, BOOL bReadOnly = FALSE);
	void UpdateGroupLowNameCtrl_H14(MComboBox& rComboBox, BOOL bReadOnly = FALSE);
	void UpdateGroupLowNameCtrl_H24(MComboBox& rComboBox, BOOL bReadOnly = FALSE);
	void UpdateGroupLowNameCtrl_H29(MComboBox& rComboBox, BOOL bReadOnly = FALSE);
	void UpdateColmGroupGroupCtrl();
	void UpdateColmGroupGroupCtrl_H14();
	void UpdateColmGroupGroupCtrl_H24();
	void UpdateColmGroupGroupCtrl_H29();
	void UpdateStepDeltaYCtrl();
	void UpdateStepDeltaYCtrl_H14();
	void UpdateStepDeltaYCtrl_H24();
	void UpdateStepDeltaYCtrl_H29();
	void InitStepDeltaLsCtrl();
	void InitStepDeltaLsCtrl_H14();
	void InitStepDeltaLsCtrl_H24();
	void InitStepDeltaLsCtrl_H29();
	void UpdateStepDeltaLsCtrl();
	void InitStepDeltaACtrl();
	void InitStepDeltaACtrl_H14();
	void InitStepDeltaACtrl_H24();
	void InitStepDeltaACtrl_H29();
	void UpdateStepDeltaACtrl();
	void UpdateLoadCaseStepCtrl(const MComboBox& loadCase, MComboBox& rComboBox);
	void UpdateAlphaCtrl();
	void GetBearingArray(CDBDoc* pDoc, const CString& strGroup, CArray<T_SIGR_LOWER_BEAR_D, T_SIGR_LOWER_BEAR_D&>& bearArray);
	void UpdateAlphaCtrl_H14();
	void UpdateAlphaCtrl_H24();
	void UpdateAlphaCtrl_H29();
	void UpdateConsiderAlpha();
	void UpdateConsiderAlpha_H14();
	void UpdateConsiderAlpha_H24();
	void UpdateConsiderAlpha_H29();
	void UddateValueByCalcTypeAuto(const UINT/*MPhiProp::eDir*/& dir = 2/*MPhiProp::kDirMax*/);
	static void GetComponent(const MComboBox& caseNameCmb, const MComboBox& groupNameCmb, OUT UINT& rComp1);
	static BOOL GetComponentByColumn(	const UINT/*T_SIPA_D::eSeismicForceDirection*/& nSeismicForceDir,
										const T_SIGR_LOWER_K& kSigrLower, 
										OUT UINT/*QIehp::eCom*/& rComponent);
	void UddateControlByYZDir();
	BOOL UddateControlByCalcTypeAuto(const UINT/*QIehp::eCom*/& comp,
									 const UINT/*MPhiProp::eDir*/& dir,
									 const MComboBox& thisCmb,
									 OUT MComboBox& groupNameCmb,
									 OUT MComboBox& colmGroupCmb,
									 OUT MComboBox& stepDeltaYCmb,
									 OUT MComboBox& stepDeltaLsCmb);

	BOOL UddateControlByCalcTypeAuto_H14(const UINT/*QIehp::eCom*/& comp,
										const UINT/*MPhiProp::eDir*/& dir,
										const MComboBox& thisCmb,
										OUT MComboBox& groupNameCmb,
										OUT MComboBox& colmGroupCmb,
										OUT MComboBox& stepDeltaYCmb,
										OUT MComboBox& stepDeltaLsCmb);

	BOOL UddateControlByCalcTypeAuto_H24(const UINT/*QIehp::eCom*/& comp,
										const UINT/*MPhiProp::eDir*/& dir,
										const MComboBox& thisCmb,
										OUT MComboBox& groupNameCmb,
										OUT MComboBox& colmGroupCmb,
										OUT MComboBox& stepDeltaYCmb,
										OUT MComboBox& stepDeltaLsCmb);

	BOOL UddateControlByCalcTypeAuto_H29(const UINT/*QIehp::eCom*/& comp,
										const UINT/*MPhiProp::eDir*/& dir,
										const MComboBox& thisCmb,
										OUT MComboBox& groupNameCmb,
										OUT MComboBox& colmGroupCmb,
										OUT MComboBox& stepDeltaYCmb,
										OUT MComboBox& stepDeltaLsCmb);

	//Execute() 관련함수
	BOOL AddOrModData();
	BOOL DelData();
	BOOL DlgToData(OUT T_SIMD_D& rData);
	BOOL DlgToData_H14(OUT T_SIMD_D& rData);
	BOOL DlgToData_H24(OUT T_SIMD_D& rData);
	BOOL DlgToData_H29(OUT T_SIMD_D& rData);
	BOOL CheckData(const T_SIMD_D& data);
	T_SIMD_K GetExistingDataKey(const T_SIMD_D& newData);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEIS_CVL_JP_SET_INVEST_SIMD_DLG_H__)