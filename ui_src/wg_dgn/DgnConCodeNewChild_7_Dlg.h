#if !defined(__DGN_CON_CODE_NEW_CHILD_7_DLG_H__)
#define __DGN_CON_CODE_NEW_CHILD_7_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConCodeNewChild_7_Dlg.h.h : header file
//
#include "../wg_db/DB_ST_DN_JUD.h"
#include "DconChildDialog.h"
#include "HeaderPre.h"


class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild7Dlg dialog
class __MY_EXT_CLASS__ CDgnConCodeNewChild7Dlg : public CDconChildDialog
{
// Construction
public:
	CDgnConCodeNewChild7Dlg(CDgnConCodeNewDlg* pParent = NULL, T_DCON_D* pData=NULL, BOOL bExistData=FALSE);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConCodeNewChild7Dlg)
	enum { IDD = IDD_DGN_CON_CODE_CHILD7_DLG };
	
	int     m_iAlphaType1;
	double  m_dAlpha1;
	double  m_dAlpha2;
	double  m_dMrft;

	int  m_nBeamEndLocRC;
	int  m_nColmEndLocRC;
	int  m_nBeamEndLocRC_ST;
	int  m_nColmEndLocRC_ST;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConCodeNewChild7Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	virtual void ChangeDesignCode();
	virtual void UpdateChildData();
	virtual void ResizeChildDlg();
	virtual BOOL Dlg2Data();
protected:
	CArray<UINT, UINT> m_aShearforDesign;
	CArray<UINT, UINT> m_aSelectMethod;
	CArray<UINT, UINT> m_aMomentRedistr;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConCodeNewChild7Dlg)
	void InitCtrl();
	void SetEQCtrl(const CString& strCode);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnRcctrlSpecial();
	afx_msg void OnChangeAlphaType();
	afx_msg void OnUpdateByCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGN_CON_CODE_NEW_CHILD_7_DLG_H__)
