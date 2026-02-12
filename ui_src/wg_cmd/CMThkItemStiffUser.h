#if !defined(AFX_CMTHKITEMSTIFFUSER_H__40C955E1_6560_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMTHKITEMSTIFFUSER_H__40C955E1_6560_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThkItemStiffUser.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\wg_base_ControlEx.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

class CCMThkItemStiffDlg;
/////////////////////////////////////////////////////////////////////////////
// CCMThkItemStiffUser dialog

class CCMThkItemStiffUser : public CDlgChild
{
// Construction
public:
	CCMThkItemStiffUser(int nType, CWnd* pParent = NULL);   // standard constructor
	~CCMThkItemStiffUser();

public:
	static CCMThkItemStiffDlg*	m_pParent;
	CImageList* m_pImageList;
	double	m_Size[10];

public:
	void	InitialDBData();
	void	InitControls();
	int		GetCurShapeIndex();
	void	SetSectionShapeCombo();
	void	YZInitialDBData();
	void	XZInitialDBData();
	void	YZSectSizeData();
	void	SetSectSizeData();
	BOOL	OnApplyPublic();
	void	SetSectNameCombo(CString csDB, CString csShape);
	void	UpdateSectNameCobx();
	BOOL	GetCurCalcData(T_THIK_D& thik);
	BOOL	Dlg2Data();

public:
	virtual BOOL SaveData() { return OnApplyPublic(); }	// Added by KYM

	// Dialog Data
	//{{AFX_DATA(CCMThkItemStiffUser)
	enum { IDD = IDD_CMD_TP_ITEM_STIFF_USER };
	MButton	m_ExistRib;
	CTextUnit	m_distUnit;
	CCobxAutoFill	m_sectName;
	CEditUnit	m_dist;
	MComboBox	m_cboType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThkItemStiffUser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc*	m_pDoc;
	CArray<UINT, UINT> m_aCtrlDlgItem, m_aCtrlDlgItem2;
	CArray<UINT, UINT> m_aAllCtrl;
	CArray<UINT, UINT> m_aCtrlDisableInUser;
	CArray<UINT, UINT> m_aCtrlDisableInDB;
	int			m_nIdentity;

	CBCGPStatic   m_wndFirstTitle[6];
	CEditUnit	m_wndFirstSize[6];
	CTextUnit m_wndFirstUnit[6];

	// Generated message map functions
	//{{AFX_MSG(CCMThkItemStiffUser)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdTpIdType();
	afx_msg void OnSelchangeCmdTpSectName();
	afx_msg void OnChangeSectSize();
	afx_msg void OnCmdTpExistRibs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHKITEMSTIFFUSER_H__40C955E1_6560_11D5_880F_00010263A1F4__INCLUDED_)
