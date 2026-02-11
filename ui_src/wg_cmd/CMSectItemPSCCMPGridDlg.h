#if !defined(AFX_CMSECTITEMPSCCMPGRIDDLG_H__A0788A85_6FB8_40C2_AC3C_671D164A962C__INCLUDED_)
#define AFX_CMSECTITEMPSCCMPGRIDDLG_H__A0788A85_6FB8_40C2_AC3C_671D164A962C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCCMPGridDlg.h : header file
//
#include "../MIT_frx/MTabCtrl.h"
#include "CMSectItemPSCCMPJointGrid.h"
#include "CMSectItemPSCCMPSizeGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCCMPGridDlg dialog

class CCMSectItemPSCCMPGridDlg : public CDialogMove
{
// Construction
public:
	CCMSectItemPSCCMPGridDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCCMSectItemPSCCMPGridDlg)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_CMP_GRID_DLG };
	mit::frx::MTabCtrl	m_tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCMSectItemPSCCMPGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCMSectItemPSCCMPJointGrid m_wndJointGrid;
	CCMSectItemPSCCMPSizeGrid m_wndSizeGrid;
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
	//{{AFX_MSG(CCCMSectItemPSCCMPGridDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSCCMPGRIDDLG_H__A0788A85_6FB8_40C2_AC3C_671D164A962C__INCLUDED_)







