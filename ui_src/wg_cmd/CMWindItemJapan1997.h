#if !defined(__CMWINDITEMJAPAN1997_H__)
#define __CMWINDITEMJAPAN1997_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemJapan1997.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan1997 dialog
class CCMWindItemDlg;
class CCMWindItemJapan1997 : public CChildDialog
{
// Construction
public:
	CCMWindItemJapan1997(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_WIND_JP1987& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_JP1987& data);

// Dialog Data
	//{{AFX_DATA(CCMWindItemJapan1997)
	enum { IDD = IDD_ETC_WIND_ITEM_JAPAN1997 };
	CEditUnit	m_editReduFactor;
	CEditUnit	m_editPresCoef;
		CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemJapan1997)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	BOOL m_bModify;
	T_WIND_JP1987  m_Data;
	CCMWindItemDlg *m_pParent;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemJapan1997)
	virtual BOOL OnInitDialog();
		afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMJAPAN1997_H__)
