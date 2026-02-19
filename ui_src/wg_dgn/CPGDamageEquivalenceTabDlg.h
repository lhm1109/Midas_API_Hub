#if !defined(AFX_CPGDAMAGEEQUIVALENCETABDLG_H__)
#define AFX_CPGDAMAGEEQUIVALENCETABDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPGDamageEquivalenceTabDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPGDamageEquivalenceTabDlg dialog

#include "..\wg_base\ChildDialog.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"


/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGDamageEquivalenceDlg;

class __MY_EXT_CLASS__ CCPGDamageEquivalenceTabDlg : public CChildDialog
{
// Construction
public:
	CCPGDamageEquivalenceTabDlg(CWnd* pParent = NULL);   // standard constructor

	
// Dialog Data
	//{{AFX_DATA(CCPGDamageEquivalenceTabDlg)
	enum { IDD = IDD_DGN_CPG_DAMAGE_EQUIVALENCE_TAB_DLG };
	
	BOOL	m_bUse;

	double	m_dLamdaV2;
	double	m_dLamdaV4;
	
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	//void SetData(T_CGFR_DATA shearData);
	//T_CGFR_DATA GetData();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGDamageEquivalenceTabDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void GetCgfrData();
	void SetCgfrData();
	void SetAllControlEnable();

	CCPGDamageEquivalenceDlg* m_pParent;
	T_CGFR_DATA m_Data;

protected:

	void SetInitUnit();
	void InitialDlgData();

	BOOL Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPGDamageEquivalenceTabDlg)
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

#endif // !defined(AFX_CPGDAMAGEEQUIVALENCETABDLG_H__)
