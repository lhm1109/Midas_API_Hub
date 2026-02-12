#if !defined(AFX_BNDRSOSPINTEGRAL_H__143A45CA_A6BA_4F29_A244_006ED7B07570__INCLUDED_)
#define AFX_BNDRSOSPINTEGRAL_H__143A45CA_A6BA_4F29_A244_006ED7B07570__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrSospIntegral.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBndrSospIntegral dialog
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\wg_base\DlgChild.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

struct SpringIntegral {
	int nNonType;
	int nElType;
	int nFace;
	int nDirection;
	double dWidth;
	double dVoid;
	double dWeight;
	double dAbut;
	double dGround;
	double dDeck;
	double dIncre;
	double dThermal;
};

class CBndrSospIntegral : public CDlgChild
{
// Construction
public:
	CBndrSospIntegral(SpringIntegral* pData,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBndrSospIntegral)
	enum { IDD = IDD_TM_BNDR_SOSP_INTEGRAL };
	CTextUnit m_wndWidthUnit;
	CTextUnit m_unitUnit;
	CTextUnit m_unitAbut;
	CTextUnit m_unitGround;
	CTextUnit m_unitDeck;
	CTextUnit m_unitIncre;
	MComboBox	m_cboDirection;
	CEditUnit m_wndWidth;
	CEditUnit m_editUnit;
	CEditUnit m_edtAbut;
	CMouseEdit m_edtGround;
	CMouseEdit m_edtDeck;
	CEditUnit m_edtIncre;
	MComboBox m_CobxElemType;
	MComboBox m_cboElemType;
	CEdit m_editVoid;
	MComboBox m_cboMatl;
	CEdit m_edtThermal;  
	

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrSospIntegral)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:

	void InitSubCtrl();
	BOOL GetParameter();
	virtual void UpdateBuffer();
protected:
	BOOL GetDirectionVector(CString &strVector, double& Ux, double& Uy, double& Uz);
	BOOL GetEQDistance(CString& EQStr, double& Dx, double& Dy, double& Dz);
	void CheckMode(BOOL bCheck);
	void MakeMatlCombo();
protected:
	SpringIntegral* m_pData;

	// Generated message map functions
	//{{AFX_MSG(CBndrSospIntegral)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTmElemTypeCombo();
	afx_msg void OnSelchangeTmBndrSospTypeSolidCbo();
	afx_msg void OnSelchangeTmBndrSospMaterialCbo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRSOSPINTEGRAL_H__143A45CA_A6BA_4F29_A244_006ED7B07570__INCLUDED_)
