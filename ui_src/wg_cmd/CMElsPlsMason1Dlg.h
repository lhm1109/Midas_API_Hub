#if !defined(__CMELSPLSMASON1DLG_H__)
#define __CMELSPLSMASON1DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMElsPlsMason1Dlg.h : header file
//

#include "..\MIT_frx\MillustViewer.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMason1Dlg dialog
struct T_EPMT_MASONRY;
class CCMElsPlsMason1Dlg : public CDialogMove
{
// Construction
public:
	CCMElsPlsMason1Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMElsPlsMason1Dlg)
	enum { IDD = IDD_CMD_PROP_ELS_PLS_MASON1 };
	CEditUnit m_edtMason1;
	CEditUnit m_edtMason2;
	// CEditUnit m_edtMason3;
	CEditUnit m_edtMason4;
	CEditUnit m_edtMason5;
	// CEditUnit m_edtMason6;
	// CEditUnit m_edtMason7;
	// CEditUnit m_edtMason8;
	// CEditUnit m_edtMason9;
	CEditUnit m_edtMason10;
	CEditUnit m_edtMason11;
	// CEditUnit m_edtMason12;
	// CEditUnit m_edtMason13;

	CTextUnit m_untMason1;
	CTextUnit m_untMason4;
	CTextUnit m_untMason5;
	CTextUnit m_untMason11;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMElsPlsMason1Dlg)
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
	//{{AFX_MSG(CCMElsPlsMason1Dlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMELSPLSMASON1DLG_H__)
