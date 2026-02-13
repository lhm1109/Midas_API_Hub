#if !defined(AFX_DGNSTLRATINGSHEARCONNTABJDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_DGNSTLRATINGSHEARCONNTABJDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlRatingShearConnTabJDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingShearConnTabJDlg dialog

#include "..\wg_base\ChildDialog.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"


/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CDgnStlRatingShearConnDlg;

class __MY_EXT_CLASS__ CDgnStlRatingShearConnTabJDlg : public CChildDialog
{
	// Construction
public:
	CDgnStlRatingShearConnTabJDlg(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CDgnStlRatingShearConnTabJDlg)
	enum { IDD = IDD_DGN_STL_RATING_SHEAR_CONN_TAB_DLG };

	CEditUnit m_iNumStudWeb;
	CEditUnit m_dLSpacing;
	CEditUnit m_dPim;
	CEditUnit m_dPr;
	CEditUnit m_dMaxRatio;
	CEditUnit m_dNa;

	CTextUnit m_dLSpacingUnit;
	CTextUnit m_dPimUnit;
	CTextUnit m_dPrUnit;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlRatingShearConnTabJDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void GetSrscData();
	void SetSrscData();
	void SetAllControlEnable(bool enable);

	CDgnStlRatingShearConnDlg* m_pParent;
	T_SRSC_STUD m_Data;

protected:

	void SetInitUnit();
	void InitialDlgData();

	BOOL Data2Dlg();
	BOOL Dlg2Data();

	void SetText();

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnStlRatingShearConnTabJDlg)
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

#endif // !defined(AFX_DgnStlRatingShearConnTABJDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
