#if !defined(__CMSECTOFFSETDLG_H__)
#define __CMSECTOFFSETDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectOffsetDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectOffsetDlg dialog
using namespace mit::frx;
class CCMSectItemBase;
class CCMSectOffsetDlg : public CDialogMove
{
// Construction
public:
	CCMSectOffsetDlg(T_SECT_SECTION_D* pData, BOOL bTapered=FALSE, CWnd* pParent = NULL); 

// Dialog Data
	//{{AFX_DATA(CCMSectOffsetDlg)
	enum { IDD = IDD_CMD_SECT_OFFSET_DLG };
	CTextUnit	m_unitVJ;
	CEditUnit	m_editVJ;
	CTextUnit	m_unitVI;
	CEditUnit	m_editVI;
	MComboBox	m_wndOffset;
	CTextUnit	m_unitHJ;
	CEditUnit	m_editHJ;
	CTextUnit	m_unitHI;
	CEditUnit	m_editHI;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectOffsetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CCMSectItemBase* m_pParent;
	T_SECT_SECTION_D* m_pData;
	BOOL m_bTapered;

	CArray<UINT, UINT> m_aCtrlCenter;
	CArray<UINT, UINT> m_aCtrlVertical;
	CArray<UINT, UINT> m_aCtrlHorizont;
	CArray<UINT, UINT> m_aCtrlUserOffsetRef;

protected:
	void InitOffsetCombo();
	void Data2Dlg();
	BOOL Dlg2Data();
	void EnableDisableCenterIs();
	void EnableDisableUserOffsetRef();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectOffsetDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdVerticalRadio();
	afx_msg void OnCmdHorizontalRadio();
	afx_msg void OnSelchangeCmdOffsetCb();
	afx_msg void OnCmdOffsetBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTOFFSETDLG_H__)
