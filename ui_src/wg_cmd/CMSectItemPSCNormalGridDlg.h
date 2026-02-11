#if !defined(AFX_CMSECTITEMPSCNORMALGRIDDLG_H__2EFA9E5B_F2F2_436A_8C57_D12E7C1E20B2__INCLUDED_)
#define AFX_CMSECTITEMPSCNORMALGRIDDLG_H__2EFA9E5B_F2F2_436A_8C57_D12E7C1E20B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCNormalGridDlg.h : header file
//

#include "CMSectItemPSCNormalGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCNormalGridDlg dialog

class CCMSectItemPSCNormalGridDlg : public CDialogMove
{
// Construction
public:
	CCMSectItemPSCNormalGridDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSCNormalGridDlg)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_NORMAL_GRID_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCNormalGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCMSectItemPSCNormalGrid m_wndGrid;
public:
	CWnd* m_pWnd;
	T_SECT_D* m_pSect;
	


public:
	void SetData(T_SECT_D* pSect,CWnd* pWnd);

protected:
	void InitControls();
public:
	void ShowData();

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSCNormalGridDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSCNORMALGRIDDLG_H__2EFA9E5B_F2F2_436A_8C57_D12E7C1E20B2__INCLUDED_)







