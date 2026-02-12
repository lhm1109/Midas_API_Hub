#if !defined(AFX_CMWINDITEMTAIWAN1986_H__4EF342CD_5488_404D_8046_67A74C03893A__INCLUDED_)
#define AFX_CMWINDITEMTAIWAN1986_H__4EF342CD_5488_404D_8046_67A74C03893A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemTaiwan1986.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemTaiwan1986 dialog
class CCMWindItemDlg;
class CCMWindItemTaiwan1986 : public CChildDialog
{
// Construction
public:
	CCMWindItemTaiwan1986(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_WIND_TAIWAN86& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_TAIWAN86& data);
// Dialog Data
	//{{AFX_DATA(CCMWindItemTaiwan1986)
	enum { IDD = IDD_ETC_WIND_ITEM_TAIWAN1986 };
	CComboBox	m_wndSiteCategoryCmb;
	CEditUnit	m_wndShapeFactor;
		CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemTaiwan1986)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:  
	void InitSiteCategoryCombo();
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	BOOL m_bInit;
	BOOL m_bModify;
	T_WIND_TAIWAN86  m_Data;
	CCMWindItemDlg *m_pParent;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemTaiwan1986)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMWINDITEMTAIWAN1986_H__4EF342CD_5488_404D_8046_67A74C03893A__INCLUDED_)
