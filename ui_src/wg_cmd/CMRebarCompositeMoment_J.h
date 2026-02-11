#if !defined(__CMREBARCOMPOSITEMOMENT_J_H__)
#define __CMREBARCOMPOSITEMOMENT_J_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMRebarCompositeMoment_J.h : header file
//
//#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_db\wg_db_DBDoc.h"

class CCMRebarCompositeMomentGrid;
/////////////////////////////////////////////////////////////////////////////
// CCMRebarCompositeMoment_J dialog
class CCMRebarCompositeMoment;
class CCMRebarCompositeMoment_J : public CChildDialog
{
// Construction
public:
	CCMRebarCompositeMoment_J(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMRebarCompositeMoment_J();

// interface
public:
	void SetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData, BOOL bSymmetry);
	void GetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData, BOOL bSaveAlways);

// Dialog Data
	//{{AFX_DATA(CCMRebarCompositeMoment_J)
	enum { IDD = IDD_CMD_REBAR_COMPOSITE_MOMENT_J };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMRebarCompositeMoment_J)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CCMRebarCompositeMomentGrid* m_wndGrid;
	CCMRebarCompositeMoment* m_pParent;

	BOOL m_bSymmetry;
	CArray<T_RPSC_MBAR, T_RPSC_MBAR&> m_Data;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMRebarCompositeMoment_J)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMREBARCOMPOSITEMOMENT_J_H__)
