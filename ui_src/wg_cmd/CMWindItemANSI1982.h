#if !defined(__CMWINDITEMANSI1982_H__)
#define __CMWINDITEMANSI1982_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemANSI1982.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemANSI1982 dialog

class CCMWindItemDlg;
class CCMWindItemANSI1982 : public CChildDialog
{
// Construction
public:
	CCMWindItemANSI1982(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_WIND_ANSI1982& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_ANSI1982& data);

// Dialog Data
	//{{AFX_DATA(CCMWindItemANSI1982)
	enum { IDD = IDD_ETC_WIND_ITEM_ANSI1982 };
	CEditUnit	m_editWinwardCoef;
	CBCGPStatic  	m_unitSpeed;
	CEditUnit	m_editSpeed;
	CEditUnit	m_editLeewardCoef;
	CEditUnit	m_editIFactor;
	MComboBox	m_cobxCategory;
	CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemANSI1982)
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
	T_WIND_ANSI1982 m_Data;
	CCMWindItemDlg *m_pParent;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemANSI1982)
	virtual BOOL OnInitDialog();
		afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMANSI1982_H__)
