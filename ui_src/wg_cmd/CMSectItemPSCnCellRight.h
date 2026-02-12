#if !defined(__CMSECTITEMPSCNCELLRIGHT_H__)
#define __CMSECTITEMPSCNCELLRIGHT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCnCellRight.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCnCellRight dialog
class CCMSectItemPSCnCell;
class CCMSectItemPSCnCellRight : public CChildDialog
{
// Construction
public:
	CCMSectItemPSCnCellRight(CWnd* pParent = NULL);   // standard constructor

public:
	BOOL GetDlgData(T_SECT_D* pData);
	void ChangeStatus(BOOL bSym, int nCellType, int CellNum, int nJoint, BOOL bSideHole);
	//void ChangeStatus(int nType, int nJoint, BOOL bHole, BOOL bSymmetric);

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSCnCellRight)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_NCELL_RIGHT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCnCellRight)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CCMSectItemPSCnCell* m_pParent;
	CEditUnit m_aCtrlEdit[29];
	CTextUnit m_aCtrlUnit[29];
	double    m_Size[29];
public:
	void SetData2Dlg();

protected:
	
	void SetDlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSCnCellRight)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeCmdEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTITEMPSCNCELLRIGHT_H__)
