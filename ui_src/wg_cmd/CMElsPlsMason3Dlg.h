#if !defined(__CMELSPLSMASON3DLG_H__)
#define __CMELSPLSMASON3DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMElsPlsMason3Dlg.h : header file
//

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMason3Dlg dialog

class CCMElsPlsMason3Dlg : public CDialogMove
{
// Construction
public:
	CCMElsPlsMason3Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMElsPlsMason3Dlg)
	enum { IDD = IDD_CMD_PROP_ELS_PLS_MASON3 };
	CEditUnit m_edtMason1;
	CEditUnit m_edtMason2;
	CEditUnit m_edtMason3;
	CEditUnit m_edtMason4;
	CEditUnit m_edtMason5;
	CEditUnit m_edtMason6;

	CTextUnit m_untMason1;
	CTextUnit m_untMason3;
	CTextUnit m_untMason4;
	CTextUnit m_untMason6;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMElsPlsMason3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData(T_EPMT_MASONRY *pData) { m_pData = pData; }

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:
	T_EPMT_MASONRY *m_pData;
	mit::frx::MillustViewer m_wndPicture;

	// Generated message map functions
	//{{AFX_MSG(CCMElsPlsMason3Dlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMELSPLSMASON3DLG_H__)
