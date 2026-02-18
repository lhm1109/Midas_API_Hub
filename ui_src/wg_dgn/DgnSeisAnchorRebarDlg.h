#if !defined(__DGNSEISANCHORREBARDLG_H__)
#define __DGNSEISANCHORREBARDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorRebarDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorRebarDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnSeisAnchorRebarDlg : public CDialogMove
{
// Construction
public:
	CDgnSeisAnchorRebarDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorRebarDlg)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_REBAR_DLG };
	
	BOOL      m_bTension;
	CEditUnit m_edtFyat;
	CTextUnit m_untFyat;
	CEditUnit m_edtdsat;
	CTextUnit m_untdsat;
	BOOL      m_bShear;
	CEditUnit m_edtFyasL;
	CTextUnit m_untFyasL;
	CEditUnit m_edtdsasL;
	CTextUnit m_untdsasL;
	CEditUnit m_edtNasL;
	CEditUnit m_edtFyasT;
	CTextUnit m_untFyasT;
	CEditUnit m_edtdsasT;
	CTextUnit m_untdsasT;
	CEditUnit m_edtNasT;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorRebarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitUnit();

	void EnableDisableControls();

	void Data2Dlg();
	BOOL Dlg2Data();

public:
	T_ANEV_REBAR_D m_Data;

protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorRebarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSeisAnchorRebarTensionChk();
	afx_msg void OnDgnSeisAnchorRebarShearChk();
	afx_msg void OnDgnSeisAnchorRebarOKBtn();
	afx_msg void OnDgnSeisAnchorRebarCancelBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	MillustViewer m_wndPicture;
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISANCHORREBARDLG_H__)
