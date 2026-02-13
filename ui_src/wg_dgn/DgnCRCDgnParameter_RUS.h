#if !defined(__DGNCRCDGNPARAMETER_RUS_H__)
#define __DGNCRCDGNPARAMETER_RUS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCRCDgnParameter_RUS.h : header file
//

#include "..\wg_cmd\MyChildDialog.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnCRCDgnParameter_EC2PSC dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnCRCDgnParameter_RUS : public CMyChildDialog
{
	// Construction
public:
	CDgnCRCDgnParameter_RUS(T_PSCD_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	~CDgnCRCDgnParameter_RUS(); 

	// Dialog Data
	//{{AFX_DATA(CDgnCRCDgnParameter_EC2PSC)
	enum { IDD = IDD_DGN_CRC_DESIGN_PARAMETER_RUS};
	
	mit::frx::MComboBox m_cmbBridgeType;
	
	BOOL      m_bmb7mb8;
	int       m_nmb7mb8Type;
	CEditUnit m_edtmb7;
	CEditUnit m_edtmb8;
	BOOL      m_bmb9;
	int       m_nmb9Type;
	CEditUnit m_edtmb9;

	BOOL      m_b1stGrp_1;
	BOOL      m_b1stGrp_2;
	BOOL      m_b1stGrp_3;
	BOOL      m_b1stGrp_4;

	BOOL      m_b2ndGrp_1;
	BOOL      m_b2ndGrp_2;
	BOOL      m_b2ndGrp_3;
	BOOL      m_b2ndGrp_4;
	BOOL      m_b2ndGrp_5;
	BOOL      m_b2ndGrp_6;
	//}}AFX_DATA


	// Overrides
	//{{AFX_VIRTUAL(CDgnCRCDgnParameter_EC2PSC)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	
	// Implementation
protected:
	//{{AFX_MSG(CDgnCRCDgnParameter_RUS)
	virtual BOOL OnInitDialog();
	afx_msg void OnConditionFactormb7mb8Chk();
	afx_msg void OnConditionFactormb7mb8TypeRdo();
	afx_msg void OnConditionFactormb9Chk();
	afx_msg void OnConditionFactormb9TypeRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Attribute 
public:
	void InitUnit();
	void InitCombo();
	void EnableDisableControls();

	BOOL Dlg2Data();
	void Data2Dlg();
	
public:
	T_PSCD_D* m_pData;
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}

#endif // !defined(__DGNCRCDGNPARAMETER_RUS_H__)
