#if !defined(AFX_CMSECTITEMPSC3CELLOPT_H__B4F21B36_49EC_44B0_9B89_3D156E0FA0F3__INCLUDED_)
#define AFX_CMSECTITEMPSC3CELLOPT_H__B4F21B36_49EC_44B0_9B89_3D156E0FA0F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSC3CellOpt.h : header file
//
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC3CellOpt dialog
class CCMSectItemPSC3Cell;
class CCMSectItemPSC3CellOpt : public CChildDialog
{
// Construction
public:
	CCMSectItemPSC3CellOpt(CWnd* pParent = NULL);   // standard constructor

public:
	BOOL GetDlgData(void* pData);
	void UpdateCtrls();

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSC3CellOpt)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_3CELL_OPT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSC3CellOpt)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public: 
	void SetData2Dlg();
	void SetDlg2Data();

// Implementation
protected:
	CCMSectItemPSC3Cell* m_pParent;
	CTextUnit m_Units[26];
	CEditUnit m_Edits[26];
	double    m_Size [26];
 
	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSC3CellOpt)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeCmdEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSC3CELLOPT_H__B4F21B36_49EC_44B0_9B89_3D156E0FA0F3__INCLUDED_)
