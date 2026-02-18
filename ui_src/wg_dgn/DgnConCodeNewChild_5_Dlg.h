#if !defined(__DGN_CON_CODE_NEW_CHILD_5_DLG_H__)
#define __DGN_CON_CODE_NEW_CHILD_5_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConCodeNewChild_5_Dlg.h.h : header file
//
#include "..\mit_frx\MButton.h"

#include "DconChildDialog.h"
#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild5Dlg dialog
class CDgnDataCtrl;
class __MY_EXT_CLASS__ CDgnConCodeNewChild5Dlg : public CDconChildDialog
{
// Construction
public:
	CDgnConCodeNewChild5Dlg(CDgnConCodeNewDlg* pParent = NULL, T_DCON_D* pData=NULL, BOOL bExistData=FALSE);   // standard constructor
    virtual ~CDgnConCodeNewChild5Dlg();

// Dialog Data
	//{{AFX_DATA(CDgnConCodeNewChild5Dlg)
	enum { IDD = IDD_DGN_CON_CODE_CHILD5_DLG };
	
	BOOL    m_bStrongColmWeakBeam;
	BOOL	  m_bTorsionDesign;
	int     m_iAlphaType1;
	double  m_dAlpha1;
	double  m_dAlpha2;
	double  m_dConShearStrengthRatio;
	double  m_dMrft;
	double  m_dTrft;

	mit::frx::MButton	  m_btnTorsion;

	int		m_nPMCurveMethod;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConCodeNewChild5Dlg)
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
	CArray<UINT, UINT> m_aStrongColmWeakBeam;
	CArray<UINT, UINT> m_aSCWBDesign;
	CArray<UINT, UINT> m_aTorsionDesign;
	CArray<UINT, UINT> m_aMomentRedistr;
	CArray<UINT, UINT> m_aRdoSCWBMethod;
	CArray<UINT, UINT> m_aPMCurveMethod;

    CDgnDataCtrl* m_pDgnDataCtrl;

//   T_DCON_D* m_pData;
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConCodeNewDlg)
	void InitCtrl();
	void SetEQCtrl(const CString& strCode);
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnRcctrlSpecial();
	afx_msg void OnChangeAlphaType();
	afx_msg void OnChangeTorsion();
	afx_msg void OnUpdateByCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGN_CON_CODE_NEW_CHILD_5_DLG_H__)
