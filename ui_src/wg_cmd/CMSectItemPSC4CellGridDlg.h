#if !defined(AFX_CMSECTITEMPSC4CELLGRIDDLG_H__E22DC569_6592_410C_B1EF_E539854F3A56__INCLUDED_)
#define AFX_CMSECTITEMPSC4CELLGRIDDLG_H__E22DC569_6592_410C_B1EF_E539854F3A56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSC4CellGridDlg.h : header file
//

#include "CMSectItemPSC4CellGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC4CellGridDlg dialog

class CCMSectItemPSC4CellGridDlg : public CDialogMove
{
// Construction
public:
	CCMSectItemPSC4CellGridDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSC4CellGridDlg)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_4CELL_GRID_DLG };
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSC4CellGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCMSectItemPSC4CellGrid m_wndGrid;
public:
	CWnd* m_pWnd;
	T_SECT_D* m_pSect;
	


public:
	void SetData(T_SECT_D* pSect,CWnd* pWnd);
	void DecideEnable();

protected:
	void InitControls();
public:
	void ShowData();

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSC4CellGridDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmd3cellTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSC4CELLGRIDDLG_H__E22DC569_6592_410C_B1EF_E539854F3A56__INCLUDED_)







