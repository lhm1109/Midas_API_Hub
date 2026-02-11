#if !defined(__CMSECTITEMPSC4CELL_H__)
#define __CMSECTITEMPSC4CELL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSC4Cell.h : header file
//
#include "CMSectItemPSCBase.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC4Cell dialog
class CCMSectItemPSC;

class CCMSectItemPSC4Cell : public CCMSectItemPSCBase
{
// Construction
public:
	CCMSectItemPSC4Cell(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSectItemPSC4Cell();

public:
	virtual BOOL GetDlgData(void* pData);
	virtual void SetCurDlgData(void* pData, BOOL bModify);
	virtual void DisplayOffsetPoint();
	virtual void SetAutotShearCheckPos(double dShearPos[3]);
	virtual void SetAutoMinWebThick(double dShear[3], double dTorsion);
	virtual void SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6]);

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSC4Cell)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_4CELL };
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
	mit::frx::MEdit	m_editGirderNum;
	int		m_nType;
	//}}AFX_DATA
	CTextUnit	m_unitWebShear[3];
	CEditUnit	m_editWebShear[3];
	mit::frx::MButton	  m_chkWebShear[3];
	int   m_nWarpingCheck;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSC4Cell)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	BOOL m_bModify;
	T_SECT_D  m_SectData;
	CCMSectItemPSC* m_pParent;
	CTextUnit m_Units[13];
	CEditUnit m_Edits[13];
	CArray<UINT, UINT> m_aCtrlType;
public:
	void UpdateCurDatas();
	void RedrawSection();
protected:
	
	void EnableDisableControls();
	void ChangeShearCheck(int nCheck);
	void ChangeAutoShearCheck1();
	void ChangeAutoShearCheck3();

	BOOL ChangeBitmap(int nIndex);
	void SaveDataForDrawSection();
	void SetDataSource(T_SECT_D* pDataSrc);
 

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSC4Cell)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeCmdSize();
	afx_msg void OnCmdSectPscType();
	afx_msg void OnChangeCmdSectPscNum();
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


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTITEMPSC4CELL_H__)
