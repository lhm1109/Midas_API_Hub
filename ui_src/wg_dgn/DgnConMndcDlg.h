#pragma once

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "HeaderPre.h"

/* Russian  SP_63_2018 : Modify nonlinear deformation model of concrete */
class CDBDoc;

class __MY_EXT_CLASS__ CDgnConMndcDlg : public CMenuBarChildDlg
{
	// Construction
public:
	CDgnConMndcDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(CDgnConMndcDlg)
	enum { IDD = IDD_DGN_CON_MNDC_DLG };

public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDgnConMndcDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConMndcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMndcType();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnSelChangeDiagramType();
	afx_msg void OnUpdateEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//bool HasAlphacc4ShearCtrl(int nAnnex);

	int	m_nOption;
	int	m_nParametersType;
	CComboBox	m_DiagramComb;
	CEditUnit   m_dCompression[6];
	CEditUnit   m_dTension[6];
	CTextUnit	m_dSigmaUnit[6];

	CArray<UINT, UINT> m_arIDCtrl;
	CArray<UINT, UINT> m_arSigma2;
	CArray<UINT, UINT> m_arSigma0;
	
private:
	
};

#include "HeaderPost.h"
