#if !defined(__DGNCONWALLENVELOPMETHOD_H__)
#define __DGNCONWALLENVELOPMETHOD_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConWallEnvelopMethod.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MillustViewer.h"
#include "HeaderPre.h"


/////////////////////////////////////////////////////////////////////////////
// CDgnConWallEnvelopMethod dialog

class __MY_EXT_CLASS__ CDgnConWallEnvelopMethod : public CDialogMove
{
// Construction
public:
	CDgnConWallEnvelopMethod(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConWallEnvelopMethod)
	enum { IDD = IDD_DGN_CON_WALL_ENVELOP_METHOD_DLG };
	//}}AFX_DATA

public:
	void SetMethod(int nMethod=0) { m_nMethod = nMethod; }
	int  GetMethod() { return m_nMethod; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConWallEnvelopMethod)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nMethod;
	CArray<UINT, UINT> m_aMethodRdo;
	void Data2Dlg();
	BOOL Dlg2Data();

	mit::frx::MillustViewer m_wndMethod1;
	mit::frx::MillustViewer m_wndMethod2;

	// Generated message map functions
	//{{AFX_MSG(CDgnConWallEnvelopMethod)
	virtual BOOL OnInitDialog();	

	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNCONWALLENVELOPMETHOD_H__)
