#if !defined(AFX_CPGUnbraLengTabIDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_CPGUnbraLengTabIDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPGUnbraLengTabIDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPGUnbraLengTabIDlg dialog

#include "..\wg_base\ChildDialog.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"


/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGUnbraLengDlg;

class __MY_EXT_CLASS__ CCPGUnbraLengTabIDlg : public CChildDialog
{
// Construction
public:
	CCPGUnbraLengTabIDlg(CWnd* pParent = NULL);   // standard constructor

	
// Dialog Data
	//{{AFX_DATA(CCPGUnbraLengTabIDlg)
	enum { IDD = IDD_DGN_CPG_UNBR_LENG_TAB_I_DLG };
	
	CEditUnit	m_dFlangeTop;
	CEditUnit	m_dFlangeBot;

	CTextUnit m_dFlangeTopU;
	CTextUnit	m_dFlangeBotU;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	//void SetData(T_CPUL_TYPE shearData);
	//T_CPUL_TYPE GetData();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGUnbraLengTabIDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void GetCpulData();
	void SetCpulData();
	void SetAllControlEnable();

	CCPGUnbraLengDlg* m_pParent;
	T_CPUL_TYPE m_Data;

protected:

	void SetInitUnit();
	void InitialDlgData();

	BOOL Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPGUnbraLengTabIDlg)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPGUnbraLengTabIDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
