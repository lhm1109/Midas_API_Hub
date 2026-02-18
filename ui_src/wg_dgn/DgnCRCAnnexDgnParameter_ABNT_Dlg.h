#if !defined(__DgnCRCAnnexDgnParameterABNTDlg_H__)
#define __DgnCRCAnnexDgnParameterABNTDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCRCAnnexDgnParameter_ABNT_Dlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "DgnPscCommon.h"

#include "HeaderPre.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg dialog

class __MY_EXT_CLASS__ CDgnCRCAnnexDgnParameterABNTDlg : public CDialogMove
{
	// Construction
public:
	CDgnCRCAnnexDgnParameterABNTDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(CDgnConMatDlg)
	enum { IDD = IDD_DGN_CRC_DESIGN_PARAMETER_ABNT_NBR_ANNEX };

	CEditUnit m_Gamma_c_N_NBR;
	CEditUnit m_Gamma_c_SC_NBR;
	CEditUnit m_Gamma_c_E_NBR;
	CEditUnit m_Gamma_s_N_NBR;
	CEditUnit m_Gamma_s_SC_NBR;
	CEditUnit m_Gamma_s_E_NBR;
	//}}AFX_DATA


public:
	T_PSCD_D* m_pParamData;
	int m_nAnnex;

	void Dlg2Data();
	void Initial_Data();
	//void GetDefaultDatabyAnnex(T_PSCD_D& ParamData, int nAnnex);

private:

protected:
	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDgnConMatDlg)
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

private:
	void SetAlphaCtrl();
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnCRCAnnexDgnParameterABTNDlg_H__4B768343_DD95_11D3_888F_0000C0F30D4D__INCLUDED_)
