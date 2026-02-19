#if !defined(AFX_CPGCurveBridTabIDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_CPGCurveBridTabIDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPGCurveBridTabIDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPGCurveBridTabIDlg dialog

#include "..\wg_base\ChildDialog.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"


/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGCurveBridDlg;

class __MY_EXT_CLASS__ CCPGCurveBridTabIDlg : public CChildDialog
{
// Construction
public:
	CCPGCurveBridTabIDlg(CWnd* pParent = NULL);   // standard constructor

	
// Dialog Data
	//{{AFX_DATA(CCPGCurveBridTabIDlg)
	enum { IDD = IDD_DGN_CPG_CURV_BRID_TAB_I_DLG };
	
	CEditUnit	m_edtRadius;
	CTextUnit	m_untRadius;
	int       m_nCurveType;

	CFoldGroupBox_Cross	m_chkGroup;

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGCurveBridTabIDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void  GetCpcbData();
	void  SetCpcbData();
	void  ControlsEnableDisable();
	void  ResizeWindow();
	CRect GetResizeHeight();

	CCPGCurveBridDlg* m_pParent;
	T_CPCB_TYPE m_Data;

protected:
	void InitUnit();

	void Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPGCurveBridTabIDlg)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDgnCpgCpcbCurveGuideBtn();
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPGCurveBridTabIDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
