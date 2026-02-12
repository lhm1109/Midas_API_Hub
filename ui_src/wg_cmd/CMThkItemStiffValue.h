#if !defined(AFX_CMTHKITEMSTIFFVALUE_H__6F715C22_64C6_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMTHKITEMSTIFFVALUE_H__6F715C22_64C6_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThkItemStiffValue.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_DlgChild.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;


class CCMThkItemStiffDlg;
/////////////////////////////////////////////////////////////////////////////
// CCMThkItemStiffValue dialog

class CCMThkItemStiffValue : public CDlgChild
{
// Construction
public:
	CCMThkItemStiffValue(int nType, CWnd* pParent = NULL);   // standard constructor
	~CCMThkItemStiffValue();

public:
	static CCMThkItemStiffDlg*	m_pParent;
	CImageList* m_pImageList;

public:
	void	InitialDBData();
	void	InitControls();
	int		GetCurShapeIndex();
	void	SetSectionShapeCombo();
	void	YZInitialDBData();
	void	XZInitialDBData();
	BOOL	OnApplyPublic();

public:
	virtual BOOL SaveData() { return OnApplyPublic(); }	// Added by KYM

// Dialog Data
	//{{AFX_DATA(CCMThkItemStiffValue)
	enum { IDD = IDD_CMD_TP_ITEM_STIFF_VALUE };
	CTextUnit	m_unit4;
	CTextUnit	m_unit3;
	CTextUnit	m_unit2;
	CTextUnit	m_unit1;
	CEditUnit	m_value4;
	CEditUnit	m_value3;
	CEditUnit	m_value2;
	CEditUnit	m_value1;
	MComboBox	m_cboType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThkItemStiffValue)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int		m_nIdentity;

	// Generated message map functions
	//{{AFX_MSG(CCMThkItemStiffValue)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdTpIdType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHKITEMSTIFFVALUE_H__6F715C22_64C6_11D5_880F_00010263A1F4__INCLUDED_)
