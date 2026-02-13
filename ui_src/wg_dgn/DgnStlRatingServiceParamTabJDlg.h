#if !defined(AFX_DgnStlRatingServiceParamTabJDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_DgnStlRatingServiceParamTabJDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlRatingServiceParamTabJDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingServiceParamTabJDlg dialog

#include "..\wg_base\ChildDialog.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"


/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CDgnStlRatingServiceParamDlg;
class CDBDoc;

class __MY_EXT_CLASS__ CDgnStlRatingServiceParamTabJDlg : public CChildDialog
{
	// Construction
public:
	CDgnStlRatingServiceParamTabJDlg(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CDgnStlRatingServiceParamTabJDlg)
	enum { IDD = IDD_DGN_STL_RATING_SERVICE_PARAM_TAB_J_DLG };

	CEditUnit m_edtTopFlange;
	CTextUnit m_untTopFlange;
	CEditUnit m_edtBotFlange;
	CTextUnit m_untBotFlange;

	CArray <UINT, UINT> arWarpingCtrlID;

	// NOTE: the ClassWizard will add data members here
//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlRatingServiceParamTabJDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void GetSrspData();
	void SetSrspData();
	void ControlsEnableDisable(BOOL bEnable);

	CDgnStlRatingServiceParamDlg* m_pParent;
	T_SRSP_TYPE m_Data;

protected:
	CDBDoc *m_pDoc;
	void InitUnit();

	void Data2Dlg();
	BOOL Dlg2Data();
	void SetText();

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnStlRatingServiceParamTabJDlg)
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

#endif // !defined(AFX_DgnStlRatingServiceParamTabJDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
