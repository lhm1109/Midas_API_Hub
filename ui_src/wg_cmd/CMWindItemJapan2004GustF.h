#if !defined(__CMWINDITEMJAPAN2004GUSTF_H__)
#define __CMWINDITEMJAPAN2004GUSTF_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemJapan2004GustF.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MEdit.h"
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan2004GustF dialog

class CCMWindItemJapan2004GustF : public CDialogMove
{
// Construction
public:
	CCMWindItemJapan2004GustF(T_WIND_JP2004* pData, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMWindItemJapan2004GustF)
	enum { IDD = IDD_ETC_WIND_ITEM_JAPAN2004_GUSTF };
	CEditUnit	m_editModeY;
	CEditUnit	m_editModeX;
	CEditUnit	m_editGustY;
	CEditUnit	m_editGustX;
	CEditUnit	m_editFreqY;
	CEditUnit	m_editFreqX;
	CEditUnit	m_editDampY;
	CEditUnit	m_editDampX;
	CTextUnit	m_unitBreadthY;
	CEditUnit	m_editBreadthY;
	CTextUnit	m_unitBreadthX;
	CEditUnit	m_editBreadthX;
	MEdit	    m_editTopoY;
	MEdit	    m_editTopoX;
	CTextUnit	m_unitHeight;
	CEditUnit	m_editHeight;
	MEdit   	m_editCategory;
	CEditUnit	m_editSpeedY;
	CEditUnit	m_editSpeedX;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemJapan2004GustF)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitControls();
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	T_WIND_JP2004* m_pData;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemJapan2004GustF)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdCalcBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMJAPAN2004GUSTF_H__)
