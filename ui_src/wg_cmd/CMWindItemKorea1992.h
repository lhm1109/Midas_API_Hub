#if !defined(__CMWINDITEMKOREA1992_H__)
#define __CMWINDITEMKOREA1992_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemKorea1992.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\mit_frx\MComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemKorea1992 dialog
using namespace mit::frx;
class CCMWindItemDlg;
class CCMWindItemKorea1992 : public CChildDialog
{
// Construction
public:
	CCMWindItemKorea1992(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_WIND_KS1992& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_KS1992& data);

// Dialog Data
	//{{AFX_DATA(CCMWindItemKorea1992)
	enum { IDD = IDD_ETC_WIND_ITEM_KOREA1992 };
	CBCGPStatic 	m_unitSpeed;
	CEditUnit	m_editSpeed;
	MComboBox	m_cobxCategory;
	CEditUnit	m_editCoeff;
		CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemKorea1992)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
protected:
	void InitCategotyCombo();
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	BOOL m_bModify;
	T_WIND_KS1992  m_Data;
	CCMWindItemDlg *m_pParent;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemKorea1992)
	virtual BOOL OnInitDialog();
		afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMKOREA1992_H__)
