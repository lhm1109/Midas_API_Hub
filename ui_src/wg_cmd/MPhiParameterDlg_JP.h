#if !defined(__CMD_MPHIPARAMETERDLG_JP_H__)
#define __CMD_MPHIPARAMETERDLG_JP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MPhiParameterDlg_JP.h : header file

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "MPhiParameterDef.h"

#include "HeaderPre.h"

class CDBDoc;
class CMPhiGlobalDlg;
class CMPhiHingeDlg;
class CMPhiMaterialDlg_JP;
class CMPhiSectionDlg_JP;

class __MY_EXT_CLASS__ CMPhiParameterDlg_JP : public CDialogMove, public CDBUpdateConnector
{
//Attribute
private:
	int m_nKind;	// m_nKind : (0)MPGB (1)MPHG (2)MPST
	UINT m_nModifyKey;
	MPhiDataAll m_DataAll;
	
	CDBDoc* m_pDoc;
	CDlgTabCtrl	m_cTabCtrl;
	// sub dialogs....
	CMPhiGlobalDlg* m_pDlgGlobal;
	CMPhiHingeDlg* m_pDlgHinge;
	CMPhiMaterialDlg_JP* m_pDlgMaterial;
	CMPhiSectionDlg_JP* m_pDlgSection;
// Construction
public:
	CMPhiParameterDlg_JP(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMPhiParameterDlg_JP();

	// Dialog Data
	//{{AFX_DATA(CMPhiParameterDlg_JP)
	enum { IDD = IDD_CMD_MPHI_CALC_DLG_JP };

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_DATA

// Overrides
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMPhiParameterDlg)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
	
// Implementation
	// Generated message map functions
	//{{AFX_MSG(CMPhiParameterDlg)
	afx_msg void OnSelchangeCmdTabCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	// nKind : (0)MPGB (1)MPHG (2)MPST
	void SetParamData(int nKind, UINT Key);

private:
	void SetMembDataByDB();
	void UpdateBuffer(LPARAM lHint, CObject* pHint);
};

#include "HeaderPost.h"

#endif // !defined(__CMD_MPHIPARAMETERDLG_JP_H__)