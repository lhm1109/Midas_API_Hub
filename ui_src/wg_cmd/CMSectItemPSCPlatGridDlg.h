#if !defined(AFX_CMSECTITEMPSCPLATGRIDDLG_H__A0FD4943_4907_409B_8614_E649ED13C139__INCLUDED_)
#define AFX_CMSECTITEMPSCPLATGRIDDLG_H__A0FD4943_4907_409B_8614_E649ED13C139__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCPlatGridDlg.h : header file
//

#include "CMSectItemPSCPlatJointGrid.h"
#include "CMSectItemPSCPlatSizeGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCPlatGridDlg dialog

class CCMSectItemPSCPlatGridDlg : public CDialogMove
{
// Construction
public:
	CCMSectItemPSCPlatGridDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCCMSectItemPSCPlatGridDlg)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_PLAT_GRID_DLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCMSectItemPSCPlatGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCMSectItemPSCPlatJointGrid m_wndJointGrid;
	CCMSectItemPSCPlatSizeGrid m_wndSizeGrid;
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
	//{{AFX_MSG(CCCMSectItemPSCPlatGridDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSCPLATGRIDDLG_H__A0FD4943_4907_409B_8614_E649ED13C139__INCLUDED_)







