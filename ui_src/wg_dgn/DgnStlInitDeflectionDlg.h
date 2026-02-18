#pragma once

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

class CDBDoc;

class CDgnStlInitDeflectionDlg : public CMenuBarChildDlg
{
public:
	CDgnStlInitDeflectionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();

// Dialog Data
	//{{AFX_DATA(CDgnStlPhibFacDlg)
	enum{IDD = IDD_DGN_STL_INDF_DLG};
	int		m_nOption;
	//double	m_dOmegaWy;
	//double	m_dOmegaWz;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	BOOL ErrorCheck();
	//virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlPhibFacDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	CEditUnit m_edtOmegaWy;
	CTextUnit m_untOmegaWy;
	CEditUnit m_edtOmegaWz;
	CTextUnit m_untOmegaWz;
// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CDgnStlPhibFacDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

