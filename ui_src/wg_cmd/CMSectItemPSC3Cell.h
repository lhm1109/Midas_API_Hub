#if !defined(AFX_CMSECTITEMPSC3CELL_H__6DC56141_CF30_4BD6_B4AA_C641D32791D0__INCLUDED_)
#define AFX_CMSECTITEMPSC3CELL_H__6DC56141_CF30_4BD6_B4AA_C641D32791D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSC3Cell.h : header file
//
#include "CMSectItemPSCBase.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC3Cell dialog
class CCMSectItemPSC;
class CCMSectItemPSC3CellDef;
class CCMSectItemPSC3CellOpt;

class CCMSectItemPSC3Cell : public CCMSectItemPSCBase
{
// Construction
public:
	CCMSectItemPSC3Cell(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSectItemPSC3Cell();   
	
// Interface
public:
	virtual BOOL GetDlgData(void *pData);
	virtual void SetCurDlgData(void *pData, BOOL bModify);
	virtual void DisplayOffsetPoint();
	virtual void SetAutotShearCheckPos(double dShearPos[3]);
	virtual void SetAutoMinWebThick(double dShear[3], double dTorsion);
	virtual void SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6]);

	int GetSelectedTab();
	void UpdateCtrls();

	void RedrawSection();
	T_SECT_D* GetDataPointer();

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSC3Cell)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_3CELL };
	mit::frx::MButton	m_chkWebI;
	mit::frx::MButton	m_chkShear3;
	mit::frx::MButton	m_chkShear1;
	CTextUnit	m_unitWebI;
	CEditUnit	m_editWebI;
	CTextUnit	m_unitShear3;
	CEditUnit	m_editShear3;
	CTextUnit	m_unitShear2;
	CEditUnit	m_editShear2;
	CTextUnit	m_unitShear1;
	CEditUnit	m_editShear1;
	mit::frx::MButton	m_chkShearCheck;
	mit::frx::MButton   m_chkShearDeform;
	mit::frx::MButton   m_chkWarpingEffect;
	CBCGPStatic	m_txtOffset;
	CDlgTabCtrl	m_ctrlTab;
	BOOL	m_bJI1;
	BOOL	m_bJI10;
	BOOL	m_bJI11;
	BOOL	m_bJI2;
	BOOL	m_bJI3;
	BOOL	m_bJI4;
	BOOL	m_bJI5;
	BOOL	m_bJI6;
	BOOL	m_bJI7;
	BOOL	m_bJI8;
	BOOL	m_bJI9;
	BOOL	m_bJO1;
	BOOL	m_bJO2;
	//}}AFX_DATA
	CTextUnit	m_unitWebShear[3];
	CEditUnit	m_editWebShear[3];
	mit::frx::MButton	  m_chkWebShear[3];
	int   m_nWarpingCheck;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSC3Cell)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bModify;
	T_SECT_D m_SectData;
	CCMSectItemPSC*         m_pParent;
	CCMSectItemPSC3CellDef* m_pDefDlg;
	CCMSectItemPSC3CellOpt* m_pOptDlg;

protected:
	void SetDlg2Data();
	
	void InitCtrlsData();
	void LayoutChildDlgs(void);
	BOOL ChangeBitmap(int nIndex);
	void SetDataSource(T_SECT_D* pDataSrc);
public:
	void SetData2Dlg();
	void ChangeTab(int index);

protected:
	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSC3Cell)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdCheckCtrl();
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdSectCentroidBtn();
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

#endif // !defined(AFX_CMSECTITEMPSC3CELL_H__6DC56141_CF30_4BD6_B4AA_C641D32791D0__INCLUDED_)
