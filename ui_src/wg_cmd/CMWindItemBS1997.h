#if !defined(__CMWINDITEMBS1997_H__)
#define __CMWINDITEMBS1997_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemBS1997.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemBS1997 dialog

class CCMWindItemDlg;
class CCMWindItemBS1997 : public CChildDialog
{
// Construction
public:
	CCMWindItemBS1997(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_WIND_BS6399& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_BS6399& data);

// Dialog Data
	//{{AFX_DATA(CCMWindItemBS1997)
	enum { IDD = IDD_ETC_WIND_ITEM_BS1997 };
	CBCGPStatic 	m_unitTownDist;
	CEditUnit	m_editTownDist;
	CBCGPStatic  	m_unitSpeed;
	CEditUnit	m_editSpeed;
	CTextUnit	m_unitSeparation;
	CEditUnit	m_editSeparation;
	CBCGPStatic 	m_unitSeaDist;
	CEditUnit	m_editSeaDist;
	CTextUnit	m_unitRoofH;
	CEditUnit	m_editRoofH;
	CEditUnit	m_editFriction;
	MComboBox	m_cobxCategory;
	MComboBox	m_cobxBDType;
		CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemBS1997)
public:
		virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitCategoryCombo();
	void InitBuildingTypeCombo();
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	BOOL m_bModify;
	T_WIND_BS6399  m_Data;
	CCMWindItemDlg *m_pParent;

	CArray<UINT, UINT> m_aCtrlMethod; 
	CArray<UINT, UINT> m_aCtrlDirection;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemBS1997)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdMethodRadio();
	afx_msg void OnSelchangeCmdCategoryCombo();
	afx_msg void OnCmdSpeedfactorBtn();
		afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMBS1997_H__)
