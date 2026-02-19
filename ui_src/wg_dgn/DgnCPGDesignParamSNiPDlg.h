#if !defined(__CDGNCPGDESIGNPARAMSNIPDLG_H__)
#define __CDGNCPGDESIGNPARAMSNIPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDgnCPGDesignParamAASHTO07Dlg.h : header file
//
#include "DgnDlgBase.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamAASHTO07Dlg dialog

using namespace mit::frx;

class CDgnCPGDesignParamBaseDlg;
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnCPGDesignParamSNiPDlg : public CChildDialog
{
	// Construction
public:
	CDgnCPGDesignParamSNiPDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnCPGDesignParamSNiPDlg();

	// Dialog Data
	//{{AFX_DATA(CDgnCPGDesignParamSNiPDlg)
	enum { IDD = IDD_DGN_CPG_PARAM_RUS_SNIP};

	MComboBox m_cmbBridgeType;

	MButton	m_btnmb7mb8;
	MButton	m_btnmb9;
	BOOL      m_bmb7mb8;
	BOOL      m_bmb9;
	CEditUnit m_edtmb7;
	CEditUnit m_edtmb8;
	CEditUnit m_edtmb9;
	CArray<UINT, UINT> m_armb7mb8Type;
	CArray<UINT, UINT> m_armb9Type;

	CEditUnit m_edtConcContri_Str;
	CEditUnit m_edtConcContri_Crack;

	CEditUnit m_edtFactorErection_m;
	CEditUnit m_edtFactorOperation_m;
	CEditUnit m_edtFactor_gamma_n;

	CArray<UINT, UINT> m_arStressComponentType;

	MButton	m_btnDgnParam_Sterngth;
	MButton	m_btnDgnParam_Stability;
	MButton	m_btnDgnParam_Fatigue;
	MButton	m_btnDgnParam_Shear_Connector;
	MButton	m_btnDgnParam_Crack;

	//}}AFX_DATA

	void InitCtrl();
	void InitUnit();
	void EnableDisableControls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	// Implementation
protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	virtual void OnCancel();

	afx_msg void OnConditionFactormb7mb8Chk();
	afx_msg void OnConditionFactormb7mb8TypeRdo();
	afx_msg void OnConditionFactormb9Chk();
	afx_msg void OnConditionFactormb9TypeRdo();



	DECLARE_MESSAGE_MAP()

protected:
	CDgnCPGDesignParamBaseDlg* m_pParent;
	CDBDoc* m_pDoc;

	T_CPGD_D *m_pData;

public:
	inline void SetData(T_CPGD_D* pData) { m_pData = pData; }
	void Data2Dlg();
	void Dlg2Data();		
	afx_msg void OnSelchangeBrdtypeCmb();
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CDGNCPGDESIGNPARAMSNIPDLG_H__)
