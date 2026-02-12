#if !defined(AFX_CMSECTITEMPSC3CELLGRIDDLG_H__B4F1908D_C050_4772_8A30_991092029FDD__INCLUDED_)
#define AFX_CMSECTITEMPSC3CELLGRIDDLG_H__B4F1908D_C050_4772_8A30_991092029FDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSC3CellGridDlg.h : header file
//

#include "CMSectItemPSC3CellJointGrid.h"
#include "CMSectItemPSC3CellSizeGrid.h"

#include "../MIT_frx/MTabCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC3CellGridDlg dialog

class CCMSectItemPSC3CellGridDlg : public CDialogMove
{
// Construction
public:
	CCMSectItemPSC3CellGridDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSC3CellGridDlg)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_3CELL_GRID_DLG };
	mit::frx::MTabCtrl	m_tabOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSC3CellGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCMSectItemPSC3CellJointGrid m_wndJointGrid;
	CCMSectItemPSC3CellSizeGrid m_wndSizeGrid;
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
	//{{AFX_MSG(CCMSectItemPSC3CellGridDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmd3cellTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSC3CELLGRIDDLG_H__B4F1908D_C050_4772_8A30_991092029FDD__INCLUDED_)







