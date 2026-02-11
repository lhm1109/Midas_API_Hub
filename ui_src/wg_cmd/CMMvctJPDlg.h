#if !defined(__CMMVCTJPDLG_H__)
#define __CMMVCTJPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctJPDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\MIT_frx\MComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvctJPDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMMvctJPDlg : public CDialogMove
{
public:
	
	CCMMvctJPDlg(CWnd* pParent = NULL);   // standard constructor
 

// Dialog Data
	//{{AFX_DATA(CCMMvctJPDlg)
	enum { IDD = IDD_CMD_ML_MVCT_JAPAN };
	mit::frx::MComboBox	m_cboForceMoment;
	mit::frx::MComboBox	m_cboDisplacement;
	mit::frx::MComboBox	m_cboReaction;
	int		m_nReaction;
	int		m_nDisplacement;
	int		m_nForceMoment;  
	int		m_nFrameOpt;
	int		m_nPlateOpt;
	BOOL	m_bDisplacement;
	BOOL	m_bForceMoment;
	BOOL	m_bReaction;
	BOOL	m_bStressCalc;
	BOOL	m_bStressCalcFrame;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctJPDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CDBDoc* m_pDoc;
	T_MVCTjp_D m_Data;

	void UpdateCtrls();
	void SetData2Dlg();
	BOOL SetDlg2Data();
	void InitComboBox();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMMvctJPDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdMvctOk();
	afx_msg void OnCmdMvctCancel();
	afx_msg void OnCmdMvctCtrls();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CArray<UINT, UINT> arReactionGroup;
	CArray<UINT, UINT> arDisplaceGroup;
	CArray<UINT, UINT> arForceMomGroup;

};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVCTJPDLG_H__19488B30_1026_423F_B720_EBEE876FE673__INCLUDED_)
