#if !defined(__CMWINDITEMUBC1997_H__)
#define __CMWINDITEMUBC1997_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemUBC1997.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemUBC1997 dialog
class CCMWindItemDlg;
class CCMWindItemUBC1997 : public CChildDialog
{
// Construction
public:
	CCMWindItemUBC1997(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_WIND_UBC1997& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_UBC1997& data);

// Dialog Data
	//{{AFX_DATA(CCMWindItemUBC1997)
	enum { IDD = IDD_ETC_WIND_ITEM_UBC1997 };
	CBCGPStatic 	m_unitSpeed;
	CEditUnit	m_editSpeed;
	CTextUnit	m_unittRoofH;
	CEditUnit	m_editRoofH;
	CEditUnit	m_editPresCoef;
	CEditUnit	m_editIFactor;
	MComboBox	m_cobxCategory;
		CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemUBC1997)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitDefaultData();
	void InitCategoryCombo();
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	BOOL m_bInit;
	BOOL m_bModify;
	T_WIND_UBC1997   m_Data;
	CCMWindItemDlg *m_pParent;

	CArray<UINT, UINT> m_aCtrlMethod;
	CArray<UINT, UINT> m_aCtrlProjectArea;
	CArray<UINT, UINT> m_aCtrlNormalForce;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemUBC1997)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSimpleAnalRadio();
		afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMUBC1997_H__)
