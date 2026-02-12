#if !defined(__SEIS_CVL_JP_SET_INVEST_SIDP_DLG_H__)
#define __SEIS_CVL_JP_SET_INVEST_SIDP_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSeisCvlJpSetInvestSidpDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestSidpDlg dialog

class CSeisCvlJpSetInvestSidpDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
private:
	int			m_nOption;
	MComboBox	m_caseName;
	MComboBox	m_groupName;
	MComboBox	m_colmGroupName;
	int			m_nAllowDispPlasFormula;
	MEdit		m_Alpha;
	MEdit		m_DeltaLs;
	int			m_nLpFormula;
	MEdit		m_LpLength;

	CDBDoc* m_pDoc;

	// Construction
public:
	CSeisCvlJpSetInvestSidpDlg(CWnd* pParent = NULL);   // standard constructor
	~CSeisCvlJpSetInvestSidpDlg();

	// Dialog Data
		//{{AFX_DATA(CSeisCvlJpSetInvestSidpDlg)
	enum { IDD = IDD_TM_SIDP };

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCvlJpSetInvestSidpDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL
	
	// Generated message map functions
	//{{AFX_MSG(CSeisCvlJpSetInvestSidpDlg)
	afx_msg void OnChangedCaseCmb();
	afx_msg void OnChangedGroupCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual void Execute();

private:
	void InitControl();
	void InitCaseNameCtrl();
	void InitGroupNameCtrl();
	void UpdateColmGroupNameCtrl();
	void InitAdprFormulaCtrl();
	void InitDeltaLs_LengthCtrl();

	void UpdateControl_Alpha();
	void UpdateControl_AlphaH14(const T_SIPA_D& sipaD, const T_SIGR_LOWER_D& groupD);
	void UpdateControl_AlphaH24(const T_SIGR_LOWER_D& data);

	void UpdateControl_DeltaLs();

	//Execute() 관련함수
	BOOL AddOrModData();
	BOOL DelData();
	BOOL DlgToData(OUT T_SIDP_D& rData, OUT CString& errMsg);
	T_SIDP_K GetExistingDataKey(const T_SIDP_D& newData);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEIS_CVL_JP_SET_INVEST_SIDP_DLG_H__)