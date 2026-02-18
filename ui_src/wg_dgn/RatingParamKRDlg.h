#if !defined(__RATINGPARAMKRDLG_H__)
#define __RATINGPARAMKRDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RatingParamKRDlg.h : header file
//
#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CRatingParamKRDlg dialog
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRatingParamKRDlg : public CDialogMove
{
// Construction
public:
	CRatingParamKRDlg(CWnd* pParent = NULL);   // standard constructor
	~CRatingParamKRDlg();

// Dialog Data
	//{{AFX_DATA(CRatingParamKRDlg)
	enum { IDD = IDD_DGN_PSC_RATING_PRARM_KR_DLG };	
	MButton	m_ratioPSType1;
	MButton	m_ratioPSType2;
	MButton	m_ratioPSType3;
	MButton	m_ratioFlexureType1;
	MButton	m_ratioFlexureType2;
	MButton	m_chkTensionBar;
	CEditUnit	m_editLiveLoad;
	MButton	m_chkDgnOutput1;
	MButton	m_chkDgnOutput2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRatingParamKRDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRatingParamKRDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnDgnSelectAll();
	afx_msg void OnDgnUnSelectAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

	CDBDoc* m_pDoc;

	T_RAKR_D m_Data;

	void InitUnit();	
	void Data2Dlg();
	void Dlg2Data();

};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__RATINGPARAMKRDLG_H__)
