#if !defined(AFX_CMSECTITEMIMPORTREG_H__0524DF48_BD83_415C_95FC_27E3B90BBD27__INCLUDED_)
#define AFX_CMSECTITEMIMPORTREG_H__0524DF48_BD83_415C_95FC_27E3B90BBD27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemImportReg.h : header file
//
#include "CMSecViewWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemImportReg dialog

class CCMSectItemImportReg : public CDialogMove
{
// Construction
public:
	CCMSectItemImportReg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMSectItemImportReg)
	enum { IDD = IDD_CMD_SECT_ITEM_TAP_REGIMPORT };
	CListBox	m_ltbxSection;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemImportReg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCMSecViewWnd m_wndSecView; // variable for section preview
	T_SECT_D m_Data;
	// Generated message map functions
	//{{AFX_MSG(CCMSectItemImportReg)
	afx_msg void OnCmdImport();
	afx_msg void OnSelchangeCmdListSection();
	afx_msg void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	T_SECT_K m_SectKey;
public:
	void SetTargetShape(CString& rcsShape) { m_csTargetShape = rcsShape; }
private:
	CString m_csTargetShape;
 

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMIMPORTREG_H__0524DF48_BD83_415C_95FC_27E3B90BBD27__INCLUDED_)
