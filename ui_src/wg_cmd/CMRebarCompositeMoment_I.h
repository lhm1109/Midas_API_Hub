#if !defined(__CMREBARCOMPOSITEMOMENT_I_H__)
#define __CMREBARCOMPOSITEMOMENT_I_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMRebarCompositeMoment_I.h : header file
//
//#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_db\wg_db_DBDoc.h"

class CCMRebarCompositeMomentGrid;
/////////////////////////////////////////////////////////////////////////////
// CCMRebarCompositeMoment_I dialog

class CCMRebarCompositeMoment;
class CCMRebarCompositeMoment_I : public CChildDialog
{
// Construction
public:
	CCMRebarCompositeMoment_I(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMRebarCompositeMoment_I();

// interface
public:
	void SetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData);
	void GetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData, BOOL bSaveAlways);

// Dialog Data
	//{{AFX_DATA(CCMRebarCompositeMoment_I)
	enum { IDD = IDD_CMD_REBAR_COMPOSITE_MOMENT_I };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMRebarCompositeMoment_I)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CCMRebarCompositeMomentGrid* m_wndGrid;
	CArray<T_RPSC_MBAR, T_RPSC_MBAR&> m_Data;
	CCMRebarCompositeMoment* m_pParent;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMRebarCompositeMoment_I)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMREBARCOMPOSITEMOMENT_I_H__)
