#if !defined(__DGNREBARCPGSTIFF_J_H__)
#define __DGNREBARCPGSTIFF_J_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRebarCpgStiff_J.h : header file
//
//#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\ChildDialog.h"

class CDgnRebarCpgStiffGrid;
/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgStiff_J dialog
class CDgnRebarCpgStiff;
class CDgnRebarCpgStiff_J : public CChildDialog
{
// Construction
public:
	CDgnRebarCpgStiff_J(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDgnRebarCpgStiff_J();

// interface
public:
	void SetCurData(CArray<T_CLRS_STIF, T_CLRS_STIF&>* pData, BOOL bSymmetry);
	void GetCurData(CArray<T_CLRS_STIF, T_CLRS_STIF&>* pData, BOOL bSaveAlways);

// Dialog Data
	//{{AFX_DATA(CDgnRebarCpgStiff_J)
	enum { IDD = IDD_DGN_REBAR_CPG_STIFF_J };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRebarCpgStiff_J)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CDgnRebarCpgStiffGrid* m_wndGrid;
	CDgnRebarCpgStiff* m_pParent;

	BOOL m_bSymmetry;
	CArray<T_CLRS_STIF, T_CLRS_STIF&> m_Data;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnRebarCpgStiff_J)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNREBARCPGSTIFF_J_H__)
