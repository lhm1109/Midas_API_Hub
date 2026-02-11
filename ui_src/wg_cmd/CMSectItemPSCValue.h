#if !defined(__CMSECTITEMPSCVALUE_H__)
#define __CMSECTITEMPSCVALUE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSC_I.h : header file
//
#include "CMSectItemPSCBase.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MenuBtnEx.h"

#include "CMSectItemStiffGrid.h"

#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCValue dialog
class CCMSectItemPSC;
class CCMSectItmePSCView;
class CCMSectItemPSCValue : public CCMSectItemPSCBase
{
// Construction
public:
	CCMSectItemPSCValue(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSectItemPSCValue();

	CCMSectItmePSCView* GetViewPointer();

public:
	virtual BOOL GetDlgData(void* pData);
	virtual void SetCurDlgData(void* pData, BOOL bModify);
	virtual void DisplayOffsetPoint();
	virtual void SetAutotShearCheckPos(double dShearPos[3]);
	virtual void SetAutoMinWebThick(double dShear[3], double dTorsion);
	virtual void SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6]);

	virtual void OnAfterCalcPeriAndStiff(T_SECT_SECTION_D* pSectBefore);
	void OnCalculateButton();
	void OnDisplayCentroldButton();
	void ExternalShowData();


// Dialog Data
	//{{AFX_DATA(CCMSectItemPSCValue)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_VALUE };
	mit::frx::MButton	  m_wndZ3Thk;
	CTextUnit	m_unitZ3Thk;
	CEditUnit	m_editZ3Thk;
	mit::frx::MButton	  m_wndZ3Qy;
	CTextUnit	m_unitZ3Qy;
	CEditUnit	m_editZ3Qy;
	CTextUnit	m_unitZ3Pos;
	CEditUnit	m_editZ3Pos;
	mit::frx::MButton	  m_wndZ2Thk;
	CTextUnit	m_unitZ2Thk;
	CEditUnit	m_editZ2Thk;
	mit::frx::MButton	  m_wndZ2Qy;
	CTextUnit	m_unitZ2Qy;
	CEditUnit	m_editZ2Qy;
	mit::frx::MButton	  m_wndZ1Thk;
	CTextUnit	m_unitZ1Thk;
	CEditUnit	m_editZ1Thk;
	mit::frx::MButton	  m_wndZ1Qy;
	CTextUnit	m_unitZ1Qy;
	CEditUnit	m_editZ1Qy;
	CTextUnit	m_unitZ1Pos;
	CEditUnit	m_editZ1Pos;
	mit::frx::MButton	  m_wndTorsion;
	CTextUnit	m_unitTorsion;
	CEditUnit	m_editTorsion;
	mit::frx::MButton	  m_wndShearDeform;
	mit::frx::MButton m_chkWarpingEffect;
	CTextUnit	m_unitHT;
	CEditUnit	m_editHT;
	CTextUnit	m_unitBT;
	CEditUnit	m_editBT;
	CTextUnit	m_unitT2;
	CEditUnit	m_editT2;
	CTextUnit	m_unitT1;
	CEditUnit	m_editT1;
	mit::frx::MEdit	    m_editPath;
	CBCGPStatic   m_txtOffset;
	//}}AFX_DATA
	mit::frx::CMenuBtnEx m_btnImport;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCValue)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitControls();
	void ShowData();

	void ResetData();
	BOOL ChangeBitmap(int nIndex);
	void SaveDataForDrawSection();
	void RedrawSection();
	void OnCmdSectWapingCheck();

protected:
	BOOL      m_bModify;
	T_SECT_D  m_SectData;
	UINT      m_CurSectID;
	int   m_nWarpingCheck;

	CCMSectItemPSC* m_pParent;
	CCMSectItemStiffGrid m_wndGridStiff;

	CArray<UINT, UINT>          m_aImportID;
	CArray<T_SECT_D, T_SECT_D&> m_aImportSect;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSCValue)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSectCentroidBtn();
	afx_msg void OnCmdChangeOffsetBtn(); 
	afx_msg void OnCmdImportBtn();
	afx_msg void OnCmdDefineBtn();
	afx_msg void OnCmdShear1QyAuto();
	afx_msg void OnCmdShear2QyAuto();
	afx_msg void OnCmdShear3QyAuto();
	afx_msg void OnCmdShear1ThkAuto();
	afx_msg void OnCmdShear2ThkAuto();
	afx_msg void OnCmdShear3ThkAuto();
	afx_msg void OnCmdSelectBtn();
	afx_msg void OnChangeCmdShearEdit();
	afx_msg void OnChkWarpingEffect();
	afx_msg void OnCmdSectWapingCheckBtn();
	afx_msg void OnCmdSectWapingCheckUser();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTITEMPSCVALUE_H__)
