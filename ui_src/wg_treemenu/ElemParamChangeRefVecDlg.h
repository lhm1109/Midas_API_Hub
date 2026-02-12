#if !defined(__ELEMPARAMCHANGEREFVECDLG_H__)
#define      __ELEMPARAMCHANGEREFVECDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemParamChangeRefVecDlg.h : header file
//
#include "..\wg_db\wg_db_DlgBase.h"
#include "..\wg_db\wg_db_SelectCtrl.h"

class CPlateSelectEdit : public CSelectEdit
{
public:
	CPlateSelectEdit() {}

protected:
	afx_msg LRESULT OnUpdateText(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

class CElemParamChangeDlg;
class CElemParamChangeRefVecDlg : public CDBDlgBase
{
public:
	CElemParamChangeRefVecDlg(CWnd* pParent = NULL);
	~CElemParamChangeRefVecDlg();

	// Dialog Data
	//{{AFX_DATA(CMgtShellDlg)
	enum { IDD = IDD_TM_ELEM_PARAMETER_REF_VEC_DLG };
	CPlateSelectEdit m_wndSelectElemListEdit;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMgtShellDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	//virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);

	// Implementation
protected:
	CElemParamChangeDlg * m_pParent;
	CString GetAverageVectorOfSelectedPlate();

	// Generated message map functions
	//{{AFX_MSG(CMgtShellDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX__ELEMPARAMCHANGEREFVECDLG_H__)