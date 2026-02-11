//CMWindItemINDIA2015ManDlg.h : header file
// Add by Pinakin 2020-2-20  For IS 875 2015 


#if !defined(AFX_CMWindItemINDIA2015ManDlg_H__04C9E1E2_3D38_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMWindItemINDIA2015ManDlg_H__04C9E1E2_3D38_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
//#include "ShapeGrid.h"
#include "CMGXGridWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CMWindItemINDIA2015ManDlg dialog
class CMWindItemINDIA2015ManDlg : public CDialogMove
{
// Construction
public:
	CMWindItemINDIA2015ManDlg(CWnd* pParent = NULL);   // standard constructor
//	~CMWindItemINDIA2015ManDlg();
protected:

// Dialog Data
	//{{AFX_DATA(CMWindItemINDIA2015ManDlg)
	enum { IDD = IDD_ETC_WIND_ITEM_INDIA2015_WINDSHIELD_DLG };
	
//  CShapeGrid* m_pGrid;
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMWindItemINDIA2015ManDlg)
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
	//{{AFX_MSG(CMWindItemINDIA2015ManDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMWindItemINDIA2015ManDlg_H__04C9E1E2_3D38_11D5_880F_00010263A1F4__INCLUDED_)
