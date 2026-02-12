#if !defined(AFX_CMTHKITEMSTLWALLDLG_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMTHKITEMSTLWALLDLG_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThkItemValueDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_DlgChild.h"

struct StlWall
{
	int ID;
	double ThickCon;
	double ThickSteel;
	void Initialize()
	{
		ThickCon = 0.0;
		ThickSteel = 0.0;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CCMThkItemStlWallDlg dialog

class CCMThkItemStlWallDlg : public CDlgChild
{
// Construction
public:
	CCMThkItemStlWallDlg(CDBDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

public:
	CDBDoc* m_pDoc;
	BOOL		m_bModify;
	StlWall		m_sDlgData;

	T_THIK_K m_Key;
	T_THIK_D m_Data;

public:
	void	SetModifyData(T_THIK_K Key, T_THIK_D &Data);
	BOOL	OnApplyPublic();

// Dialog Data
	//{{AFX_DATA(CCMThkItemStlWallDlg)
	enum { IDD = IDD_CMD_TP_ITEM_STLWALL };
	CTextUnit	m_UnitCon;
	CTextUnit	m_UnitSteel;
	UINT	m_nID;
	double m_dbThkCon;
	double m_dbThkSteel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThkItemStlWallDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void MakeUnitSystem();
	BOOL SaveData();
	void GetData(T_THIK_K &Key, T_THIK_D &Data);
	void SetData();
	void InitDlgItems(BOOL bOnInitDialog);

	// Generated message map functions
	//{{AFX_MSG(CCMThkItemStlWallDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHKITEMSTLWALLDLG_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_)
