#if !defined(AFX_CMSECTITEMCOM_H__673754E7_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSECTITEMCOM_H__673754E7_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemCom.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "CMSectItemBase.h"

#include "..\mit_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\mit_frx\MComboBox.h"


/////////////////////////////////////////////////////////////////////////////
// CCMSectItemCom dialog

class CCMSectItemCom : public CCMSectItemBase
{
// Construction
public:
	CCMSectItemCom(CWnd* pParent = NULL);   // standard constructor
	~CCMSectItemCom();

//---------------------------------------------------
// Overrides
//---------------------------------------------------
public:
	BOOL OnOKPublic() override;
	BOOL OnApplyPublic() override;
	void ShowCalcResult() override;
	void DisplayOffsetPoint() override;
	void SetNewID(T_SECT_K nNewID) override;

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
	void SetFirstNameCombo();
	void SetSecondNameCombo();
	void SetFirstSectData();
	void SetSecondSectData();
	void ChangeBitmap();
	void DoActionWhenDBChanged();

	CString GetDB();
	void    SetDB(CString& csDBName);


public:
// Dialog Data
	//{{AFX_DATA(CCMSectItemCom)
	enum { IDD = IDD_CMD_SECT_ITEM_COM };
	CBCGPStatic	m_txtOffset;
	MComboBox	m_wndDB;
	CCobxAutoFill	m_wndSecondName;
	CCobxAutoFill	m_wndFirstName;
	MComboBox	m_cboType;
	MEdit	m_wndName;
	MEdit	m_wndID;
	MButton m_chkShearDeform;
	MButton m_chkWarpingEffect;
	//}}AFX_DATA
	CBCGPStatic   m_wndFirstTitle[5];
	CEditUnit	m_wndFirstSize[5];
	CTextUnit m_wndFirstUnit[5];
	CBCGPStatic   m_wndSecondTitle[4];
	CEditUnit	m_wndSecondSize[4];
	CTextUnit m_wndSecondUnit[4];


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemCom)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT, UINT> m_aCtrlDB;

	// For changing section name by automatically
	BOOL    m_bAutoChangeName;

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemCom)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeShape();
	afx_msg void OnChangeFirstName();
	afx_msg void OnChangeSecondName();
	afx_msg void OnChangeDB();
	afx_msg void OnChangeFirstSize();
	afx_msg void OnChangeSecondSize();
	afx_msg void OnChangeUseDB();
	afx_msg void OnSetfocusCmdSpIdName();
	afx_msg void OnUpdateCmdSpIdName();
	afx_msg void OnCmdSectCentroidBtn();
	afx_msg void OnCmdChangeOffsetBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMCOM_H__673754E7_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
