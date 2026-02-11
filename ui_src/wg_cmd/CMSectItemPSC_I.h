#if !defined(__CMSECTITEMPSC_I_H__)
#define __CMSECTITEMPSC_I_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSC_I.h : header file
//
#include "CMSectItemPSCBase.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC_I dialog
class CCMSectItemPSC;
class CCMSectItemPSCIDB;
class CCMSectItemPSC_I : public CCMSectItemPSCBase
{
// Construction
public:
	CCMSectItemPSC_I(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSectItemPSC_I();

public:
	virtual BOOL GetDlgData(void* pData);
	virtual void SetCurDlgData(void* pData, BOOL bModify);
	virtual void DisplayOffsetPoint();
	virtual void SetAutotShearCheckPos(double dShearPos[3]);
	virtual void SetAutoMinWebThick(double dShear[3], double dTorsion);
	virtual void SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6]);

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSC_I)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_I };
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
	mit::frx::MButton m_chkShearCheck;
	mit::frx::MButton m_chkShearDeform;
	mit::frx::MButton m_chkWarpingEffect;
	CBCGPStatic	m_txtOffset;
	mit::frx::MComboBox m_cobxTypeName;
	mit::frx::MComboBox	m_cobxSectName;
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
	int   m_nWarpingCheck;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSC_I)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void UpdateCurDatas();
	void UpdateCtrls();
	void RedrawSection();
protected:
	void InitCtrlsData();
	void InitSectTypeCombo();
	void InitSectNameCombo();
	
	
	void UpdateCtrlsByJoint();
	int  _GetHeightFromFont();
	//BOOL IsSymmetry(T_SECT_D& rData);
	void ChangeShearCheck(int nCheck);
	void ChangeAutoShearCheck1();
	void ChangeAutoShearCheck3();

	BOOL ChangeBitmap(int nIndex);
	void SaveDataForDrawSection();
	void SetDataSource(T_SECT_D* pDataSrc);
	
	void SyncEditData(BOOL bSync);
	void SyncCheckData(BOOL bSync);
protected:
	BOOL      m_bModify;
	T_SECT_D  m_SectData;
	CTextUnit m_Units[33];
	CEditUnit m_Edits[33];
	mit::frx::MButton   m_jChk[8];
	CCMSectItemPSC* m_pParent;
	CCMSectItemPSCIDB* m_pSectDB;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSC_I)
	afx_msg void OnCmdCheckLeftCtrls();
	afx_msg void OnChangeCmdLeftEdit();
	afx_msg void OnChangeCmdEdit();
	afx_msg void OnCmdCheckCtrls();
	afx_msg void OnSymmetryCheck();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdSectTypeCombo();
	afx_msg void OnSelchangeCmdSectNameCombo();
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
	CArray<UINT, UINT> m_aJ1Group;
	CArray<UINT, UINT> m_aJL1Group;
	CArray<UINT, UINT> m_aJL2Group;
	CArray<UINT, UINT> m_aJL3Group;
	CArray<UINT, UINT> m_aJL4Group;
	CArray<UINT, UINT> m_aJR1Group;
	CArray<UINT, UINT> m_aJR2Group;
	CArray<UINT, UINT> m_aJR3Group;
	CArray<UINT, UINT> m_aJR4Group;
	CArray<UINT, UINT> m_aSymmetryGroup1;
	CArray<UINT, UINT> m_aSymmetryGroup2;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTITEMPSC_I_H__)
