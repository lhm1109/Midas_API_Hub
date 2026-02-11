#if !defined(AFX_CMSECTITEMPSCPLAT_H__00AC5A0F_CCF6_41C4_A50C_85FE9E96DCDA__INCLUDED_)
#define AFX_CMSECTITEMPSCPLAT_H__00AC5A0F_CCF6_41C4_A50C_85FE9E96DCDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCPlat.h : header file
//
#include "CMSectItemPSCBase.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCPlat dialog
class CCMSectItemPSC;
class CCMSectItemPSCPlatDB;

class CCMSectItemPSCPlat : public CCMSectItemPSCBase
{
// Construction
public:
	CCMSectItemPSCPlat(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSectItemPSCPlat();

public:
	virtual BOOL GetDlgData(void* pData);
	virtual void SetCurDlgData(void* pData, BOOL bModify);
	virtual void DisplayOffsetPoint();
	virtual void SetAutotShearCheckPos(double dShearPos[3]);
	virtual void SetAutoMinWebThick(double dShear[3], double dTorsion);
	virtual void SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6]);

public:
// Dialog Data
	//{{AFX_DATA(CCMSectItemPSCPlat)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_PLAT };
	mit::frx::MButton	m_chkWebI;
	mit::frx::MButton	m_chkShear3;
	mit::frx::MButton	m_chkShear1;
	mit::frx::MComboBox	m_cobxSectName;
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
	mit::frx::MComboBox	m_cbo1Cell;
	mit::frx::MComboBox	m_cboHalf;
	BOOL	m_bJL1;
	BOOL	m_bJR1;
	int		m_nSecType;
	BOOL	m_bSymmetry;
	//}}AFX_DATA
	CTextUnit	m_unitWebShear[3];
	CEditUnit	m_editWebShear[3];
	mit::frx::MButton	  m_chkWebShear[3];
	int   m_nWarpingCheck;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCPlat)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	BOOL m_bModify;
	T_SECT_D  m_SectData;
	CCMSectItemPSC* m_pParent;
	CCMSectItemPSCPlatDB* m_pSectDB;

	CTextUnit m_Units[31];
	CEditUnit m_Edits[31];
	mit::frx::MButton	  m_jChk[2];
public:
	void UpdateCurDatas();
	void UpdateCtrlsByType();
	void RedrawSection();

protected:
	void InitCtrlsData();
	
	
	void UpdateCtrlsByJoint();
	void InitCellTypeCombo();
	void InitSectNameCombo();
	//BOOL IsSymmetry(T_SECT_D& rData);
	void ChangeShearCheck(int nCheck);
	void ChangeAutoShearCheck1();
	void ChangeAutoShearCheck3();

	void SaveDataForDrawSection();
	BOOL ChangeBitmap(int nIndex);
	void SetDataSource(T_SECT_D* pDataSrc);
	
	void SyncEditData(BOOL bSync);
	void SyncCheckData(BOOL bSync);
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSCPlat)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSectype();
	afx_msg void OnSelchangeCmdCombo();
	afx_msg void OnCmdJl1Check();
	afx_msg void OnCmdJr1Check();
	afx_msg void OnSymmetryCheck();
	afx_msg void OnChangeCmdLeftEdit();
	afx_msg void OnChangeCmdEdit();
	afx_msg void OnCmdSectCentroidBtn();
	afx_msg void OnCmdChangeOffsetBtn();
	afx_msg void OnChangeCmdShearEdit();
	afx_msg void OnCmdShearCheck();
	afx_msg void OnSelchangeCmdSectNameCombo();
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

protected:
	CArray<UINT, UINT> LeftDefaltGroup;
	CArray<UINT, UINT> LeftOptionGroup;
	CArray<UINT, UINT> RghtDefaltGroup;
	CArray<UINT, UINT> RghtOptionGroup;
	CArray<UINT, UINT> CellOptionGroup;
	CArray<UINT, UINT> JL1Group;
	CArray<UINT, UINT> JR1Group;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSCPLAT_H__00AC5A0F_CCF6_41C4_A50C_85FE9E96DCDA__INCLUDED_)
