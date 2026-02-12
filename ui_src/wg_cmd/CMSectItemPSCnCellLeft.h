#if !defined(__CMSECTITEMPSCNCELLLEFT_H__)
#define __CMSECTITEMPSCNCELLLEFT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCnCellLeft.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCnCellLeft dialog
class CCMSectItemPSCnCell;
class CCMSectItemPSCnCellLeft : public CChildDialog
{
// Construction
public:
	CCMSectItemPSCnCellLeft(CWnd* pParent = NULL);   // standard constructor

public:
	BOOL GetDlgData(T_SECT_D* pData);
	void ChangeStatus(BOOL bSym, int nCellType, int CellNum, int nJoint, BOOL bSideHole);
	//void ChangeStatus(int nType, int nJoint, BOOL bHole, BOOL bSymmetric=FALSE);

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSCnCellLeft)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_NCELL_LEFT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCnCellLeft)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
		void SetData2Dlg();

protected:
	CCMSectItemPSCnCell* m_pParent;
	CEditUnit m_aCtrlEdit[29];
	CTextUnit m_aCtrlUnit[29];
	double    m_Size[29];

protected:

	void SetDlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSCnCellLeft)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeCmdEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTITEMPSCNCELLLEFT_H__)
