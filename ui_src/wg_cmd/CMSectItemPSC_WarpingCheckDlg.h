#if !defined(AFX_CMSectItemPSC_WarpingCheckDlg_H_INCLUDED_)
#define AFX_CMSectItemPSC_WarpingCheckDlg_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSC_WarpingCheckDlg.h : header file
//

#include "CMSectItemPSC_WarpingCheckGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC_WarpingCheckDlg dialog

class CCMSectItemPSC_WarpingCheckDlg : public CDialogMove
{
	// Construction
public:
	CCMSectItemPSC_WarpingCheckDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CCCMSectItemPSC_WarpingCheckDlg)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_WARPINGCHECK_DLG };
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCMSectItemPSC_WarpingCheckDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CCMSectItemPSC_WarpingCheckGrid m_wndPointGrid;
public:
	CWnd* m_pWnd;
	T_SECT_D* m_pSect;



public:
	void SetData(T_SECT_D* pSect,CWnd* pWnd);
	//void DecideEnable();

protected:
	void InitControls();
public:
	void ShowData();

	// Generated message map functions
	//{{AFX_MSG(CCCMSectItemPSC_WarpingCheckDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSectItemPSC_WarpingCheckDlg_H_INCLUDED_)







