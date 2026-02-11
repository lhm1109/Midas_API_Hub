#if !defined(AFX_CMSECTITEMPSCOUTTER_H__CDD38C7A_389E_492D_9D55_A474F81A0FF3__INCLUDED_)
#define AFX_CMSECTITEMPSCOUTTER_H__CDD38C7A_389E_492D_9D55_A474F81A0FF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCOutter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCOutter dialog
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\wg_base_ChildDialog.h"

class CCMSectItemPSCOutter : public CChildDialog
{
// Construction
public:
	CCMSectItemPSCOutter(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSCOutter)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_OUTTER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CTextUnit m_Units[12];
	CEditUnit m_Edits[12];
	double    m_Size [12];

	void *m_pSectData;
									//T_SECT_D  
	BOOL GetDlgData(void* pData);
	void SetCurDlgData(void * pData);
	void UpdateCtrls();
	void UpdateCurDatas();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCOutter)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSCOutter)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSCOUTTER_H__CDD38C7A_389E_492D_9D55_A474F81A0FF3__INCLUDED_)
