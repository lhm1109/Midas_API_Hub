#if !defined(AFX_CMSECTITEMPSCMIDGRIDDLG_H__C522625F_07E2_49C8_A957_D322C353296F__INCLUDED_)
#define AFX_CMSECTITEMPSCMIDGRIDDLG_H__C522625F_07E2_49C8_A957_D322C353296F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCMidGridDlg.h : header file
//

#include "CMSectItemPSCMidJointGrid.h"
#include "CMSectItemPSCMidSizeGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCMidGridDlg dialog

class CCMSectItemPSCMidGridDlg : public CDialogMove
{
// Construction
public:
	CCMSectItemPSCMidGridDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCCMSectItemPSCMidGridDlg)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_MID_GRID_DLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCMSectItemPSCMidGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCMSectItemPSCMidJointGrid m_wndJointGrid;
	CCMSectItemPSCMidSizeGrid m_wndSizeGrid;
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
	//{{AFX_MSG(CCCMSectItemPSCMidGridDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSCMIDGRIDDLG_H__C522625F_07E2_49C8_A957_D322C353296F__INCLUDED_)







