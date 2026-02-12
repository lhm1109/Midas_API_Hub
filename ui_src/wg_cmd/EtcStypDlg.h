#if !defined(__ETCSTYPDLG_H__)
#define __ETCSTYPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EtcStypDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CEtcStypDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CEtcStypDlg : public CDialogMove
{
// Construction
public:
	CEtcStypDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEtcStypDlg)
	enum { IDD = IDD_ETC_STYP };
	CEditUnit	m_ctrlIniTemp;
	CEditUnit	m_ctrlGravity;
	CTextUnit	m_ctrlIniTempUnit;
	CTextUnit	m_ctrlGravUnit;
	int		m_nStructType;
	int   m_nMassType;
	int		m_nStructureMass;
	BOOL  m_bConsiderOffset;
	BOOL  m_bConvertSelfWeight; 
	BOOL	m_bAlignBeam;
	BOOL	m_bAlignSlab;
	BOOL	m_bConsiderRotMPF;    // Consider Rotational Rigid Body Mode for Modal Participation Factor
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEtcStypDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitDialogData();
	void CtrlManager();
	CArray<UINT, UINT> m_aCtrlLumpedMass, m_aCtrlConsiderOffset;

protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CEtcStypDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnEtcPcMassType();
	afx_msg void OnEtcPcStructMass();
	afx_msg void OnEtcPcSelfWeight();
	afx_msg void OnEtcPcConsiderOffset(); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ETCSTYPDLG_H__)
