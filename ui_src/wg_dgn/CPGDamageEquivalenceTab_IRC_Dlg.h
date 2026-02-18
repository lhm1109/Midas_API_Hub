#if !defined(AFX_CPGDAMAGEEQUIVALENCETAB_IRC_DLG_H__)
#define AFX_CPGDAMAGEEQUIVALENCETAB_IRC_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPGDamageEquivalenceTabDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPGDamageEquivalenceTab_IRC_Dlg dialog

#include "..\wg_base\ChildDialog.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"


/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGDamageEquivalence_IRC_Dlg;

class __MY_EXT_CLASS__ CCPGDamageEquivalenceTab_IRC_Dlg : public CChildDialog
{
// Construction
public:
	CCPGDamageEquivalenceTab_IRC_Dlg(CWnd* pParent = NULL);   // standard constructor

	
// Dialog Data
	//{{AFX_DATA(CCPGDamageEquivalenceTab_IRC_Dlg)
	enum { IDD = IDD_DGN_CPG_DAMAGE_EQUIVALENCE_TAB_IRC_DLG };
	
	BOOL	m_bUse;

	double	m_dLamdaV2;
	double	m_dLamdaV4;
	double m_dCorrFact;
		CTextUnit    m_nUnitFf;
		CTextUnit    m_nUnitTf;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	//void SetData(T_CGFR_DATA shearData);
	//T_CGFR_DATA GetData();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGDamageEquivalenceTab_IRC_Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void GetCgfrData();
	void SetCgfrData();
	void SetAllControlEnable();

	CCPGDamageEquivalence_IRC_Dlg* m_pParent;
	T_CGFR_DATA m_Data;

protected:

	void SetInitUnit();
	void InitialDlgData();

	BOOL Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPGDamageEquivalenceTab_IRC_Dlg)
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
