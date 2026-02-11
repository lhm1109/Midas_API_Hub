#if !defined(__ETCACTLTRUSSELEMDLG_H__)
#define __ETCACTLTRUSSELEMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EtcActlDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CEtcActlTrussElemDlg dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CEtcActlTrussElemDlg : public CDialogMove
{
	// Construction
public:
	CEtcActlTrussElemDlg(CWnd* pParent = NULL);   // standard constructor

	void SetData(T_ACTL_D& data);
	void GetData(T_ACTL_D& data);

	
	enum { IDD = IDD_ETC_ACTL_TRUSS_ELEM };
	CTextUnit	m_unitExTolerance;
	CEditUnit	m_edtExTolerance;
	CFormulaEditSpin	m_edtExLoadCase;

	int				m_nLoadStepType;
	CEditUnit		m_edtNumberOfIncrements;
	MComboBox		m_cmbIntermediateOutputRequest;

	BOOL			m_bDisp;
	BOOL			m_bLoad;
	BOOL			m_bWork;
	CEditUnit		m_edtDisp;
	CEditUnit		m_edtLoad;
	CEditUnit		m_edtWork;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEtcActlDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitUnit();
	void InitComboBox();
	void InitDialogData();
	void EnableDisableControls();

	T_ACTL_D m_Data;
protected:
	CDBDoc* m_pDoc;

	CArray<UINT, UINT> m_arIncludeType;
	CArray<UINT, UINT> m_arExcludeType;

	// Generated message map functions
	//{{AFX_MSG(CEtcActlDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdNlctAcasLoadStepTypeRdo();
	afx_msg void OnCmdNlctAcasDispChk();
	afx_msg void OnCmdNlctAcasLoadChk();
	afx_msg void OnCmdNlctAcasWorkChk();
	afx_msg void OnCmdNlctAcasLoadStepsBtn();
	afx_msg void OnCmdNlctAcasAdvancedBtn();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetCtrlText() const;
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ETCACTLTRUSSELEMDLG_H__)

