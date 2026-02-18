#if !defined(__DGNSTLCODEJPDLG_H__)
#define __DGNSTLCODEJPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlCodeJPDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "DgnStlCodeKRDlg.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

#include "HeaderPre.h"


// struct T_ENVELOPE_DATA
// {
// 	BOOL bP_max;
// 	BOOL bP_min;
// 	BOOL bVy_max;
// 	BOOL bVy_min;
// 	BOOL bVz_max;
// 	BOOL bVz_min;
// 	BOOL bT_max;
// 	BOOL bT_min;
// 	BOOL bMy_max;
// 	BOOL bMy_min;
// 	BOOL bMz_max;
// 	BOOL bMz_min;
// 
// 	void Initialize()
// 	{
// 		bP_max = TRUE;
// 		bP_min = TRUE;
// 		bVy_max = TRUE;
// 		bVy_min = TRUE;
// 		bVz_max = TRUE;
// 		bVz_min = TRUE;
// 		bT_max = TRUE;
// 		bT_min = TRUE;
// 		bMy_max = TRUE;
// 		bMy_min = TRUE;
// 		bMz_max = TRUE;
// 		bMz_min = TRUE;
// 	}
// };

using namespace mit::frx;
class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnStlCodeJPDlg dialog
class __MY_EXT_CLASS__ CDgnStlCodeJPDlg : public CDialogMove
{
// Construction
public:
	CDgnStlCodeJPDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnStlCodeJPDlg)
	enum { IDD = IDD_DGN_STL_CODE_JP_DLG };
	MComboBox	m_DesignCode;
	
	MButton	m_radioSectForce1;
	MButton	m_radioSectForce2;
	MButton	m_radioSectForce3;

	MButton	m_chkSectCheck1;
	MButton	m_chkSectCheck2;
	MButton	m_chkSectCheck3;

	MButton	m_btnEnvelope;
	MButton	m_btnSelectAll;
	MButton	m_btnUnSelectAll;
	//}}AFX_DATA
	int m_nShearStressMethod;
	int m_nDesignForceMethod;
	
public:

	T_ENVELOPE_DATA m_EnvelopeData;

	void Initial_ExistData();
	void Initial_Data();
	void InitCtrl();
	void EnableDisable_Ctrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlCodeJPDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlCodeJPDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnChangeDgnCodeCmb();
	afx_msg void OnDgnEnvelopeBtn();
	afx_msg void OnDgnSelectAllBtn();
	afx_msg void OnDgnUnSelectAllBtn();
	afx_msg void OnDgnSectionForceRadio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CArray<UINT, UINT> m_aPrintCtrlLst;
};

#include "HeaderPost.h"

#endif 