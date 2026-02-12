
#pragma once

/////////////////////////////////////////////////////////////////////////////
// ResponseDispLoadMode4Dlg dialog
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\wg_base\DlgChild.h"
#include "StldNbofDlg.h"



class ResponseDispLoadMode4Dlg : public CStldNbofDlg
{
// Construction
public:
	ResponseDispLoadMode4Dlg(CWnd* pParent = NULL);   // standard constructor
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
		virtual BOOL IsAdd2ListCase(const T_STLD_D &Stld);
	// Generated message map functions
	//{{AFX_MSG(ResponseDispLoadMode4Dlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
