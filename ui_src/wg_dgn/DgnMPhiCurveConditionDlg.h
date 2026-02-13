#if !defined(AFX_DGNMPHICURVECONDITIONDLG_H__DE99E2AA_1F5C_4560_B233_AA15FF23E8D4__INCLUDED_)
#define AFX_DGNMPHICURVECONDITIONDLG_H__DE99E2AA_1F5C_4560_B233_AA15FF23E8D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnMPhiCurveConditionDlg.h : header file
//

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnMPhiCurveConditionDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnMPhiCurveConditionDlg : public CDialogMove
{
// Construction
public:
	CDgnMPhiCurveConditionDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnMPhiCurveConditionDlg();   // standard deconstruction

// Dialog Data
	//{{AFX_DATA(CDgnMPhiCurveConditionDlg)
	enum { IDD = IDD_DGN_MPHI_CURVE_CONDITION_DLG };

	BOOL      m_becu;
	int       m_necuType;
	CEditUnit m_edtecu_User;
	BOOL      m_bfck85;
	BOOL      m_beru_tens;
	BOOL      m_beru_comp;
	int       m_neruType;
	CEditUnit m_edteru_comp;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnMPhiCurveConditionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void ControlsEnableDisable();

	void Data2Dlg();
	BOOL Dlg2Data();

	CDBDoc* m_pDoc;

	T_ECUC_D m_Data;

	// Generated message map functions
	//{{AFX_MSG(CDgnMPhiCurveConditionDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDgnMPhiCurveConditionecuChk();
	afx_msg void OnDgnMPhiCurveConditionecutypeRdo();
	afx_msg void OnDgnMPhiCurveConditioneru_compChk();
	afx_msg void OnDgnMPhiCurveConditionerutypeRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNMPHICURVECONDITIONDLG_H__DE99E2AA_1F5C_4560_B233_AA15FF23E8D4__INCLUDED_)
