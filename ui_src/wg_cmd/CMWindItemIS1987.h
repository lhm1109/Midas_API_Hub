#if !defined(__CMWINDITEMIS1987_H__)
#define __CMWINDITEMIS1987_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemIS1987.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemIS1987 dialog
class CCMWindItemDlg;
class CCMWindItemIS1987 : public CChildDialog
{
// Construction
public:
	CCMWindItemIS1987(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_WIND_IS1987& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_IS1987& data);

// Dialog Data
	//{{AFX_DATA(CCMWindItemIS1987)
	enum { IDD = IDD_ETC_WIND_ITEM_IS1987 };
	CEditUnit	m_editSpeed;
	CEditUnit	m_editForceCoef;
	CTextUnit	m_unitVRange;
	CEditUnit	m_editVRange;
	CEditUnit	m_editTopoFactor;
	MButton	  m_chkTopographic;
	CBCGPStatic 	m_unitSpeed;
	MComboBox	m_cobxStructClass;
	CEditUnit	m_editRiskCoef;
	MButton	  m_chkLoadEval;
	CEditUnit	m_editGustFactor;
	MComboBox	m_cobxFriction;
	CEditUnit	m_editForceCoef2;
	MComboBox	m_cobxCategory;
	MComboBox	m_cobxBuildingClass;
		CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemIS1987)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitDefaultData();
	void AlignControls();
	void InitCategoryCombo();
	void InitBuildingClassCombo();
	void InitSpeedCombo();
	void InitFrictionCombo();
	void InitStructureClassCombo();
	void CalculateRiskCoef();
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	BOOL m_bInit;
	BOOL m_bModify;
	T_WIND_IS1987  m_Data;
	CCMWindItemDlg *m_pParent;
	
	CArray<UINT, UINT> m_aCtrlMethod;
	CArray<UINT, UINT> m_aCtrlAutoUser;
	CArray<UINT, UINT> m_aCtrlStandard;
	CArray<UINT, UINT> m_aCtrlGustFactor;
	CArray<UINT, UINT> m_aCtrlFriction;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemIS1987)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdMethodRadio();
	afx_msg void OnCmdAutoUserRadio();
	afx_msg void OnSelchangeCmdSclassCombo();
	afx_msg void OnCmdLoadevalChk();
	afx_msg void OnCmdTopoChk();
	afx_msg void OnChangeCmdSpeedEdit();
		afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMIS1987_H__)
