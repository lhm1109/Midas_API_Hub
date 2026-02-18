#if !defined(__DgnCRCAnnexDgnParameterDlg_H__)
#define __DgnCRCAnnexDgnParameterDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCRCAnnexDgnParameterDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "DgnPscCommon.h"

#include "HeaderPre.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg dialog

class __MY_EXT_CLASS__ CDgnCRCAnnexDgnParameterDlg : public CDialogMove
{
// Construction
public:
	CDgnCRCAnnexDgnParameterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConMatDlg)
	enum { IDD = IDD_DGN_CRC_DESIGN_PARAMETER_EC2_ANNEX };

	CEditUnit m_dPartialFac_c_PT;
	CEditUnit m_dPartialFac_c_A;
	CEditUnit m_dPartialFac_c_S;
	CEditUnit m_dPartialFac_s_PT;
	CEditUnit m_dPartialFac_s_A;
	CEditUnit m_dPartialFac_s_S;
	CEditUnit m_dPartialFac_ps_PT;
	CEditUnit m_dPartialFac_ps_A;
	CEditUnit m_dPartialFac_ps_S;
	//
	CEditUnit m_dAlpha_cc;
    CEditUnit m_dAlpha_cc_4Shear;
	CEditUnit m_dAlpha_ct;
    //
	CEditUnit m_dStressLim_c_k1;
	CEditUnit m_dStressLim_c_k3;
	CEditUnit m_dStressLim_c_k4;
	CEditUnit m_dStressLim_c_k6;
	CEditUnit m_dStressLim_s_k1;
	CEditUnit m_dStressLim_s_k2;
	CEditUnit m_dStressLim_s_k5;
	CEditUnit m_dStressLim_s_k7;
	CEditUnit m_dStressLim_s_k8;
	//
	CEditUnit m_dk_c_ps_stage;
	CEditUnit m_dk_t_ps_stage;
	CEditUnit m_dk_c_ps;
	CEditUnit m_dk_t_ps;
	//
	CEditUnit m_dk3_cr;
	CEditUnit m_dk4_cr;
	CEditUnit m_edtk1_Fatigue;
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
    bool HasAlphacc4ShearCtrl();
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnCRCAnnexDgnParameterDlg_H__4B768343_DD95_11D3_888F_0000C0F30D4D__INCLUDED_)
