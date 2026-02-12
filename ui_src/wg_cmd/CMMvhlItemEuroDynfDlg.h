#if !defined(__CMMVHLITEMEURODYNFDLG_H__)
#define __CMMVHLITEMEURODYNFDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemEuroDynfDlg.h : header file
//

#include "CMChildBarBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemEuroDynfDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMMvhlItemEuroDynfDlg : public CCMChildBarBase, public CDBUpdateConnector
{
// Construction
public:
	CCMMvhlItemEuroDynfDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMMvhlItemEuroDynfDlg();

// Dialog Data
	//{{AFX_DATA(CCMSectionManagerStiffenerSubDlg)
	enum { IDD = IDD_DBAR_DYNF_DLG };

	int       m_nOption;
	int       m_nAutoUser;
	CEditUnit m_edtLength;
	CTextUnit m_untLength;
	int       m_nMaintainType;
	BOOL      m_bEffect;
	CEditUnit m_edtHeight;
	CTextUnit m_untHeight;
	CEditUnit m_edtDynFactor;
	//}}AFX_DATA

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectionManagerStiffenerSubDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//}}AFX_VIRTUAL


// Implementation
public:
	void InitUnit();
	void InitControlsText();
	void Initial_SelectItem();
	void EnableDisableControls();

	virtual void Execute() { OnCMMvhlItemEuroDynfApplyBtn(); };

	BOOL Dlg2Data();

	CDBDoc* m_pDoc;

	T_DYNF_D m_Data;

	CArray<UINT, UINT> m_aAutoControls;
	CArray<UINT, UINT> m_aUserControls;
	CArray<UINT, UINT> m_aAllControls;


	// Generated message map functions
	//{{AFX_MSG(CCMSectionManagerStiffenerSubDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCMMvhlItemEuroDynfOptionRdo();
	afx_msg void OnCMMvhlItemEuroDynfAutoUserRdo();
	afx_msg void OnCMMvhlItemEuroDynfReduceDynfChk();
	afx_msg void OnCMMvhlItemEuroDynfApplyBtn();
	afx_msg void OnCMMvhlItemEuroDynfCloseBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVHLITEMEURODYNFDLG_H__)
