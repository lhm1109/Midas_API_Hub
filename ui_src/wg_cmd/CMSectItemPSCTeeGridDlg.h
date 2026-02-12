#if !defined(AFX_CMSECTITEMPSCTEEGRIDDLG_H__6B8643CB_460D_4C79_A6FA_E3E0F9D1F27E__INCLUDED_)
#define AFX_CMSECTITEMPSCTEEGRIDDLG_H__6B8643CB_460D_4C79_A6FA_E3E0F9D1F27E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCTeeGridDlg.h : header file
//

#include "CMSectItemPSCIJointGrid.h"
#include "CMSectItemPSCTeeSizeGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCTeeGridDlg dialog

class CCMSectItemPSCTeeGridDlg : public CDialogMove
{
// Construction
public:
	CCMSectItemPSCTeeGridDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCCMSectItemPSCTeeGridDlg)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_TEE_GRID_DLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCMSectItemPSCTeeGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCMSectItemPSCIJointGrid m_wndJointGrid;
	CCMSectItemPSCTeeSizeGrid m_wndSizeGrid;
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
	//{{AFX_MSG(CCCMSectItemPSCTeeGridDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSCTEEGRIDDLG_H__6B8643CB_460D_4C79_A6FA_E3E0F9D1F27E__INCLUDED_)







