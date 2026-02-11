#if !defined(AFX_SEISIS1893PERIODCALCDLG_H__6843F3BE_2DEE_4F35_BEB7_99D2AD4A5016__INCLUDED_)
#define AFX_SEISIS1893PERIODCALCDLG_H__6843F3BE_2DEE_4F35_BEB7_99D2AD4A5016__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\MIT_frx\MComboBox.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_dbdoc.h"
#include "SeisIS1893_25PeriodCalcHelper.h"
using namespace mit::frx;

enum EN_IS1893_PC_DIR
{
	EN_IS1893_PC_DIR_X = 1,
	EN_IS1893_PC_DIR_Y = 2,
	EN_IS1893_PC_DIR_NONE = 3
};



class CSeisIS1893_25PeriodCalcHelper;
class CSeisIS1893_25PeriodCalcDlg : public CDialogMove
{
	// Construction
public:
	CSeisIS1893_25PeriodCalcDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL CalcPeriod(int nDir, double& dPeriod, BOOL bPrint);
	double m_dPeriod_X;
	double m_dPeriod_Y;
private:
	FILE* m_Fout = nullptr;
	CString m_strFile;
	BOOL SetFile(const CString& strFile);
public:
	enum { IDD = IDD_ETC_SEIS_CALCPERIOD_IS1893_2025 };
	CEditUnit	m_wndYHedit;
	CEditUnit	m_wndXHedit;
	CEditUnit	m_wndYDedit;
	CEditUnit	m_wndXDedit;
	CEditUnit	m_wndYAcoedit;
	CEditUnit	m_wndXAcoedit;
	CEditUnit	m_wndYAwaedit;
	CEditUnit	m_wndXAwaedit;
	CEditUnit	m_wndYAfpedit;
	CEditUnit	m_wndXAfpedit;
	CEditUnit	m_wndYAmwedit;
	CEditUnit	m_wndXAmwedit;
	CButton		m_btnReport;
	MComboBox   m_cmbBaseStory;
	int		m_nXMethod;
	int		m_nYMethod;
	double	m_dXAwaedit;
	double	m_dYAwaedit;
	double	m_dXAcoedit;
	double	m_dYAcoedit;
	double	m_dXAfpedit;
	double	m_dYAfpedit;
	double	m_dXAmwedit;
	double	m_dYAmwedit;
	double	m_dXDedit;
	double	m_dYDedit;
	double	m_dXHedit;
	double	m_dYHedit;
	T_STOR_K m_nBaseStoryK;


	//}}AFX_DATA

	void GetFilePath();
	bool Data2Dlg();
	bool Dlg2Data();
	bool CheckData();
	bool CheckDataDir(T_NPC_IS1893_2025_DIR_D& rData);
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisIS1893_25PeriodCalcDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSeisIS1893_25PeriodCalcDlg)
	virtual BOOL OnInitDialog();
	T_UNIT_INDEX SetCodeUnits();
	void RestoreOriginalUnits(T_UNIT_INDEX& rCurIndex);
	void EnableDisableWindow();
	void InitBaseStoryCmb();
	void CalculateHeight();
	void SetStoryWidthD();
	afx_msg void OnRdoXmethod();
	afx_msg void OnRdoYmethod();
	afx_msg void OnBaseStoryChange();
	virtual void OnOK();
	afx_msg void OnButtonReport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	T_NPC_IS1893_2025_D m_Data;

	BOOL m_bNTCFlag;
	BOOL m_bP100Flag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISIS02PERIODCALCDLG_H__6843F3BE_2DEE_4F35_BEB7_99D2AD4A5016__INCLUDED_)