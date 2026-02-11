#if !defined(AFX_CMSECTITEMCFM_H__673754E9_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSECTITEMCFM_H__673754E9_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemCfm.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "CMSectItemBase.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemCfm dialog

class CCMSectItemCfm : public CCMSectItemBase
{
// Construction
public:
	CCMSectItemCfm(CWnd* pParent = NULL);   // standard constructor
	~CCMSectItemCfm();

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
	BOOL SaveData();
	BOOL Dlg2Data();
	BOOL GetPeriAndStiff();

	void InitControls();
	void ShowKey2Dlg();
	void ShowData2Dlg();
	void SetSectionShapeCombo();
	void SetDBNameList();
	void SetFirstNameCombo();
	void SetFirstSectData();
	void ChangeBitmap();
	void SetBuiltUp();
	void DoActionWhenDBChanged();

	CString GetDB();
	void    SetDB(CString& csDBName);


//---------------------------------------------------
// Implementation
//---------------------------------------------------
protected:

public:
// Dialog Data
	//{{AFX_DATA(CCMSectItemCfm)
	enum { IDD = IDD_CMD_SECT_ITEM_CFM };
	CBCGPStatic	m_txtOffset;
	CButton	m_wndBuiltUp;
	CCobxAutoFill	m_wndFirstName;
	CComboBox	m_wndDB;
	CComboBoxEx	m_cboType;
	CEdit	m_wndName;
	CEdit	m_wndID;
	CButton m_chkShearDeform;
	CButton m_chkWarpingEffect;
	//}}AFX_DATA
	CBCGPStatic   m_wndFirstTitle[5];
	CEditUnit	m_wndFirstSize[5];
	CTextUnit m_wndFirstUnit[5];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemCfm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// For changing section name by automatically
	BOOL    m_bAutoChangeName;

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemCfm)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeShape();
	afx_msg void OnChangeDB();
	afx_msg void OnChangeFirstName();
	afx_msg void OnChangeDblDB();
	afx_msg void OnChangeDblSectName();
	afx_msg void OnChangeFirstSize();
	afx_msg void OnChangeBuiltUp();
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

#endif // !defined(AFX_CMSECTITEMCFM_H__673754E9_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
