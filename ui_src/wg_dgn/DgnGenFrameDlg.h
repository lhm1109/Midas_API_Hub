#if !defined(__DGNGENFRAMEDLG_H__)
#define __DGNGENFRAMEDLG_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenFrameDlg.h : header file
//
#include "..\MIT_frx\MComboBox.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "wg_dgnRes2.h"

#include "HeaderPre.h"

using namespace mit::frx;

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnGenFrameDlg dialog

class __MY_EXT_CLASS__ CDgnGenFrameDlg : public CDialogMove
{
// Construction
public:
	CDgnGenFrameDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnGenFrameDlg)
	enum { IDD = IDD_DGN_GEN_FRAME_DLG };
	BOOL	m_bAutoK;
	int		m_XBraced;
	int		m_YBraced;
	int		m_DgnType;
	MComboBox m_cmbMtyp;
	MComboBox m_cmbStyp;
	BOOL  m_b02Q0; //add by maxiao
	double m_d02Q0;//add by maxiao
	//}}AFX_DATA

public:

protected:
	void SetMtypCombo();
	void SetStypCombo_GB10(int nStructMatlType);

	void SetStructTypeControl();

public:
	void Initial_Data();
	void Initial_ExistData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenFrameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;  

	// Generated message map functions
	//{{AFX_MSG(CDgnGenFrameDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDgnCtrldataXbraced();
	afx_msg void OnDgnCtrldataXunbraced();
	afx_msg void OnDgnCtrldataYbraced();
	afx_msg void OnDgnCtrldataYunbraced();
	afx_msg void OnDgnCtrldataAutok();
	afx_msg void OnSelChangeStypCmb();//add by maxiao
	afx_msg void OnDgnCtrlCheck02Q0();//add by maxiao
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENFRAMEDLG_H__B1565522_E5DB_11D3_888F_0000C0F30D4D__INCLUDED_)
