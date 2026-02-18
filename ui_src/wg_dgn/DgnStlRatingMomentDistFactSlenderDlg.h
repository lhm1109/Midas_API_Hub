// DgnStlRatingMomentDistFactSlenderDlg.h: interface for the CDgnStlRatingMomentDistFactSlenderDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DgnStlRatingMomentDistFactSlenderDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_DgnStlRatingMomentDistFactSlenderDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class __MY_EXT_CLASS__ CDgnStlRatingMomentDistFactSlenderDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CDgnStlRatingMomentDistFactSlenderDlg(CWnd* pParent = NULL);

	// Dialog Data
		//{{AFX_DATA(CDgnStlRatingMomentDistFactSlenderDlg)
	enum { IDD = IDD_DGN_STL_RATING_MOMENT_DIST_FACT_SLENDER_DLG };

	int		  m_nOption;
	CEditUnit m_edtLambdaLT;
	//}}AFX_DATA

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDgnStlRatingMomentDistFactSlenderDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	// Implementation
protected:
	void InitUnit();
	void Initial_SelectItem();
	void EnableDisableControls();

	BOOL Dlg2Data();

	CDBDoc* m_pDoc;

	T_MDFS_D m_Data;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlRatingMomentDistFactSlenderDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnOption();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnStlRatingMomentDistFactSlenderDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)