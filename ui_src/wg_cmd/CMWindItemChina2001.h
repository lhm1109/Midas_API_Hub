#if !defined(__CMWINDITEMCHINA2001_H__)
#define __CMWINDITEMCHINA2001_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemChina2001.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemChina2001 dialog

class CCMWindItemDlg;
class CCMWindItemChina2001 : public CChildDialog
{
// Construction
public:
	CCMWindItemChina2001(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_WIND_CH2002& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_CH2002& data);

// Dialog Data
	//{{AFX_DATA(CCMWindItemChina2001)
	enum { IDD = IDD_ETC_WIND_ITEM_CHINA2001 };
	CEditUnit	m_editWindward;
	CEditUnit	m_editTopoCoef;
	CBCGPStatic  	m_unitSpeed;
	CEditUnit	m_editSpeed;
	CEditUnit	m_editPeriody;
	CEditUnit	m_editPeriodx;
	MComboBox	m_cobxMaterial;
	CEditUnit	m_editMagFy;
	CEditUnit	m_editMagFx;
	CEditUnit	m_editLeeward;
	CEditUnit	m_editDamping;
	MComboBox	m_cobxCategory;

	CEditUnit	m_editCheckWindPressure;//add by maxiao
	CEditUnit	m_editCheckDamping;	
		CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemChina2001)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitDefaultValue();
	void InitCategoryCombo();
	void InitMaterialCombo();
	void CalcPulseMagnifyFactor();
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	double	m_dHXCH2001;
	double	m_dHYCH2001;
	double	m_dBXCH2001;
	double	m_dBYCH2001;	
	double	m_nNXCH2001;
	double	m_nNYCH2001;	
	int		  m_nMtdXCH2001;
	int			m_nMtdYCH2001;
	BOOL m_bComfort;

	BOOL m_bInit;
	BOOL m_bModify;
	T_WIND_CH2002 m_Data;
	CCMWindItemDlg *m_pParent;

	CArray<UINT, UINT> m_aCtrlCalc;
	CArray<UINT, UINT> m_aCtrlAuto;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemChina2001)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdAutoUserRadio();
	afx_msg void OnCmdPeriodButton();
	afx_msg void OnSelchangeCmdMatlCombo();
	afx_msg void OnChangeCmdPeriodXEdit();
	afx_msg void OnChangeCmdPeriodYEdit();
	afx_msg void OnChangeCmdDampingEdit();
	afx_msg void OnSelchangeCategoryCombo();
	afx_msg void OnChangeCmdSpeedEdit();
	afx_msg void OnCmdComfortCheck();
		afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMCHINA2001_H__)
