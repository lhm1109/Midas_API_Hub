#if !defined(__CMWINDITEMJAPAN2000_H__)
#define __CMWINDITEMJAPAN2000_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemJapan2000.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan2000 dialog
class CCMWindItemDlg;
class CCMWindItemJapan2000 : public CChildDialog
{
// Construction
public:
	CCMWindItemJapan2000(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_WIND_JPN2000& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_JPN2000& data);

// Dialog Data
	//{{AFX_DATA(CCMWindItemJapan2000)
	enum { IDD = IDD_ETC_WIND_ITEM_JAPAN2000 };
	CBCGPStatic  	m_unitSpeed;
	CEditUnit	m_editSpeed;
	MComboBox	m_cobxCategory;
		CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemJapan2000)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitCategoryCombo();
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	BOOL m_bModify;
	T_WIND_JPN2000  m_Data;
	CCMWindItemDlg *m_pParent;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemJapan2000)
	virtual BOOL OnInitDialog();
		afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMJAPAN2000_H__)
