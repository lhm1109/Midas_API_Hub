#if !defined(AFX_CMSECTITEMPSC3CELLDEF_H__DFEB6853_D8D3_4738_A2E2_3E75D876B6EA__INCLUDED_)
#define AFX_CMSECTITEMPSC3CELLDEF_H__DFEB6853_D8D3_4738_A2E2_3E75D876B6EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSC3CellDef.h : header file
//

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC3CellDef dialog
class CCMSectItemPSC3Cell;
class CCMSectItemPSC3CellDef : public CChildDialog
{
// Construction
public:
	CCMSectItemPSC3CellDef(CWnd* pParent = NULL);   // standard constructor
	BOOL GetDlgData(T_SECT_D* pData);
	
// Dialog Data
	//{{AFX_DATA(CCMSectItemPSC3CellDef)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_3CELL_DEF };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSC3CellDef)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCMSectItemPSC3Cell* m_pParent;
	CTextUnit m_Units[22];
	CEditUnit m_Edits[22];
	double    m_Size [22];

public:
	void SetData2Dlg();
	void SetDlg2Data();

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSC3CellDef)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeCmdEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSC3CELLDEF_H__DFEB6853_D8D3_4738_A2E2_3E75D876B6EA__INCLUDED_)
