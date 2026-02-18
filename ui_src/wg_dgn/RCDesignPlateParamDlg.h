#if !defined(__RCDESIGNPLATEPARAMDLG_H__)
#define __RCDESIGNPLATEPARAMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CRCDesignPlateParamDlg.h : header file
//

#include "DgnDlgBase.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\DB_ST_DN.h"

/////////////////////////////////////////////////////////////////////////////
// CRCDesignPlateParamDlg dialog



class CRCPlateParameter;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CRCDesignPlateParamDlg : public CDgnDlgBase, public CDBUpdateConnector //CDialogMove
{
// Construction
public:
	CRCDesignPlateParamDlg(CWnd* pParent = NULL);   // standard constructor
	~CRCDesignPlateParamDlg();
	
// interface
public:
	//void SetParamData(T_RPSC_K RpscK);
	//void UpdateView(BOOL bEndI);

// Dialog Data
	//{{AFX_DATA(CRCDesignPlateParamDlg)
	enum { IDD = IDD_DGN_CON_DESIGN_PARAM_PLATE_BASE };	

	CDlgTabCtrl	m_ctrlTab;  
	CString	m_strDesignCode;
	//}}AFX_DATA
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCDesignPlateParamDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	BOOL m_bGroupMessage;

protected:
	CDBDoc* m_pDoc;

	BOOL m_bModify;

	CRCPlateParameter* m_pPlateDlg;

	void LayoutChildDlg();
	//void ChangeItem(int nIndex);

	void EnableCalcBtn(bool Enable);
	void ChangeLoadCombination();

	void SetRCDesignCodeName();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRCDesignPlateParamDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnClose();
	afx_msg void OnDgnModifyConcMateBtn();
	afx_msg void OnDgnModifyMemberTypeBtn();
	afx_msg void OnSelchangeDesigncode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__RCDESIGNPLATEPARAMDLG_H__)
