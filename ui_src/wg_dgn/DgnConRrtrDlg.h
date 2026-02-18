#pragma once

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "HeaderPre.h"

/* Russian  SP_63_2018 : Rus Rcs Transverse reinforcement */
class CDBDoc;

class __MY_EXT_CLASS__ CDgnConRrtrDlg : public CMenuBarChildDlg
{
	// Construction
public:
	CDgnConRrtrDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(CDgnConRrtrDlg)
	enum { IDD = IDD_DGN_CON_RRTR_DLG };

public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDgnConRrtrDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConRrtrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPunchType();
	afx_msg void OnShearType();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//bool HasAlphacc4ShearCtrl(int nAnnex);

	int	m_nOption;
	int	m_nPunchReinforcement;
	int	m_nShearReinforcement;

	CEditUnit   m_ValueEdit[4];
	CTextUnit	m_ValueUnit[4];

	CTextUnit	m_ValueUnit2[2];

	CArray<UINT, UINT> m_arIDCtrl;
	CArray<UINT, UINT> m_arIDCtr2;
	
private:
	
};

#include "HeaderPost.h"
