#if !defined(AFX_CMSECTITEMPSCIGRIDDLG_H__EDADDB82_168B_4205_8532_26B8F55C4714__INCLUDED_)
#define AFX_CMSECTITEMPSCIGRIDDLG_H__EDADDB82_168B_4205_8532_26B8F55C4714__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCIGridDlg.h : header file
//

#include "CMSectItemPSCIJointGrid.h"
#include "CMSectItemPSCISizeGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCIGridDlg dialog

class CCMSectItemPSCIGridDlg : public CDialogMove
{
// Construction
public:
	CCMSectItemPSCIGridDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCCMSectItemPSCIGridDlg)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_I_GRID_DLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCMSectItemPSCIGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCMSectItemPSCIJointGrid m_wndJointGrid;
	CCMSectItemPSCISizeGrid m_wndSizeGrid;
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
	//{{AFX_MSG(CCCMSectItemPSCIGridDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSCIGRIDDLG_H__EDADDB82_168B_4205_8532_26B8F55C4714__INCLUDED_)







