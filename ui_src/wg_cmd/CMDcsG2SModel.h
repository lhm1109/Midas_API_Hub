#if !defined(AFX_CMDCSG2SMODEL_H__940B6D3F_AA2B_42C4_B456_DEBAFABD0FB4__INCLUDED_)
#define AFX_CMDCSG2SMODEL_H__940B6D3F_AA2B_42C4_B456_DEBAFABD0FB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMDcsG2SModel.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CCMDcsG2SModel dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMDcsG2SModel : public CDialogMove
{
// Construction
public:
	CCMDcsG2SModel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMDcsG2SModel)
	enum { IDD = IDD_DCS_GEN2SDS_MODEL };
	CEdit	m_edtStiffnessFactor;
	CComboBox	m_cboStory;
	CEdit	m_edtFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMDcsG2SModel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	
	void InitCtrlData();
	void InitStorData();

	// Generated message map functions
	//{{AFX_MSG(CCMDcsG2SModel)
	afx_msg void OnCmdG2smodelFileBtn();
	afx_msg void OnCmdG2smodelOk();
	afx_msg void OnCmdG2smodelCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDCSG2SMODEL_H__940B6D3F_AA2B_42C4_B456_DEBAFABD0FB4__INCLUDED_)
