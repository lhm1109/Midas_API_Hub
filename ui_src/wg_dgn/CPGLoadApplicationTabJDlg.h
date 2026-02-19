#if !defined(AFX_CPGLOADAPPLICATIONTABJDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_CPGLOADAPPLICATIONTABJDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPGLoadApplicationTabJDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPGLoadApplicationTabJDlg dialog

#include "..\wg_base\ChildDialog.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"


/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGLoadApplicationDlg;

class __MY_EXT_CLASS__ CCPGLoadApplicationTabJDlg : public CChildDialog
{
// Construction
public:
	CCPGLoadApplicationTabJDlg(CWnd* pParent = NULL);   // standard constructor

	
// Dialog Data
	//{{AFX_DATA(CCPGLoadApplicationTabJDlg)
	enum { IDD = IDD_DGN_CPG_LOAD_APPL_TAB_I_DLG };
	
	BOOL	m_bUseLoad;
	int		m_iType;

	CEditUnit	m_dA;
	CEditUnit	m_dSs;
	CEditUnit	m_dC;
	
	CTextUnit	m_dAUnit;
	CTextUnit	m_dSsUnit;
	CTextUnit	m_dCUnit;	
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	//void SetData(T_CPTF_TYPE shearData);
	//T_CPTF_TYPE GetData();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGLoadApplicationTabJDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void GetCptfData();
	void SetCptfData();
	void SetAllControlEnable(bool enable);

	CCPGLoadApplicationDlg* m_pParent;
	T_CPTF_TYPE m_Data;

protected:

	void SetInitUnit();
	void InitialDlgData();

	BOOL Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPGLoadApplicationTabJDlg)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnUseLoadChk();
	afx_msg void OnTypeRadioClick();	
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPGLOADAPPLICATIONTABJDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
