#if !defined(__CMWINDITEMJAPAN2004LOADF_H__)
#define __CMWINDITEMJAPAN2004LOADF_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemJapan2004LoadF.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan2004LoadF dialog

class CCMWindItemJapan2004LoadF : public CInternationalDlg
{
// Construction
public:
	CCMWindItemJapan2004LoadF(T_WIND_JP2004* pData, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMWindItemJapan2004LoadF)
	enum { IDD = IDD_ETC_WIND_ITEM_JAPAN2004_LOADF };
	CEditUnit	m_editFreqZ;
	CEditUnit	m_editFreqY;
	CEditUnit	m_editFreqX;
	CEditUnit	m_editGustY;
	CEditUnit	m_editGustX;
	CTextUnit	m_unitBreadthY;
	CEditUnit	m_editBreadthY;
	CTextUnit	m_unitBreadthX;
	CEditUnit	m_editBreadthX;
	CTextUnit	m_unitHeight;
	CEditUnit	m_editHeight;
	CEditUnit	m_editSpeedY;
	CEditUnit	m_editSpeedX;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemJapan2004LoadF)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitControls();
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	T_WIND_JP2004* m_pData;

	CArray<UINT, UINT> m_aCtrlWindDir;
	CArray<UINT, UINT> m_aCtrlCombType;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemJapan2004LoadF)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdCalcBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMJAPAN2004LOADF_H__)
