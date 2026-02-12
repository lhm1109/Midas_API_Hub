#if !defined(__CMSECTITEMPSC_CMP_H__)
#define __CMSECTITEMPSC_CMP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSC_CMP.h : header file
//
#include "CMSectItemPSCBase.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MEdit.h"

class CCMSectItemPSC;
class CCMSectItemPSC_CMPLeft;
class CCMSectItemPSC_CMPRight;

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC_CMP dialog
class CCMSectItemPSC_CMP : public CCMSectItemPSCBase
{
// Construction
public:
	CCMSectItemPSC_CMP(CWnd* pParent = NULL);   // standard constructor

public:
	virtual BOOL GetDlgData(void *pData);
	virtual void SetCurDlgData(void *pData, BOOL bModify);
	virtual void DisplayOffsetPoint();
	virtual void SetAutotShearCheckPos(double dShearPos[3]);
	virtual void SetAutoMinWebThick(double dShear[3], double dTorsion) {}
	virtual void SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6]);
	
	void ChangeMatlData(T_MATL_CONCRETE &ConcrData, T_MATL_STEEL &SteelData);
	void RedrawSection();
	int  GetSelectedTab();
	void ChangeTab(int index);
	T_SECT_D* GetDataPointer() { return (&m_SectData); } 

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSC_CMP)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_CMP };
	CEditUnit m_editEFD;
	CEditUnit m_editLRF;
	CEditUnit m_editMValue4;
	CEditUnit m_editMValue3;
	CEditUnit m_editMValue2;
	CEditUnit m_editMValue1;
	CTextUnit m_unitCMPt;
	CEditUnit m_editCMPt;
	CTextUnit m_unitCMPh;
	CEditUnit m_editCMPh;
	CTextUnit m_unitCMPb;
	CEditUnit m_editCMPb;
	CTextUnit m_unitCMPa;
	CEditUnit m_editCMPa;
	CTextUnit	m_unitShear3;
	CEditUnit	m_editShear3;
	CTextUnit	m_unitShear2;
	CEditUnit	m_editShear2;
	CTextUnit	m_unitShear1;
	CEditUnit	m_editShear1;
	mit::frx::MButton	  m_chkShearCheck;
	mit::frx::MButton   m_chkShearDeform;
	mit::frx::MButton m_chkWarpingEffect;
	mit::frx::MButton	  m_chkHunch;
	mit::frx::MButton	  m_chkSymmetic;
	CDlgTabCtrl	m_ctrlTab;
	int   m_nWarpingCheck;
	mit::frx::MillustViewer m_wndPicture;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSC_CMP)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetDlg2Data();
	void SetData2Dlg();
	void ChangeStatus();

protected:
	void InitControls();
	void LayoutChildDlgs();

	BOOL ChangeBitmap();
	void SetDataSource(T_SECT_D* pDataSrc);
	
	
	void SetMatlSectData();
	void GetMatlSectData(T_SECT_D* pData);
	

protected:
	BOOL m_bModify;
	T_SECT_D m_SectData;

	CCMSectItemPSC*          m_pParent;
	CCMSectItemPSC_CMPLeft*  m_pLeftDlg;
	CCMSectItemPSC_CMPRight* m_pRightDlg;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSC_CMP)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdCMPLRFBtn();
	afx_msg void OnCmdChangeOffsetBtn();
	afx_msg void OnCmdSectCentroidBtn();
	afx_msg void OnCmdMaterialBtn();
	afx_msg void OnCmdHunchCheck();
	afx_msg void OnCmdSymmetricCheck();
	afx_msg void OnCmdShearCheck();
	afx_msg void OnChangeCmdShearEdit();
	afx_msg void OnChangeCmdAEdit();
	afx_msg void OnChangeCmdBEdit();
	afx_msg void OnChangeCmdHEdit();
	afx_msg void OnChangeCmdTEdit();
	afx_msg void OnChangeCmdMatlEdit();
	afx_msg void OnCmdSectTableinputBtn();
	afx_msg void OnCmdSectWapingCheckBtn();
	afx_msg void OnCmdSectWapingCheckUser();
	afx_msg void OnCmdSectWapingCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTITEMPSC_CMP_H__)
