#if !defined(AFX_CMSECTITEMPSCTEE_H__90A97DBA_95A9_4CBA_80DC_1E28AA3F4FA4__INCLUDED_)
#define AFX_CMSECTITEMPSCTEE_H__90A97DBA_95A9_4CBA_80DC_1E28AA3F4FA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCTee.h : header file
//
#include "CMSectItemPSCBase.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCTee dialog
class CCMSectItemPSC;
class CCMSectItemPSCTeeDB;

class CCMSectItemPSCTee : public CCMSectItemPSCBase
{
// Construction
public:
	CCMSectItemPSCTee(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSectItemPSCTee();

public:
	virtual BOOL GetDlgData(void* pData);
	virtual void SetCurDlgData(void* pData, BOOL bModify);
	virtual void DisplayOffsetPoint();
	virtual void SetAutotShearCheckPos(double dShearPos[3]);
	virtual void SetAutoMinWebThick(double dShear[3], double dTorsion);
	virtual void SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6]);

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSCTee)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_TEE };
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
	BOOL	m_bJ1;
	BOOL	m_bJL1;
	BOOL	m_bJL2;
	BOOL	m_bJL3;
	BOOL	m_bJL4;
	BOOL	m_bJR1;
	BOOL	m_bJR2;
	BOOL	m_bJR3;
	BOOL	m_bJR4;
	BOOL	m_bSymmetry;
	//}}AFX_DATA
	CTextUnit	m_unitWebShear[3];
	CEditUnit	m_editWebShear[3];
	mit::frx::MButton	  m_chkWebShear[3];


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCTee)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	BOOL m_bModify;
	T_SECT_D  m_SectData;
	CCMSectItemPSC* m_pParent;
	CCMSectItemPSCTeeDB* m_pSectDB;

	CTextUnit m_Units[31];
	CEditUnit m_Edits[31];
	mit::frx::MButton   m_jChk[8];
	int   m_nWarpingCheck;
public:
	void UpdateCurDatas();
	void RedrawSection();
	void UpdateCtrlsByJoint();
	void UpdateCtrlsBySymmetry();
protected:
	void InitCtrlsData();
	void InitSectNameCombo();
	
	
	//BOOL IsSymmetry(T_SECT_D& rData);
	void ChangeShearCheck(int nCheck);
	void ChangeAutoShearCheck1();
	void ChangeAutoShearCheck3();

	BOOL ChangeBitmap(int nIndex);
	void SaveDataForDrawSection();
	void SetDataSource(T_SECT_D* pDataSrc);
	
	void SyncEditData(BOOL bSync);
	void SyncCheckData(BOOL bSync);
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSCTee)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeCmdLeftSize();
	afx_msg void OnChangeCmdSize();
	afx_msg void OnCmdCheckLeftCtrls();
	afx_msg void OnCmdCheckCtrls();
	afx_msg void OnCmdPscSymmetryCheck();
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

private:
	CArray<UINT, UINT> arJ1Group;
	CArray<UINT, UINT> arJL1Group;
	CArray<UINT, UINT> arJL2Group;
	CArray<UINT, UINT> arJL3Group;
	CArray<UINT, UINT> arJL4Group;
	CArray<UINT, UINT> arJR1Group;
	CArray<UINT, UINT> arJR2Group;
	CArray<UINT, UINT> arJR3Group;
	CArray<UINT, UINT> arJR4Group;
	CArray<UINT, UINT> arSymmetryGroup;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSCTEE_H__90A97DBA_95A9_4CBA_80DC_1E28AA3F4FA4__INCLUDED_)
