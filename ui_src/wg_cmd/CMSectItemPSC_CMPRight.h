#if !defined(__CMSECTITEMPSC_CMPRIGHT_H__)
#define __CMSECTITEMPSC_CMPRIGHT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSC_CMPRight.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

class CCMSectItemPSC_CMP;

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC_CMPRight dialog
class CCMSectItemPSC_CMPRight : public CChildDialog
{
// Construction
public:
	CCMSectItemPSC_CMPRight(CWnd* pParent = NULL);   // standard constructor

public:
	BOOL GetDlgData(T_SECT_D* pData);
	void ChangeStatus(BOOL bSymmetric, BOOL bHunch);

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSC_CMPRight)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_CMP_RIGHT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSC_CMPRight)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CCMSectItemPSC_CMP* m_pParent;
	CEditUnit m_aCtrlEdit[32];
	CTextUnit m_aCtrlUnit[32];
	double    m_Size[32];

protected:
	void InitControls();
public:
	void SetData2Dlg();
	void SetDlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSC_CMPRight)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeCmdEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTITEMPSC_CMPRIGHT_H__)
