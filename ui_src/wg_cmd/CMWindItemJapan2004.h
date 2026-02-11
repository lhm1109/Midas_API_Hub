#if !defined(__CMWINDITEMJAPAN2004_H__)
#define __CMWINDITEMJAPAN2004_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemJapan2000.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan2004 dialog
class CCMWindItemDlg;
class CCMWindItemJapan2004 : public CChildDialog
{
// Construction
public:
	CCMWindItemJapan2004(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_WIND_JP2004& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_JP2004& data);

// Dialog Data
	//{{AFX_DATA(CCMWindItemJapan2004)
	enum { IDD = IDD_ETC_WIND_ITEM_JAPAN2004 };
	CEditUnit	m_editCombZ;
	CEditUnit	m_editCombY;
	CEditUnit	m_editCombX;
	MEdit	    m_editWindDir;
	CEditUnit	m_editGy;
	CEditUnit	m_editGx;
	CEditUnit	m_editCfy;
	CEditUnit	m_editCfx;
	CTextUnit	m_unitHDist;
	CEditUnit	m_editHDistY;
	CEditUnit	m_editHDistX;
	CTextUnit	m_unitHLeng;
	CEditUnit	m_editHLengY;
	CEditUnit	m_editHLengX;
	CTextUnit	m_unitHHeight;
	CEditUnit	m_editHHeightY;
	CEditUnit	m_editHHeightX;
	MComboBox	m_cobxHShapeY;
	MComboBox	m_cobxHShapeX;
	MButton	  m_chkDirY;
	MButton	  m_chkDirX;
	CEditUnit	m_editKdy;
	CEditUnit	m_editKdx;
	MComboBox	m_cobxCategory;
	CTextUnit	m_unitHeight;
	CEditUnit	m_editHeight;
	CEditUnit	m_editEFactor;
	CEditUnit	m_editSpeed;
	CEditUnit	m_editMeanSpeed;
	CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemJapan2004)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void AlignControls();
	void InitControls();
	void InitCategoryCobx();
	void InitHillShapeCobx();
	void ChangeMethod();
	void ChangeTopoXY();
	void ChangeFcCoef();

	void InitDefaultData();

	void Data2Dlg();
	BOOL Dlg2Data(T_WIND_JP2004* pData);

protected:
	T_WIND_JP2004  m_Data;
	T_WIND_JP2004  m_DataBack;
	CCMWindItemDlg *m_pParent;
	BOOL    m_bModify;
	BOOL    m_bInit;
	double  m_dDgnWindSpeedX;
	double  m_dDgnWindSpeedY;

	CArray<UINT, UINT> m_aCtrlMethod;
	CArray<UINT, UINT> m_aCtrlFcCoef;
	CArray<UINT, UINT> m_aCtrlMethod1;
	CArray<UINT, UINT> m_aCtrlMethod2;
	CArray<UINT, UINT> m_aCtrlEFactor;
	CArray<UINT, UINT> m_aCtrlDirX;
	CArray<UINT, UINT> m_aCtrlDirY;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemJapan2004)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdCombBtn();
	afx_msg void OnCmdDirXCheck();
	afx_msg void OnCmdDirYCheck();
	afx_msg void OnCmdGfactorBtn();
	afx_msg void OnCmdForceCoefRadio();
	afx_msg void OnCmdMethodRadio();
	afx_msg void OnCmdParamBtn();
		afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMJAPAN2000_H__)
