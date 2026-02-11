#if !defined(AFX_CMSECTITEMSRC_H__673754E6_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSECTITEMSRC_H__673754E6_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemSrc.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "CMSectItemBase.h"
#include "..\mit_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\MIT_frx\MComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemSrc dialog
using namespace mit::frx;
class CCMSectItemSrc : public CCMSectItemBase
{
// Construction
public:
	CCMSectItemSrc(CWnd* pParent = NULL);   // standard constructor
	~CCMSectItemSrc();

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
	void ChangeMatlData(T_MATL_CONCRETE &ConcrData, T_MATL_STEEL &SteelData);
	BOOL GetSectData(T_SECT_D& sect_data); //add hjs 03/03/12

//---------------------------------------------------
// Implementation
//---------------------------------------------------
protected:
	BOOL SaveData();
	BOOL Dlg2Data();
	BOOL GetPeriAndStiff();
	BOOL SetErr(LPCTSTR msg);
	void AlignControls();
	void InitControls();
	void ShowKey2Dlg();
	void ShowData2Dlg();
	void SetSectionShapeCombo();
	void SetDBNameList();
	void SetSteelNameCombo();
	void SetSteelSectData();
	void SetSteelFirstNameCombo();
	void SetSteelSecondNameCombo();
	void SetSteelFirstSectData();
	void SetSteelSecondSectData();  
	void SetSteel3SectData();
	void SetSteel4SectData();
	void SetConcrSectData();
	void ChangeBitmap();
	void DoActionWhenDBChanged();

	CString GetDB();
	void    SetDB(CString& csDBName);

	void SetMatlSectData();

public:
// Dialog Data
	//{{AFX_DATA(CCMSectItemSrc)
	enum { IDD = IDD_CMD_SECT_ITEM_SRC };
	CFormulaEditSpin	m_editN2;
	CFormulaEditSpin	m_editN1;
	CBCGPStatic	m_txtOffset;
	MButton	m_btnOffset;
	CEditUnit	m_wndMStiff;	
	CCobxAutoFill	m_wndSteel2Name2;
	CCobxAutoFill	m_wndSteel2Name1;
	MButton	m_wndBuiltUp;
	CEditUnit	m_wndPc;
	CEditUnit	m_wndPs;
	CEditUnit	m_wndDsc;
	CEditUnit	m_wndEsc;
	CCobxAutoFill	m_wndSteelName;
	MComboBox	m_wndDB;
	MComboBox	m_cboType;
	MEdit	m_wndName;
	MEdit	m_wndID;
	MButton m_chkShearDeform;
	MButton m_chkWarpingEffect;
	MButton m_chkInfusionConc;
	//}}AFX_DATA
	CBCGPStatic   m_wndConcrTitle[2];
	CEditUnit	m_wndConcrSize[2];
	CTextUnit m_wndConcrUnit[2];
	CBCGPStatic   m_wndSteelTitle[6];
	CEditUnit	m_wndSteelSize[6];
	CTextUnit m_wndSteelUnit[6];
	CBCGPStatic   m_wndSteel2Title[8];
	CEditUnit	m_wndSteel2Size[8];
	CTextUnit m_wndSteel2Unit;
	CBCGPStatic   m_wndSteel3Title[10];
	CEditUnit m_wndSteel3Size[10];
	CTextUnit m_wndSteel3Unit[10];
	CBCGPStatic   m_wndSteel4Title[7];
	CEditUnit     m_wndSteel4Size[7];
	CTextUnit     m_wndSteel4Unit[7];


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemSrc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT, UINT> m_aCtrlDB;
	CArray<UINT, UINT> m_aCtrlReplace;
	CArray<UINT, UINT> m_aCtrlType0;
	CArray<UINT, UINT> m_aCtrlType1;
	CArray<UINT, UINT> m_aCtrlType2;
	CArray<UINT, UINT> m_aCtrlType3;
	CArray<UINT, UINT> m_aCtrlType4; // Dumbbell

	int  m_nCurShapeType;
	int  GetShapeType(CString& csShape);
	void ChangeShapeType(int nShapeType);

	BOOL m_bDumb;

	// For changing section name by automatically
	BOOL    m_bAutoChangeName;

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemSrc)
	afx_msg void OnChangeShape();
	afx_msg void OnChangeDB();
	afx_msg void OnChangeSteelName();
	afx_msg void OnChangeUseDB();
	afx_msg void OnChangeBuiltUp();
	afx_msg void OnChangeConcrSize();
	afx_msg void OnChangeMatlValue();
	afx_msg void OnChangeSteelSize();
	afx_msg void OnChangeSteelSize2();
	afx_msg void OnChangeSteelSize3();
	afx_msg void OnChangeSteelSize4();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeReplace();
	afx_msg void OnCmdInfusionCheck();
	afx_msg void OnChangeID();
	afx_msg void OnChangeName();
	afx_msg void OnClickBtnSelectMatlFromDB();
	afx_msg void OnSetfocusCmdSpIdName();
	afx_msg void OnUpdateCmdSpIdName();
	afx_msg void OnChangeSteel2Name();
	afx_msg void OnChangeSteel2Name2();
	afx_msg void OnCmdSectCentroidBtn();
	afx_msg void OnCmdChangeOffsetBtn();
// 	afx_msg void OnDeltaposCmdSpId3Spin1(NMHDR* pNMHDR, LRESULT* pResult);
// 	afx_msg void OnDeltaposCmdSpId3Spin2(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMSRC_H__673754E6_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
