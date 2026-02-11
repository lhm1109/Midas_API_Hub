#if !defined(__SEIS_CVL_JP_SET_INVEST_SIRD_DLG_H__)
#define __SEIS_CVL_JP_SET_INVEST_SIRD_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSeisCvlJpSetInvestSirdDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CDBDoc;
class CSeisCvlJpSetInvestMethodTimeGrid;

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestSirdDlg dialog

class CSeisCvlJpSetInvestSirdDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
private:
	int			m_nOption;
	MComboBox	m_caseName;
	MComboBox	m_groupName;
	MComboBox	m_direction;
	MEdit		m_angle;
	MComboBox	m_calcType;
	MComboBox	m_this_plus;
	MComboBox	m_groupName_plus;
	MComboBox	m_colmGroup_plus;
	MComboBox	m_stepDeltaY_plus;
	MEdit		m_length_plus;
	MComboBox	m_this_minus;
	MComboBox	m_groupName_minus;
	MComboBox	m_colmGroup_minus;
	MComboBox	m_stepDeltaY_minus;
	MEdit		m_length_minus;
	MComboBox	m_calcMethod;
	BOOL m_bMethodTime;
	CSeisCvlJpSetInvestMethodTimeGrid* m_pGrid;

	CDBDoc* m_pDoc;

	// Construction
public:
	CSeisCvlJpSetInvestSirdDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSeisCvlJpSetInvestSirdDlg();

	// Dialog Data
		//{{AFX_DATA(CSeisCvlJpSetInvestSirdDlg)
	enum { IDD = IDD_TM_SIRD };

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCvlJpSetInvestSirdDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSeisCvlJpSetInvestSirdDlg)
	afx_msg void OnChangedCaseCmb();
	afx_msg void OnChangedGroupCmb();
	afx_msg void OnChangedDirectionCmb();
	afx_msg void OnChangedCalcTypeCmb();
	afx_msg void OnCalcAutoBtn();
	afx_msg void OnChangedCasePulsCmb();
	afx_msg void OnChangedCaseMinusCmb();
	afx_msg void OnChangedGroupPulsCmb();
	afx_msg void OnChangedGroupMinusCmb();
	afx_msg void OnChangedColumnPulsCmb();
	afx_msg void OnClickMethodTime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual void Execute();

private:
	void InitGrid();
	void SetGrid();
	void GetGird(OUT T_SIRD_D& rData);
	void InitControl();
	void InitCaseNameCtrl();
	void InitGroupNameCtrl();
	void InitDirectionCtrl();
	void InitCalcMethodCtrl();
	void UpdateDirectionCmb();
	void UpdateAngleCtrl();
	void UpdateCalcTypeCtrl();
	void InitAnalysisCaseCtrl(MComboBox& rComboBox);
	void InitGroupLowNameCtrl(MComboBox& rComboBox);
	void UpdateGroupLowNameCtrl(MComboBox& rComboBox, BOOL bReadOnly = FALSE);
	void UpdateAnalysisCaseCtrl();
	void UpdateCalcAutoBtn();
	void InitColmGroupCtrl(const MComboBox& groupNameCmb, MComboBox& rComboBox);
	void UpdateColmGroupGroupCtrl();
	void UpdateStepDeltaYCtrl();
	void InitDeltaY_LengthCtrl();
	void UpdateDeltaY_LengthCtrl();
	void UpdateLoadCaseStepCtrl(const MComboBox& loadCase, MComboBox& rComboBox);
	void UddateValueByCalcTypeAuto(const UINT/*MPhiProp::eDir*/& dir = 2/*MPhiProp::kDirMax*/);
	void UddateControlByYZDir();
	BOOL UddateControlByCalcTypeAuto(const UINT/*QIehp::eCom*/& comp,
									 const UINT/*MPhiProp::eDir*/& dir,
									 const MComboBox& thisCmb,
									 OUT MComboBox& groupNameCmb,
									 OUT MComboBox& colmGroupCmb,
									 OUT MComboBox& stepDeltaYCmb);

	//Execute() 관련함수
	BOOL AddOrModData();
	BOOL DelData();
	BOOL DlgToData(OUT T_SIRD_D& rData);
	BOOL CheckData(const T_SIRD_D& data);
	T_SIRD_K GetExistingDataKey(const T_SIRD_D& newData);
	T_THCR_K GetThcrKey(const CString& str);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEIS_CVL_JP_SET_INVEST_SIRD_DLG_H__)