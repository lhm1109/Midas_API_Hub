#if !defined(__DGNREBARCPGSTIFF_I_H__)
#define __DGNREBARCPGSTIFF_I_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRebarCpgStiff_I.h : header file
//
//#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\ChildDialog.h"

class CDgnRebarCpgStiffGrid;
/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgStiff_I dialog

class CDgnRebarCpgStiff;
class CDgnRebarCpgStiff_I : public CChildDialog
{
// Construction
public:
	CDgnRebarCpgStiff_I(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDgnRebarCpgStiff_I();

// interface
public:
	void SetCurData(CArray<T_CLRS_STIF, T_CLRS_STIF&>* pData);
	void GetCurData(CArray<T_CLRS_STIF, T_CLRS_STIF&>* pData, BOOL bSaveAlways);

// Dialog Data
	//{{AFX_DATA(CDgnRebarCpgStiff_I)
	enum { IDD = IDD_DGN_REBAR_CPG_STIFF_I };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRebarCpgStiff_I)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CDgnRebarCpgStiffGrid* m_wndGrid;
	CArray<T_CLRS_STIF, T_CLRS_STIF&> m_Data;
	CDgnRebarCpgStiff* m_pParent;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnRebarCpgStiff_I)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNREBARCPGSTIFF_I_H__)
