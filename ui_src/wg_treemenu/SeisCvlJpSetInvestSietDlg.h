#if !defined(__SEIS_CVL_JP_SET_INVEST_SIET_DLG_H__)
#define __SEIS_CVL_JP_SET_INVEST_SIET_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSeisCvlJpSetInvestSietDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestSietDlg dialog

class CSeisCvlJpSetInvestSietDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
private:
	int			m_nOption;
	MComboBox	m_caseName;
	MComboBox	m_groupName;
	int			m_nOutType_Br;
	MComboBox	m_concurvity_Br;
	BOOL		m_bPierBase_Sf;
	BOOL		m_bFootingLower_Sf;
	int			m_nOutType_Sf;
	MComboBox	m_concurvity_Mb;

	CDBDoc*		m_pDoc;

	// Construction
public:
	CSeisCvlJpSetInvestSietDlg(CWnd* pParent = NULL);   // standard constructor
	~CSeisCvlJpSetInvestSietDlg();

	// Dialog Data
		//{{AFX_DATA(CSeisCvlJpSetInvestSietDlg)
	enum { IDD = IDD_TM_SIET };

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCvlJpSetInvestSietDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSeisCvlJpSetInvestSietDlg)
	virtual BOOL OnInitDialog();

	afx_msg void OnChangedMemberChk();
	afx_msg void OnChangedConcurvityBrRdo();
	afx_msg void OnChangedConcurvityMbRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual void Execute();

private:
	void InitControl();
	void InitCaseNameCtrl();
	void InitGroupNameCtrl();
	void InitConcurvityBrCtrl();
	void InitConcurvityMbCtrl();
	//Execute() 관련함수
	BOOL AddOrModData();
	BOOL DelData();
	void DlgToData(OUT T_SIET_D& rData);
	T_SIET_K GetExistingDataKey(const T_SIET_D& newData);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEIS_CVL_JP_SET_INVEST_SIET_DLG_H__)