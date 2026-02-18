#if !defined(__DgnCRCDgnParameter_EC2PSC_H__)
#define __DgnCRCDgnParameter_EC2PSC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCRCDgnParameter_EC2PSC.h : header file
//

#include "..\wg_cmd\MyChildDialog.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnCRCDgnParameter_EC2PSC dialog
#include "HeaderPre.h"

class CDgnCRCAnnexDgnParameterDlg;

class __MY_EXT_CLASS__ CDgnCRCDgnParameter_EC2PSC : public CMyChildDialog
{
	// Construction
public:
	CDgnCRCDgnParameter_EC2PSC(T_PSCD_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	~CDgnCRCDgnParameter_EC2PSC(); 

	// Dialog Data
	//{{AFX_DATA(CDgnCRCDgnParameter_EC2PSC)
	enum { IDD = IDD_DGN_CRC_DESIGN_PARAMETER_EC2PSC };
	//
	mit::frx::MButton   m_ChkOutputOpt_ULS1;
	mit::frx::MButton   m_ChkOutputOpt_ULS2;
	mit::frx::MButton   m_ChkOutputOpt_ULS3;
	mit::frx::MButton   m_ChkOutputOpt_ULS4;
	mit::frx::MButton   m_ChkOutputOpt_SLS1;
	mit::frx::MButton   m_ChkOutputOpt_SLS2;
	mit::frx::MButton   m_ChkOutputOpt_SLS3;
	mit::frx::MButton   m_ChkOutputOpt_SLS4;
	mit::frx::MButton   m_ChkOutputOpt_SLS5;
	mit::frx::MButton   m_ChkOutputOpt_SLS6;
	mit::frx::MButton   m_ChkUserInputData;
	//
	mit::frx::MButton   m_RadioTndCalcMethod;
	mit::frx::MButton   m_RadioTndCalcMethod2;
	mit::frx::MButton   m_RadioShearMethod;
	mit::frx::MButton   m_RadioShearMethod2;
	//
	mit::frx::MComboBox m_CmbExposureClass;
	//
	mit::frx::MComboBox m_CmbCementClass;
	CEditUnit m_dStrutAngle;
	int       m_nPSType;

    CEditUnit m_EdtDg;
    CTextUnit m_UntDg;
    CEditUnit m_EdtKc;
    mit::frx::MComboBox m_CmbPL;
    mit::frx::MComboBox m_CmbSL;
    mit::frx::MComboBox m_CmbSType;
    mit::frx::MComboBox m_CmbBType;

	//}}AFX_DATA

public:
	T_PSCD_D* m_pParamData;
	BOOL Dlg2Data();
	//
	void Initial_Data();
	//void InitExposureClassComboSet();
	void InitCementClassComboSet();
	int  m_nAnnex;

protected:
	CArray<UINT, UINT> m_aBetaCalType;




	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnCRCDgnParameter_EC2PSC)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnCRCDgnParameter_EC2PSC)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickModifyDgnParam();
	afx_msg void OnClickSelectAllOutputOpt();
	afx_msg void OnClickUnselectAllOutputOpt();
	afx_msg void OnCheckUserInputData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    void SetAnnexCtrl();

private:
    void InitAnnexCtrl();
    void InitAnnexData();
    void ShowAnnexCtrl() const;
    void MoveWmaxCmbCtrl() const;
    bool IsExistWmaxCtrl() const;
    void ResizeOtherCtrl() const;
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnCRCDgnParameter_EC2PSC_H__)