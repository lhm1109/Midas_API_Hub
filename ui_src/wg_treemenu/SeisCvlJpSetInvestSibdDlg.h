#if !defined(__SEIS_CVL_JP_SET_INVEST_SIBD_DLG_H__)
#define __SEIS_CVL_JP_SET_INVEST_SIBD_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSeisCvlJpSetInvestSibdDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestSibdDlg dialog

class CSeisCvlJpSetInvestSibdDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
private:
	int			m_nOption;
	MComboBox	m_caseName;
	MComboBox	m_groupName;
	int			m_surveyType;		//조사 조건
	MComboBox	m_momentType;		//내력 조사 조건
	int			m_cvsv_surveyType;	//곡률조사 조건
	MEdit		m_surrenderAlpha;	//항복조사 일 때 허용값 α
	int			m_allowCurvFormula;	//RC부재의 허용곡률 계산식
	MEdit		m_limitStateAlpha;	//한계상태조사 일 때 허용값 α

	CDBDoc* m_pDoc;

	CArray<UINT, UINT> m_aStaticCtrl;

	// Construction
public:
	CSeisCvlJpSetInvestSibdDlg(CWnd* pParent = NULL);   // standard constructor
	~CSeisCvlJpSetInvestSibdDlg();

	// Dialog Data
		//{{AFX_DATA(CSeisCvlJpSetInvestSibdDlg)
	enum { IDD = IDD_TM_SIBD };

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCvlJpSetInvestSibdDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSeisCvlJpSetInvestSibdDlg)
	afx_msg void OnChangedCaseCmb();
	afx_msg void OnChangedGroupCmb();
	afx_msg void OnChangedSurveyTypeRdo();
	afx_msg void OnChangedCvsvSurveyTypeRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual void Execute();

private:
	void MoveStaticCtrl();
	void InitControl();
	void InitCaseNameCtrl();
	void InitGroupNameCtrl();
	void UpdateAllButtonCtrl();
	void UpdateSurveyTypeCtrl();
	void UpdateCvsvSurveyType();
	void UpdateRcMemeberCtrl();
	void UpdateRcMemeberCtrl_H29(const T_SIGR_LOWER_D& sigrLowerD);
	void UpdateRcMemeberCtrl_H14_H24(const T_SIGR_LOWER_D& sigrLowerD);
	void SetEnableRcMemeberCtrl(const BOOL& bEnable);
	void SetEnableRcMemeberRadioBtn(const BOOL& bEnable);
	void UpdateRcMemeberRadioBtn(const T_SIDC_D& sidcD);
	void UpdateLimitStateAlphaEdt_H14(const T_SIGR_LOWER_D& groupD);
	void UpdateLimitStateAlphaEdt_H24(const T_SIGR_LOWER_D& groupD);
	void UpdateLimitStateAlphaEdt_H29(const T_SIGR_LOWER_D& groupD);

	//Execute() 관련함수
	BOOL AddOrModData();
	BOOL DelData();
	void DlgToData(OUT T_SIBD_D& rData);
	T_SIBD_SRSV_D::eMomentType GetMomentTypeEnum(const CString& str);
	T_SIDP_K GetExistingDataKey(const T_SIBD_D& newData);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEIS_CVL_JP_SET_INVEST_SIBD_DLG_H__)