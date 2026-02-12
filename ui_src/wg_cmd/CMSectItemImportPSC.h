#if !defined(AFX_CMSECTITEMIMPORTPSC_H__4E7215F9_F48F_4287_8F82_E8DEF2843652__INCLUDED_)
#define AFX_CMSECTITEMIMPORTPSC_H__4E7215F9_F48F_4287_8F82_E8DEF2843652__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemImportPSC.h : header file
//

#include "CMSecViewWnd.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemImportPSC dialog

class CCMSectItemImportPSC : public CDialogMove
{
// Construction
public:
	CCMSectItemImportPSC(CWnd* pParent = NULL);   // standard constructor
	
public:
	T_SECT_K m_SectKey;
	BOOL m_bIncludeOption[2]; // (0) Joint, Type, Shape
														// (1) Shear Check, Min. Web Thick                            

// Dialog Data
	//{{AFX_DATA(CCMSectItemImportPSC)
	enum { IDD = IDD_CMD_SECT_ITEM_TAP_PSCIMPORT };
	CListBox	m_ltbxSection;
	MButton	m_chkIncludeOption;
	MButton	m_chkIncludeOption2;
	//}}AFX_DATA

public:
	void SetTargetShape(CString& rcsShape) { m_csTargetShape = rcsShape; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemImportPSC)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_csTargetShape;
	CCMSecViewWnd m_wndSecView; // variable for section preview
	T_SECT_D m_Data;
	BOOL m_bSetCellTypeShape;

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemImportPSC)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdImport();
	afx_msg void OnCancel();
	afx_msg void OnSelchangeCmdListSection();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMIMPORTPSC_H__4E7215F9_F48F_4287_8F82_E8DEF2843652__INCLUDED_)
