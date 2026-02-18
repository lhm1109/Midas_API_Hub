#if !defined(__DgnCRCDgnParameter_CSA_19_H__)
#define __DgnCRCDgnParameter_CSA_19_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCRCDgnParameter_EC2PSC.h : header file
//

#include "..\wg_cmd\MyChildDialog.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CDgnCRCDgnParameter_EC2PSC dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnCRCDgnParameter_CSA_19 : public CMyChildDialog
{
	// Construction
public:
	CDgnCRCDgnParameter_CSA_19(T_PSCD_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	~CDgnCRCDgnParameter_CSA_19(); 

	// Dialog Data
	//{{AFX_DATA(CDgnCRCDgnParameter_EC2PSC)
	enum { IDD = IDD_DGN_CRC_DESIGN_PARAMETER_CSA_19};

	MButton   m_ChkOutputOpt_ULS1;
	MButton   m_ChkOutputOpt_ULS2;
	MButton   m_ChkOutputOpt_ULS3;
	MButton   m_ChkOutputOpt_ULS4;

	MButton   m_ChkOutputOpt_SLS1;
	MButton   m_ChkOutputOpt_SLS2;
	MButton   m_ChkOutputOpt_SLS3;
	MButton   m_ChkOutputOpt_SLS4;
	MButton   m_ChkOutputOpt_SLS5;
	MButton   m_ChkOutputOpt_SLS6;

	MButton   m_RdoTndType;
	MButton   m_RdoTndCalcMethod;
	MButton   m_RdoRebarEpoxy;
	MButton   m_RdoExposureType;
	//}}AFX_DATA


	// Overrides
	//{{AFX_VIRTUAL(CDgnCRCDgnParameter_EC2PSC)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


	// Implementation
protected:
	//{{AFX_MSG(CDgnCRCDgnParameter_CSA_19)
	virtual BOOL OnInitDialog();
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


public:
	T_PSCD_D* m_pParamData;

protected:
	CArray<UINT, UINT> m_aTndType;
	CArray<UINT, UINT> m_aTndCalcMethod;
	CArray<UINT, UINT> m_aRebarEpoxy;
	CArray<UINT, UINT> m_aExposureType;
	CArray<UINT, UINT> m_aSegment;

};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}

#endif // !defined(__DgnCRCDgnParameter_CSA_19_H__)
