#if !defined(AFX_MPHIGLOBALDLG_H__20F42ABD_566F_4361_8EBD_0C204F318FD9__INCLUDED_)
#define AFX_MPHIGLOBALDLG_H__20F42ABD_566F_4361_8EBD_0C204F318FD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MPhiGlobalDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\wg_base\wg_base_DlgChild.h"
//#include "..\wg_base\wg_base_ChildDialog.h"

#include "..\wg_db\wg_db_SelectLC.h"
#include "MPhiParameterDef.h"

#include "..\mit_frx\MEdit.h"
#include "..\mit_frx\MButton.h"
using namespace mit::frx;

//#include "CMDlgBase.h"

/////////////////////////////////////////////////////////////////////////////
// CMPhiGlobalDlg dialog
class CMPhiGlobalDlg : public CDlgChild
{
// Construction
public:
	CMPhiGlobalDlg(MPhiDataAll* pMPhiDataAll, CWnd* pParent = NULL);   // standard constructor

public:
	CDBDoc* m_pDoc;
	MPhiDataAll* m_pMPhiDataAll;

// Dialog Data
	//{{AFX_DATA(CMPhiGlobalDlg)
	enum { IDD = IDD_CMD_MPHI_GLOBAL_DLG };

	MComboBox	m_cLoadCaseCobx;
	int		m_nInitForceType;
	int		m_nYieldPoint;
	MEdit		m_edtNoSectDivision;
	CFormulaEditSpin m_edtMaxIteration;
	MEdit		m_edtTolerance;
	MButton	m_Tendonchk;
	MButton	m_Shiftchk;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMPhiGlobalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Dlg2Data(); 

protected:
	BOOL Data2Dlg();
	
	BOOL Apply();
	void InitComboBox();
	void AdjustComboListBox();  
	void AdjustComboListBox(CComboBox& Cbx) /* 콤보박스의 길이를 현재 사용되고 있는 리스트의 길이에 맞추어 변경한다. */;
	void EnableCtrl();
	// Generated message map functions
	//{{AFX_MSG(CMPhiGlobalDlg)
	//afx_msg void OnRemoveDataBtn();
	//afx_msg void OnApplyBtn();
	afx_msg void OnEquilibriumRdo();
	afx_msg void OnLoadcaceRdo();	
	afx_msg void OnTypeIiRdo();  	
	afx_msg void OnChkCurve();  	
	afx_msg void OnDestroy();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	//virtual void OnCancel();
	//virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MPHIGLOBALDLG_H__20F42ABD_566F_4361_8EBD_0C204F318FD9__INCLUDED_)
