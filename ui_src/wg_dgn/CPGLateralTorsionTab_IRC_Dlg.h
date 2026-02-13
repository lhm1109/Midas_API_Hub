#if !defined(AFX_CPGLATERALTORSIONTAB_IRC_DLG_H__)
#define AFX_CPGLATERALTORSIONTAB_IRC_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPGLateralTorsionTab_IRC_Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPGLateralTorsionTab_IRC_Dlg dialog

#include "..\wg_base\ChildDialog.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"


/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGLateraTorsion_IRC_Dlg;

class __MY_EXT_CLASS__ CCPGLateralTorsionTab_IRC_Dlg : public CChildDialog
{
// Construction
public:
	CCPGLateralTorsionTab_IRC_Dlg(CWnd* pParent = NULL);   // standard constructor

	
// Dialog Data
	//{{AFX_DATA(CCPGLateralTorsionTab_IRC_Dlg)
	enum { IDD = IDD_DGN_CPG_RATER_TORSION_TAB_IRC_DLG };
	
	BOOL	m_bUse;

	CEditUnit	m_dI;
	CEditUnit	m_dCd;
	int	m_iAlpha;
	CEditUnit	m_dA;

	CTextUnit	m_dIUnit;
	CTextUnit	m_dCdUnit;	
	CTextUnit	m_dAUnit;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	//void SetData(T_CGLT_DATA shearData);
	//T_CGLT_DATA GetData();

// 	CArray <UINT, UINT> m_aAlphaCtrl; // Ctrl ID for Alpha (for k1)
// 	CArray <UINT, UINT> m_aSBPBCtrl;  // Ctrl ID for spacing between the parallel beam (a)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGLateralTorsionTab_IRC_Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void GetCgltData();
	void SetCgltData();
	void SetAllControlEnable();

	CCPGLateraTorsion_IRC_Dlg* m_pParent;
	T_CGLT_DATA m_Data;

protected:

	void SetInitUnit();
	void InitialDlgData();

	BOOL Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPGLateralTorsionTab_IRC_Dlg)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnUseChk();	
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPGLateralTorsionTab_IRC_Dlg_H__)
