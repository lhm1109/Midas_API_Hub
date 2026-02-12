#if !defined(AFX_CMSECTITEMTAPOLD_H__673754E8_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSECTITEMTAPOLD_H__673754E8_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemTap.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "CMSectItemBase.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemTapOld dialog

class CCMSectItemTapOld : public CCMSectItemBase
{
// Construction
public:
	CCMSectItemTapOld(CWnd* pParent = NULL);   // standard constructor

//---------------------------------------------------
// Overrides
//---------------------------------------------------
public:
	virtual BOOL OnOKPublic();
	virtual BOOL OnApplyPublic();
	virtual void ShowCalcResult();

//---------------------------------------------------
// Interface
//---------------------------------------------------
public:

//---------------------------------------------------
// Implementation
//---------------------------------------------------
protected:
	BOOL SaveData();
	BOOL Dlg2Data();
	BOOL GetPeriAndStiff();
	void InitControls();
	void ShowKey2Dlg();
	void ShowData2Dlg();
	void SetSectionShapeCombo();
	void SetDBNameList();
	void SetNameCombo();
	void SetSectData();
	void ChangeBitmap();
	void DoActionWhenDBChanged();

	CString GetDB();
	void    SetDB(CString& csDBName);

public:
// Dialog Data
	//{{AFX_DATA(CCMSectItemTapOld)
	enum { IDD = IDD_CMD_SECT_ITEM_TAP_OLD };
	CComboBox	m_wndVar2;
	CComboBox	m_wndVar1;
	CComboBox	m_wndDB;
	CComboBox	m_wndOffset;
	CCobxAutoFill	m_wndSecondName;
	CCobxAutoFill	m_wndFirstName;
	CComboBoxEx	m_cboType;
	CEdit	m_wndName;
	CEdit	m_wndID;
	//}}AFX_DATA
	CBCGPStatic   m_wndFirstTitle[6];
	CEditUnit	m_wndFirstSize[6];
	CTextUnit m_wndFirstUnit[6];
	CBCGPStatic   m_wndSecondTitle[6];
	CEditUnit	m_wndSecondSize[6];
	CTextUnit m_wndSecondUnit[6];


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemTapOld)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT, UINT> m_aCtrlDB;

	// For changing section name by automatically
	BOOL    m_bAutoChangeName;

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemTapOld)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeShape();
	afx_msg void OnChangeFirstName();
	afx_msg void OnChangeSecondName();
	afx_msg void OnChangeDB();
	afx_msg void OnChangeFirstSize();
	afx_msg void OnChangeSecondSize();
	afx_msg void OnChangeUseDB();
	afx_msg void OnChangeVar1();
	afx_msg void OnChangeVar2();
	afx_msg void OnSetfocusCmdSpIdName();
	afx_msg void OnUpdateCmdSpIdName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMTAPOLD_H__673754E8_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
