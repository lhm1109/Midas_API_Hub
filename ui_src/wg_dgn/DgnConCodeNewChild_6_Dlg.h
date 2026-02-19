#if !defined(__DGN_CON_CODE_NEW_CHILD_6_DLG_H__)
#define __DGN_CON_CODE_NEW_CHILD_6_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConCodeNewChild_6_Dlg.h.h : header file
//

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "DconChildDialog.h"
#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild6Dlg dialog
class CDgnDataCtrl;
class __MY_EXT_CLASS__ CDgnConCodeNewChild6Dlg : public CDconChildDialog
{
// Construction
public:
	CDgnConCodeNewChild6Dlg(CDgnConCodeNewDlg* pParent = NULL, T_DCON_D* pData=NULL, BOOL bExistData=FALSE);   // standard constructor
    virtual ~CDgnConCodeNewChild6Dlg();

// Dialog Data
	//{{AFX_DATA(CDgnConCodeNewDlg)
	enum { IDD = IDD_DGN_CON_CODE_CHILD6_DLG };
	
	int     m_iAlphaType1;
	double  m_dAlpha1;
	double  m_dAlpha2;
	double  m_dMrft;

	BOOL	  m_bTorsionDesign;

	int     m_iIs13920Sel;

	CEditUnit m_dedtFOS; 
	CEditUnit m_dedtSubRex;
	CTextUnit	m_ctrlUnit;

	int		m_nPMCurveMethod;
    BOOL    m_bBCJointDgn;
    int     m_iSCWBLast;
    BOOL    m_bStrongColmWeakBeam;
	BOOL    m_bConsiderBECheck;
	int     m_nBEEBarDesignMethod;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConCodeNewChild6Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	virtual void UpdateChildData();
	virtual void ChangeDesignCode();
	virtual void ResizeChildDlg();
	virtual BOOL Dlg2Data();

protected:
	CArray<UINT, UINT> m_aDuctileDesign;
	CArray<UINT, UINT> m_aShearforDesign;
	CArray<UINT, UINT> m_aMomentRedistr;
	CArray<UINT, UINT> m_aMatProperty;
	CArray<UINT, UINT> m_aPMCurveMethod;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConCodeNewChild6Dlg)
	void InitCtrl();
	void SetEQCtrl(const CString& strCode);
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnRcctrlSpecial();
	afx_msg void OnChangeAlphaType();
	afx_msg void OnUpdateByCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedDgnIs13920_1993();
    afx_msg void OnBnClickedDgnIs13920_2016();
    afx_msg void OnBnClickedDgnPmCurveMethodPRadio();
    afx_msg void OnBnClickedDgnPmCurveMethodMpRadio();
    afx_msg void OnBnClickedDgnIs456ScwbLaftFlr();
	afx_msg void OnBnConsiderBEDesign();
	afx_msg void OnBnClickedMethodARadio();
	afx_msg void OnBnClickedMethodBRadio();
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGN_CON_CODE_NEW_CHILD_6_DLG_H__)
