#if !defined(__CMSectItemImportStlg_H__)
#define __CMSectItemImportStlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemImportStlg.h : header file
//

#include "CMSecViewWnd.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemImportStlg dialog

class CCMSectItemImportStlg : public CDialogMove
{
// Construction
public:
	CCMSectItemImportStlg(CWnd* pParent = NULL);   // standard constructor
	
public:
	T_SECT_K m_SectKey;
	BOOL m_bIncludeOption[1]; // (0) Joint, Type, Shape
														// (1) Shear Check, Min. Web Thick                            
	int m_nFlangeShape;
	int m_nCellNum;
	int m_nCellType;

// Dialog Data
	//{{AFX_DATA(CCMSectItemImportStlg)
	enum { IDD = IDD_CMD_SECT_ITEM_TAP_STLGIMPORT };
	CListBox	m_ltbxSection;
	MButton	m_chkIncludeOption;
	//}}AFX_DATA

public:
	void SetTargetShape(CString& rcsShape) { m_csTargetShape = rcsShape; EnableDisableOption();}


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemImportStlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_csTargetShape;
	CCMSecViewWnd m_wndSecView; // variable for section preview
	T_SECT_D m_Data;
	BOOL m_bSetCellTypeShape;
	BOOL m_bInitialized;
	void EnableDisableOption();

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemImportStlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdImport();
	afx_msg void OnCancel();
	afx_msg void OnSelchangeCmdListSection();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSectItemImportStlg_H__)
