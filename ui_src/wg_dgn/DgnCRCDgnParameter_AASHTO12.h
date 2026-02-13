#if !defined(__DgnCRCDgnParameter_AASHTO12_H__)
#define __DgnCRCDgnParameter_AASHTO12_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCRCDgnParameter_AASHTO12.h : header file
//

#include "..\wg_cmd\MyChildDialog.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\MIT_frx\MButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnCRCDgnParameter_AASHTO12 dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnCRCDgnParameter_AASHTO12 : public CMyChildDialog
{
	// Construction
public:
	CDgnCRCDgnParameter_AASHTO12(T_PSCD_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	~CDgnCRCDgnParameter_AASHTO12(); 
	
	// Dialog Data
	//{{AFX_DATA(CDgnCRCDgnParameter_EC2PSC)
	enum { IDD = IDD_DGN_CRC_DESIGN_PARAMETER_AASHTO12};

	CEditUnit m_edtExposureUser;

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
	
	//}}AFX_DATA
	
	
	// Overrides
	//{{AFX_VIRTUAL(CDgnCRCDgnParameter_AASHTO12)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	
	// Implementation
protected:
	//{{AFX_MSG(CDgnCRCDgnParameter_AASHTO12)
	virtual BOOL OnInitDialog();
	afx_msg void OnUserInput();
	afx_msg void OnClickSelectAllOutputOpt();
	afx_msg void OnClickUnselectAllOutputOpt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
		
		// Attribute 
public:
	BOOL Dlg2Data();
	
	void Initial_Data();
	//   void InitExposureClassComboSet();
	//   void InitCementClassComboSet();
	void SetAnnexCtrl();
	
public:
	T_PSCD_D* m_pParamData;
	BOOL m_bInitData;
	int  m_nAnnex;
	
protected:
	CArray<UINT, UINT> m_aTndType;
	CArray<UINT, UINT> m_aExposureType;
	CArray<UINT, UINT> m_aCorrosiveConditon;
	CArray<UINT, UINT> m_aTndCalcMethod;
	CArray<UINT, UINT> m_aSegment;
	CArray<UINT, UINT> m_aCastType;
	
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}

#endif // !defined(__DgnCRCDgnParameter_CSA_H__)
