// DgnStlRatingAssignMemberTypeDlg.h: interface for the CDgnStlRatingAssignMemberTypeDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DgnStlRatingAssignMemberTypeDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_DgnStlRatingAssignMemberTypeDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class __MY_EXT_CLASS__ CDgnStlRatingAssignMemberTypeDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CDgnStlRatingAssignMemberTypeDlg(CWnd* pParent = NULL);

	// Dialog Data
		//{{AFX_DATA(CDgnStlRatingAssignMemberTypeDlg)
	enum { IDD = IDD_DGN_STL_RATING_ASSIGN_MEMBER_TYPE_DLG };

	int		m_nOption;
	int     m_nMemberType;
	//}}AFX_DATA

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDgnStlRatingAssignMemberTypeDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	// Implementation
protected:
	void Initial_SelectItem();
	void EnableDisableControls();

	BOOL Dlg2Data();

	CDBDoc* m_pDoc;

	T_UFMT_D m_Data;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlRatingAssignMemberTypeDlg)
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

#endif // !defined(AFX_DgnStlRatingAssignMemberTypeDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
