#if !defined(AFX_CMSECTITEMPSCINNER_H__004C97E4_907D_4C46_B054_1609E19C05F3__INCLUDED_)
#define AFX_CMSECTITEMPSCINNER_H__004C97E4_907D_4C46_B054_1609E19C05F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCInner.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCInner dialog
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\wg_base_ChildDialog.h"


class CCMSectItemPSCInner : public CChildDialog
{
// Construction
public:
	CCMSectItemPSCInner(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSCInner)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_INNER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CTextUnit m_Units[18];
	CEditUnit m_Edits[18];
	double    m_Size [18];

	void *m_pSectData;
								 //T_SECT_D  
	BOOL GetDlgData(void* pData);
	void SetCurDlgData(void * pData);
	void UpdateCtrls();
	void UpdateCurDatas();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCInner)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSCInner)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSCINNER_H__004C97E4_907D_4C46_B054_1609E19C05F3__INCLUDED_)
