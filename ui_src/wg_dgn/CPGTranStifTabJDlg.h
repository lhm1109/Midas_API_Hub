#if !defined(AFX_CPGTRANSTIFTABJDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_CPGTRANSTIFTABJDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPGTranStifTabJDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPGTranStifTabJDlg dialog

#include "..\wg_base\ChildDialog.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\MIT_frx\MButton.h"

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGTranStifDlg;

class __MY_EXT_CLASS__ CCPGTranStifTabJDlg : public CChildDialog
{
// Construction
public:
	CCPGTranStifTabJDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCPGTranStifTabJDlg();
	
// Dialog Data
	//{{AFX_DATA(CCPGTranStifTabJDlg)
	enum { IDD = IDD_DGN_CPG_TRAN_STIF_TAB_J_DLG };
	
	BOOL	m_bUseStif;
	BOOL	m_bInterRigidStif;
	BOOL	m_bInterNonRigidStif;

	int		m_iStifNum;

	CEditUnit	m_dHt;
	CEditUnit	m_dt;
	CEditUnit	m_dPitch;
	CEditUnit	m_dDistRstif;
	CEditUnit	m_dDistNRstif;

	CTextUnit	m_dHtUnit;
	CTextUnit	m_dtUnit;
	CTextUnit	m_dPitchUnit;
	CTextUnit	m_dDistRstifUnit;
	CTextUnit	m_dDistNRstifUnit;

	mit::frx::MButton m_Ctrl_UseCheck;
	mit::frx::MButton m_Ctrl_OneRadio;
	mit::frx::MButton m_Ctrl_TwoRadio;
	mit::frx::MButton m_Ctrl_RigidCheck;
	mit::frx::MButton m_Ctrl_NonRigidCheck;
	CBCGPStatic m_Ctrl_Static;
	CBCGPStatic m_Ctrl_Static2;
	CBCGPStatic m_Ctrl_Static3;
	CBCGPStatic m_Ctrl_Static4;
	CBCGPStatic m_Ctrl_Static5;

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	//void SetData(T_CGTS_STIF stifData);
	//T_CGTS_STIF GetData();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGTranStifTabJDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void GetCgtsData();
	void SetCgtsData();
	void SetAllControlEnable(bool enable);

	CCPGTranStifDlg* m_pParent;
	T_CGTS_STIF m_Data;


protected:
	
	BOOL Data2Dlg();
	BOOL Dlg2Data();

	void SetInitUnit();
	void InitialDlgData();

	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPGTranStifTabJDlg)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnUseStifChk();
	afx_msg void OnRigidChk();
	afx_msg void OnNonRigidChk();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPGTRANSTIFTABJDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
