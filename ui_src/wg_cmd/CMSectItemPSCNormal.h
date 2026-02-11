#if !defined(AFX_CMSECTITEMPSCNORMAL_H__B5B08BE7_6978_4889_AF36_BC2D86E7A799__INCLUDED_)
#define AFX_CMSECTITEMPSCNORMAL_H__B5B08BE7_6978_4889_AF36_BC2D86E7A799__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCNormal.h : header file
//
#include "CMSectItemPSCBase.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCNormal dialog
class CCMSectItemPSC;

class CCMSectItemPSCNormal : public CCMSectItemPSCBase
{
// Construction
public:
	void InitCtrlsData();
	CCMSectItemPSCNormal(CWnd* pParent = NULL, int nType=0);   // nType : (0)Normal (1)Half

public:
	virtual BOOL GetDlgData(void* pData);
	virtual void SetCurDlgData(void * pData, BOOL bModify);
	virtual void DisplayOffsetPoint();
	virtual void SetAutotShearCheckPos(double dShearPos[3]);
	virtual void SetAutoMinWebThick(double dShear[3], double dTorsion);
	virtual void SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6]);

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSCNormal)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_NOMAL };
	//CTextUnit	m_unitWebI2;
	//CEditUnit	m_editWebI2;
	//CButton	m_chkWebI2;
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
	mit::frx::MButton m_chkShearDeform;
	mit::frx::MButton m_chkWarpingEffect;
	CBCGPStatic	m_txtOffset;
	mit::frx::MComboBox	m_cOffsetCB;
	mit::frx::MComboBox	m_cSecTypeCB;
	BOOL	m_bJI1;
	BOOL	m_bJI2;
	BOOL	m_bJI3;
	BOOL	m_bJI4;
	BOOL	m_bJI5;
	BOOL	m_bJO1;
	BOOL	m_bJO2;
	BOOL	m_bJO3;
	int		m_nCellType;
	int		m_nHalfType;
	int   m_nWarpingCheck;
	//}}AFX_DATA
	CTextUnit	m_unitWebShear[3];
	CEditUnit	m_editWebShear[3];
	mit::frx::MButton	  m_chkWebShear[3];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCNormal)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void AlignControls();
	void InitSecTypeCombo();
	
	void UpdateCtrlsByJoint();
	void UpdateCtrlsByCell();
	void UpdateCtrlsByType();
	void UpdateCtrlsBySect();
	
	void ChangeShearCheck(int nCheck);
	void ChangeAutoShearCheck1();
	void ChangeAutoShearCheck3();

protected:
	BOOL      m_bModify;
	int       m_bHalfType;
	T_SECT_D  m_SectData;
	CTextUnit m_Units[30];
	CEditUnit m_Edits[30];

	CCMSectItemPSC* m_pParent;
	BOOL ChangeBitmap(int nIndex);
	void SaveDataForDrawSection();
	void SetDataSource(T_SECT_D* pDataSrc);
	

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSCNormal)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdCheckCtrl();
	afx_msg void OnSelchangeCmdPscSectypeCb();
	afx_msg void OnCmdCheckCtrls2();
	afx_msg void OnCmdSectCellRadio();
	afx_msg void OnChangeCmdSize();
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

private:
	CArray<UINT, UINT> arJO1Group;
	CArray<UINT, UINT> arJO2Group;
	CArray<UINT, UINT> arJO3Group;
	CArray<UINT, UINT> arJI1Group;
	CArray<UINT, UINT> arJI2Group;
	CArray<UINT, UINT> arJI3Group;
	CArray<UINT, UINT> arJI4Group;
	CArray<UINT, UINT> arJI5Group;
	CArray<UINT, UINT> arComnGroup;
	CArray<UINT, UINT> arNoneGroup;
	CArray<UINT, UINT> arCircGroup;
	CArray<UINT, UINT> arNormGroup;
	CArray<UINT, UINT> arHalfGroup;
	CArray<UINT, UINT> arShearGroup;
	CArray<UINT, UINT> arWebGroup;
public:
	int GetSellType();
	int GetHalfType();
	BOOL IsHalfType();
	void UpdateCurDatas(); 
	void RedrawSection();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSCNORMAL_H__B5B08BE7_6978_4889_AF36_BC2D86E7A799__INCLUDED_)
