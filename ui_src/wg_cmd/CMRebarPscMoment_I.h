#if !defined(__CMREBARPSCMOMENT_I_H__)
#define __CMREBARPSCMOMENT_I_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMRebarPscMoment_I.h : header file
//
//#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_db\wg_db_DBDoc.h"

class CCMRebarPscMomentGrid;
/////////////////////////////////////////////////////////////////////////////
// CCMRebarPscMoment_I dialog

class CCMRebarPscMoment;
class CCMRebarPscMoment_I : public CChildDialog
{
// Construction
public:
	CCMRebarPscMoment_I(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMRebarPscMoment_I();

// interface
public:
	void SetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData);
	void GetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData, BOOL bSaveAlways);

// Dialog Data
	//{{AFX_DATA(CCMRebarPscMoment_I)
	enum { IDD = IDD_CMD_REBAR_PSC_MOMENT_I };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMRebarPscMoment_I)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CCMRebarPscMomentGrid* m_wndGrid;
	CArray<T_RPSC_MBAR, T_RPSC_MBAR&> m_Data;
	CCMRebarPscMoment* m_pParent;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMRebarPscMoment_I)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMREBARPSCMOMENT_I_H__)
