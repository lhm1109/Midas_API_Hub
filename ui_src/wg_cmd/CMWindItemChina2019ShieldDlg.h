//CMWindItemChina2019ShieldDlg.h : header file
// Add by maxiao 2011-5-10  For china GB 50009-2011


#if !defined(AFX_CMWindItemChina2019ShieldDlg_H__04C9E1E2_3D38_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMWindItemChina2019ShieldDlg_H__04C9E1E2_3D38_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
//#include "ShapeGrid.h"
#include "CMGXGridWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CMWindItemChina2019ShieldDlg dialog
class CMWindItemChina2019ShieldDlg : public CDialogMove
{
// Construction
public:
	CMWindItemChina2019ShieldDlg(CWnd* pParent = NULL);   // standard constructor
//	~CMWindItemChina2019ShieldDlg();
protected:

// Dialog Data
	//{{AFX_DATA(CMWindItemChina2019ShieldDlg)
	enum { IDD = IDD_ETC_WIND_ITEM_CHINA2019_WINDSHIELD_DLG };
	
//  CShapeGrid* m_pGrid;
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMWindItemChina2019ShieldDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	void InitGrid();
	void SetGridTitle();

	void Data2Grid();
	BOOL Grid2Data();

	BOOL m_bInit;
	CCMGXGridWnd2 m_wndGrid;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMWindItemChina2019ShieldDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMWindItemChina2019ShieldDlg_H__04C9E1E2_3D38_11D5_880F_00010263A1F4__INCLUDED_)
