#if !defined(__RatingParamPSCLSD15Annex_H__)
#define __RatingParamPSCLSD15Annex_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCRCAnnexDgnParameterDlg_LSD.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "DgnPscCommon.h"


#include "HeaderPre.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg dialog

class __MY_EXT_CLASS__ CRatingParamPSCLSDAnnex : public CDialogMove
{
	// Construction
public:
	CRatingParamPSCLSDAnnex(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDgnConMatDlg)
	enum { IDD = IDD_DGN_RATING_PSC_DESIGN_PARAMETER_LSD_ANNEX };

	CEditUnit m_dPartialFac_c_U;
	CEditUnit m_dPartialFac_s_U;
	CEditUnit m_dPartialFac_ps_U;
	CEditUnit m_dPartialFac_c_S;
	CEditUnit m_dPartialFac_s_S;
	CEditUnit m_dPartialFac_ps_S;

	void Dlg2Data();
	void Initial_Data();
	//void GetDefaultDatabyAnnex(T_PSCD_D& ParamData, int nAnnex);
	T_RAKR_D m_Data;
	CDBDoc* m_pDoc;
//private:
//
//protected:
//	// Overrides
//	// ClassWizard generated virtual function overrides
//	//{{AFX_VIRTUAL(CDgnConMatDlg)
protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDgnConMatDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnResetDgnParam_byAnnex();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX__RatingParamPSCLSD15Annex_H__4B768343_DD95_11D3_888F_0000C0F30D4D__INCLUDED_)
