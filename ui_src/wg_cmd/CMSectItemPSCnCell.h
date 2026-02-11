#if !defined(__CMSECTITEMPSCNCELL_H__)
#define __CMSECTITEMPSCNCELL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCnCell.h : header file
//
#include "CMSectItemPSCBase.h"
#include "CMSectItemPSCnCellGridDlg.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCnCell dialog
class CCMSectItemPSC;
class CCMSectItemPSCnCellLeft;
class CCMSectItemPSCnCellRight;

class CCMSectItemPSCnCell : public CCMSectItemPSCBase
{
// Construction
public:
	CCMSectItemPSCnCell(CWnd* pParent = NULL);   // standard constructor

public:
	virtual BOOL GetDlgData(void *pData);
	virtual void SetCurDlgData(void *pData, BOOL bModify);
	virtual void DisplayOffsetPoint();
	virtual void SetAutotShearCheckPos(double dShearPos[3]);
	virtual void SetAutoMinWebThick(double dShear[3], double dTorsion);
	virtual void SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6]);

	void RedrawSection();
	T_SECT_D* GetDataPointer() { return (&m_SectData); } 

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSCnCell)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_NCELL };
	mit::frx::MButton	m_chkWebI;
	mit::frx::MButton	m_chkShear3;
	mit::frx::MButton	m_chkShear1;
	CBCGPStatic	m_txtOffset;
	CTextUnit	m_unitWebI;
	CEditUnit	m_editWebI;
	CTextUnit	m_unitShear3;
	CEditUnit	m_editShear3;
	CTextUnit	m_unitShear2;
	CEditUnit	m_editShear2;
	CTextUnit	m_unitShear1;
	CEditUnit	m_editShear1;
	CTextUnit	m_unitWidth;
	CEditUnit	m_editWidth;
	mit::frx::MButton	m_chkShearCheck;
	mit::frx::MButton m_chkShearDeform;
	mit::frx::MButton m_chkWarpingEffect;
	CFormulaEditSpin	m_editCellNumSpin;
	mit::frx::MButton	m_chkJO;
	mit::frx::MButton	m_chkJI;
	mit::frx::MButton	m_chkSymmetic;
	mit::frx::MButton	m_chkHole;
	CDlgTabCtrl	m_ctrlTab;
	//}}AFX_DATA
	CTextUnit	m_unitWebShear[3];
	CEditUnit	m_editWebShear[3];
	mit::frx::MButton	  m_chkWebShear[3];
	int   m_nWarpingCheck;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCnCell)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	void ChangeTab(int index);
protected:
	void LayoutChildDlgs();

	BOOL ChangeBitmap();
	void SetDataSource(T_SECT_D* pDataSrc);

	void SetDlg2Data();
	void ChangeStatus();
	void ChangeShearCheck(int nCheck);
	void ChangeAutoShearCheck1();
	void ChangeAutoShearCheck3();
	void SaveSlabWidth(T_SECT_D* pSectData, BOOL bEnableCtrl);

protected:
	BOOL m_bModify;
	T_SECT_D m_SectData;
	CCMSectItemPSC*           m_pParent;
	CCMSectItemPSCnCellLeft*  m_pLeftDlg;
	CCMSectItemPSCnCellRight* m_pRightDlg;

	CArray<UINT, UINT> m_aCtrlCellType;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSCnCell)
	virtual BOOL OnInitDialog();
	//afx_msg void OnDeltaposCmdCellNumSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdCellTypeRadio();
	afx_msg void OnCmdJointCheck();
	afx_msg void OnCmdSectCentroidBtn();
	afx_msg void OnCmdSideHoleCheck();
	afx_msg void OnCmdSymmetricCheck();
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeCmdSlabWidthEdit();
	afx_msg void OnChangeCmdCellNumEdit();
	afx_msg void OnCmdChangeOffsetBtn();
	afx_msg void OnChangeCmdShearEdit();
	afx_msg void OnCmdShearCheck();
	afx_msg void OnCmdShear1Check();
	afx_msg void OnCmdShear3Check();
	afx_msg void OnCmdWebICheck();
	afx_msg void OnCmdWebICheck2();
	afx_msg void OnCmdSectTableinputBtn();
	afx_msg void OnCmdSectWapingCheckBtn();
	afx_msg void OnCmdSectWapingCheckUser();
	afx_msg void OnCmdSectWapingCheck();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTITEMPSCNCELL_H__)
