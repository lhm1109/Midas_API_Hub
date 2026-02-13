#if !defined(AFX_CPGDECKOVERHANGPAGE_H__)
#define AFX_CPGDECKOVERHANGPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPGDeckOverhangPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPGDeckOverhangPage dialog
#include "..\wg_base\ChildDialog.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGDeckOverhangDlg;

class __MY_EXT_CLASS__ CCPGDeckOverhangPage : public CChildDialog
{
// Construction
public:
	CCPGDeckOverhangPage(CWnd* pParent = NULL, int nTab = 0);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CCPGDeckOverhangPage)
	enum { IDD = IDD_DGN_CPG_DECK_OVERHANG_PAGE };
	
	CEditUnit m_edtDist;
	CTextUnit m_untDist;
	CEditUnit m_edtConc;
	CTextUnit m_untConc;
	CEditUnit m_edtEccen;
	CTextUnit m_untEccen;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGDeckOverhangPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void GetDohlData();
	void ControlsEnableDisable();

	CCPGDeckOverhangDlg* m_pParent;
	T_DOHL_TYPE m_Data;

	int m_nTab;

protected:
	void InitUnit();

	void Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPGDeckOverhangPage)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPGDECKOVERHANGPAGE_H__)
