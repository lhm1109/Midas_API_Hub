#if !defined(__CMWINDITEMNBC1995_H__)
#define __CMWINDITEMNBC1995_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemNBC1995.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemNBC1995 dialog
class CCMWindItemDlg;
class CCMWindItemNBC1995 : public CChildDialog
{
// Construction
public:
	CCMWindItemNBC1995(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_WIND_NBC1995& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_NBC1995& data);

// Dialog Data
	//{{AFX_DATA(CCMWindItemNBC1995)
	enum { IDD = IDD_ETC_WIND_ITEM_NBC1995 };
	MComboBox	m_cobxHillShape;
	MButton	m_chkTopographic;
	CBCGPStatic  	m_unitSpeed;
	CEditUnit	m_editSpeed;
	MButton	  m_chkLoadEval;
	CTextUnit	m_unitHLength;
	CEditUnit	m_editHLength;
	CTextUnit	m_unitHHeight;
	CEditUnit	m_editHHeight;
	CEditUnit	m_editGustFactor;
	CEditUnit	m_editGustCgy;
	CEditUnit	m_editGustCgx;
	CEditUnit	m_editForceCoef;
	CTextUnit	m_unitCBDistance;
	CEditUnit	m_editCBDistance;
	MComboBox	m_cobxCategory;
	MComboBox	m_cobxBLocation;
	CTextUnit	m_unitBHeight;
	CEditUnit	m_editBHeight;
		CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemNBC1995)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitDefaultData();
	void AlignControls();
	void InitCategoryCombo();
	void InitHillShapeCombo();
	void InitHillLocCombo();
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	double	m_dNbc95BX;
	double	m_dNbc95BY; 
	double	m_dNbc95FrequencyX;
	double	m_dNbc95FrequencyY;
	double	m_dNbc95Damping;

	BOOL m_bInit;
	BOOL m_bModify;
	T_WIND_NBC1995 m_Data;
	CCMWindItemDlg *m_pParent;

	CArray<UINT, UINT> m_aCtrlProcedure;
	CArray<UINT, UINT> m_aCtrlDetail;
	CArray<UINT, UINT> m_aCtrlSimple;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemNBC1995)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSimpleDetailRadio();
	afx_msg void OnCmdLoadevalChk();
	afx_msg void OnCmdTopoCheck();
	afx_msg void OnSelchangeCmdHillshapeCombo();
	afx_msg void OnCmdGustfButton2();
		afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMNBC1995_H__)
