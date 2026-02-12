#if !defined(AFX_CMSECTITEMVAL_H__673754E5_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSECTITEMVAL_H__673754E5_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemVal.h : header file
//

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "CMSectItemBase.h"
#include "CMSectItemGrid.h"

#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemVal dialog
using namespace mit::frx;

class CCMSectItemVal : public CCMSectItemBase
{
// Construction
public:
	CCMSectItemVal(CWnd* pParent = NULL);   // standard constructor
	~CCMSectItemVal();

//---------------------------------------------------
// Overrides
//---------------------------------------------------
public:
	virtual BOOL OnOKPublic();
	virtual BOOL OnApplyPublic();
	virtual void ShowCalcResult();
	virtual void DisplayOffsetPoint();
	virtual void SetNewID(T_SECT_K nNewID);

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
	void SetSectionATypeCombo();
	int  GetSectionAType();
	BOOL CheckData();
	void ChangeBitmap();
	void SetBuiltUp();

	int GetCurShapeIndex();
	void SetCurShapeByIndex(int nShapeIndex);
	void ResizeDialog();
	void MoveItems();

protected:
	CCMSectItemGrid m_wndGridFirst;

	CArray<UINT, UINT> m_aCtrlROct;
	CArray<UINT, UINT> m_aCtrlGS;
	CArray<UINT, UINT> m_aCtrlOption;
	CArray<UINT, UINT> m_aCtrlAType;
	
	CArray<UINT, UINT>          m_aImportID;
	CArray<T_SECT_D, T_SECT_D&> m_aImportSect;
	UINT      m_CurSectID;

public:
// Dialog Data
	//{{AFX_DATA(CCMSectItemVal)
	enum { IDD = IDD_CMD_SECT_ITEM_VAL };
	CBCGPStatic	m_txtOffset;
	CFormulaEditSpin	m_editCellNum;
	MButton	m_wndBuiltUp;
	MComboBox m_cboType;
	MEdit	m_wndName;
	MComboBox m_cbxAType;
	MEdit	m_wndID;
	MButton m_chkShearDeform;
	MButton m_chkWarpingEffect;
	MEdit	    m_editPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemVal)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemVal)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeShape();
	afx_msg void OnChkWarpingEffect();
	afx_msg void OnChangeID();
	afx_msg void OnChangeName();
	afx_msg void OnChangeBuiltUp();
	afx_msg void OnSetfocusCmdSpIdName();
	afx_msg void OnCmdSectCentroidBtn();
	afx_msg void OnChangeCmdEditCellNum();
// 	afx_msg void OnDeltaposCmdSpinCellNum(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdChangeOffsetBtn();
	afx_msg void OnCmdImportBtn();
	afx_msg void OnCmdSelectBtn();
	afx_msg void OnCmdFemRdo();
	afx_msg void OnChangeSelAType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMVAL_H__673754E5_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
