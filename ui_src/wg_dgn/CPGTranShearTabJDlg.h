#if !defined(AFX_CPGTRANSHEARTABJDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_CPGTRANSHEARTABJDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPGTranShearTabJDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPGTranShearTabJDlg dialog

#include "..\wg_base\ChildDialog.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"


/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGTranShearDlg;

class __MY_EXT_CLASS__ CCPGTranShearTabJDlg : public CChildDialog
{
// Construction
public:
	CCPGTranShearTabJDlg(CWnd* pParent = NULL);   // standard constructor

	
// Dialog Data
	//{{AFX_DATA(CCPGTranShearTabJDlg)
	enum { IDD = IDD_DGN_CPG_TRAN_SHEAR_TAB_DLG };
	
	BOOL	m_bUseShear;
	
	int		m_iShearNum;

	CEditUnit	m_dFu;
	CEditUnit	m_dDs;
	CEditUnit	m_dHsc;
	CEditUnit	m_dSt;
	CEditUnit	m_dSc;

	CTextUnit	m_dFuUnit;
	CTextUnit	m_dDsUnit;
	CTextUnit	m_dHscUnit;
	CTextUnit	m_dStUnit;
	CTextUnit	m_dScUnit;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	//void SetData(T_CGSC_STUD shearData);
	//T_CGSC_STUD GetData();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGTranShearTabJDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void GetCgscData();
	void SetCgscData();
	void SetAllControlEnable(bool enable);

	CCPGTranShearDlg* m_pParent;
	T_CGSC_STUD m_Data;

protected:

	void SetInitUnit();
	void InitialDlgData();

	BOOL Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPGTranShearTabJDlg)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnUseShearChk();	
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPGTRANSHEARTABJDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
