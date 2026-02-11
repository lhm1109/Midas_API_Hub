#if !defined(AFX_CMSECTITEMPSCNCELLGRIDDLG_H__79F658E8_45D3_40E4_906C_24DFDB57F07C__INCLUDED_)
#define AFX_CMSECTITEMPSCNCELLGRIDDLG_H__79F658E8_45D3_40E4_906C_24DFDB57F07C__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCnCellGridDlg.h : header file
//
#include "CMSectItemPSCnCellDownGrid.h"
#include "CMSectItemPSCnCellUpGrid.h"
#include "../MIT_frx/MTabCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCnCellGridDlg dialog

class CCMSectItemPSCnCellGridDlg : public CDialogMove
{
// Construction
public:
	CCMSectItemPSCnCellGridDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSCnCellGridDlg)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_NCELL_GRID_DLG };
	mit::frx::MTabCtrl	m_wndTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCnCellGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation


public:
	void SetData(T_SECT_D* pSect,CWnd* pWnd);

protected:
	void InitControls();
public:
	void ShowData();
protected:
	T_SECT_D* m_pSect;
	CCMSectItemPSCnCellDownGrid m_wndDownGrid;
	CCMSectItemPSCnCellUpGrid m_wndUpGrid;
public:
	CWnd* m_pWnd;

protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSCnCellGridDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdTabCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSCNCELLGRIDDLG_H__79F658E8_45D3_40E4_906C_24DFDB57F07C__INCLUDED_)
