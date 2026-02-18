#if !defined(AFX_CRCDesignOptTabJDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_CRCDesignOptTabJDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CRCDesignOptTabJDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCRCDesignOptTabJDlg dialog

#include "..\wg_base\ChildDialog.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"


/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCRCDgnOption;

class __MY_EXT_CLASS__ CCRCDesignOptTabJDlg : public CChildDialog
{
// Construction
public:
	CCRCDesignOptTabJDlg(CWnd* pParent = NULL);   // standard constructor

	
// Dialog Data
	//{{AFX_DATA(CCRCDesignOptTabJDlg)
	enum { IDD = IDD_DGN_CRC_DESIGN_OPTION_TMH_TAB_J
	};
	
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
	//{{AFX_VIRTUAL(CCRCDesignOptTabJDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void GetCpulData();
	void SetCpulData();
	void SetAllControlEnable(BOOL bEnable);

	CCRCDgnOption* m_pParent;
	T_CPUL_TYPE m_Data;

protected:

	void SetInitUnit();
	void InitialDlgData();

	BOOL Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCRCDesignOptTabJDlg)
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

#endif // !defined(AFX_CRCDesignOptTabJDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
