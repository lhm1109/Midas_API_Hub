#if !defined(__DGNSEISMPHIAUTOAXIALFORCEDLG_H__)
#define __DGNSEISMPHIAUTOAXIALFORCEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisMPhiAutoAxialForceDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_SelectLC.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiAutoAxialForceDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnSeisMPhiAutoAxialForceDlg : public CDialogMove
{
// Construction
public:
	CDgnSeisMPhiAutoAxialForceDlg(CWnd* pParent = NULL);   // standard constructor  
	virtual ~CDgnSeisMPhiAutoAxialForceDlg();
	
	//----------------------------------------------------------------------
	// Interface
public:
	// Dialog Data
	//{{AFX_DATA(CDgnSeisMPhiAutoAxialForceDlg)
	enum { IDD = IDD_DGN_SEIS_MPHI_AUTO_AXIAL_FORCE_DLG };

	CSelectLC m_cmbLcom;
	CEditUnit m_edtAxialForce;
	CTextUnit m_untAxialForce;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisMPhiAutoAxialForceDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

//----------------------------------------------------------------------
// Implementation
public:
	T_EVGP_K m_EvgpK;
	int      m_nEvgpColm;
	int      m_nEvgpPos;
	double   m_dAxialForce;

protected:
	void InitUnit();
	void InitCombo();

	CDBDoc* m_pDoc;
				
	// Generated message map functions
	//{{AFX_MSG(CDgnSeisMPhiAutoAxialForceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSeisMPhiLcomCmb();
	afx_msg void OnDgnSeisMPhiLcomBtn();
	afx_msg void OnDgnSeisMPhiOKBtn();
	afx_msg void OnDgnSeisMPhiCanCelBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISMPHIAUTOAXIALFORCEDLG_H__)
