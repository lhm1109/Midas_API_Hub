#pragma once
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemChina2019 dialog

class CCMWindItemDlg;
class CCMWindItemChina2019 : public CChildDialog
{
// Construction
public:
	CCMWindItemChina2019(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_WIND_CH2019& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_CH2019& data);

// Dialog Data
	//{{AFX_DATA(CCMWindItemChina2019)
	enum { IDD = IDD_ETC_WIND_ITEM_CHINA2019 };
	MComboBox	m_cobxCategory;

	CEditUnit	m_editBasicWindPressure;
	CEditUnit	m_editModifyFactor;	
	CEditUnit	m_editWindDirect;	
	CEditUnit	m_editDamping;

	CEditUnit m_editWindwardCoefX;  
	CEditUnit m_editLeewardCoefX;   
	CEditUnit m_editWindwardCoefY;  
	CEditUnit m_editLeewardCoefY;   

	CEditUnit	m_editPeriody;
	CEditUnit	m_editPeriodx;

	CEditUnit	m_editCheckWindPressure;
	CEditUnit	m_editCheckDamping;	
		CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemChina2019)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitDefaultValue();
	void InitCategoryCombo();
	void CalcPulseMagnifyFactor();
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	BOOL m_bPartitionCoef;
	BOOL m_bShield;
	BOOL m_bAlong;
	BOOL m_bCross;
	BOOL m_bComfort;
	
	BOOL m_bInit;
	BOOL m_bModify;
	T_WIND_CH2019 m_Data;
	CCMWindItemDlg *m_pParent;

	CArray<UINT, UINT> m_aCtrlStruct;
	CArray<UINT, UINT> m_aCtrlAlongStructure;

	double m_dHXCH2019;
	double m_dHYCH2019;
	double m_dBXCH2019;
	double m_dBYCH2019;	
	int m_nNXCH2019;
	int m_nNYCH2019;	
	int m_nMtdXCH2019;
	int m_nMtdYCH2019;

// Implementation
protected:
	double GetWindCoff_KW (const T_WIND_CH2019 &data);
	void CalWind_PulseMagnifyFactor    (T_WIND_CH2019 &data);

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemChina2019)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdPeriodButton();
	afx_msg void OnCmdPartitionButton();
	afx_msg void OnCmdShieldButton();
	afx_msg void OnCmdCrossButton();
	afx_msg void OnCmdPartitionCheck();
	afx_msg void OnCmdShieldCheck();
	afx_msg void OnCmdCrossCheck();
	afx_msg void OnCmdComfortCheck();
	afx_msg void OnCmdWindAlongCheck();
	
	afx_msg void OnChangeCmdPeriodXEdit();
	afx_msg void OnChangeCmdPeriodYEdit();
	afx_msg void OnChangeCmdDampingEdit();
	afx_msg void OnSelchangeCategoryCombo();
	afx_msg void OnChangeCmdSpeedEdit();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

