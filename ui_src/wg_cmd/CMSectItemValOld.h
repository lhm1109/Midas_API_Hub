#if !defined(AFX_CMSECTITEMVALOLD_H__673754E5_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSECTITEMVALOLD_H__673754E5_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemVal.h : header file
//

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "CMSectItemBase.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemValOld dialog

class CCMSectItemValOld : public CCMSectItemBase
{
// Construction
public:
	CCMSectItemValOld(CWnd* pParent = NULL);   // standard constructor

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
	void SetFirstSectData();
	void ChangeBitmap();
	void SetBuiltUp();

public:
// Dialog Data
	//{{AFX_DATA(CCMSectItemValOld)
	enum { IDD = IDD_CMD_SECT_ITEM_VAL_OLD };
	CComboBox	m_wndOffset;
	CButton	m_wndBuiltUp;
	CComboBoxEx	m_cboType;
	CEdit	m_wndName;
	CEdit	m_wndID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemValOld)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CBCGPStatic   m_wndFirstTitle[6];
	CEditUnit	m_wndFirstSize[6];
	CTextUnit m_wndFirstUnit[6];

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemValOld)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeShape();
	afx_msg void OnChangeFirstSize();
	afx_msg void OnChangeID();
	afx_msg void OnChangeName();
	afx_msg void OnChangeBuiltUp();
	afx_msg void OnClickBtnStiffness();
	afx_msg void OnSetfocusCmdSpIdName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMVALOLD_H__673754E5_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
