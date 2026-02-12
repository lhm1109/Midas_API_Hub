#if !defined(__BNDRINTEGRALABUT_H__)
#define __BNDRINTEGRALABUT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrSospIntegral.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBndrIntegralAbut dialog
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\wg_base\wg_base_DlgChild.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

struct SpringAbutment {
	int nElType;
	int nFace;
	int nDirection;
	double aVector[3];
	
	CString strEList;
	CString strNList;

	double dGround;
	double dHeight;
	double dWidth;
	double dDeck;

	double dVoid;
	double dGS;
	double dCycle;
	double dEarth;
	
	double dTemp;
	double dThermal;
	T_MATL_K MatlK;

	double dFWidth;
	double dFPress;
	int nRotation;
};

class CBndrIntegralAbut : public CDlgChild
{
// Construction
public:
	CBndrIntegralAbut(SpringAbutment* pData,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBndrIntegralAbut)
	enum { IDD = IDD_TM_BNDR_INTEBR_ABUT };
	
	CBCGPStatic	m_stcDirection;
	CBCGPStatic	m_stcEList;
	CTextUnit m_untWidth;
	CTextUnit m_untHeight;
	//CTextUnit m_untGround;
	CTextUnit m_untDeck;
	CTextUnit m_untTemp;
	CTextUnit m_untFWidth;
	CTextUnit m_untFPress;
	CTextUnit m_untThermal;
	CMouseEdit m_edtDirection;
	//CMouseEdit m_edtGround;
	CMouseEdit m_edtHeight;
	CMouseEdit m_edtWidth;
	CMouseEdit m_edtDeck;
	CEditUnit m_edtVoid;
	CEditUnit m_edtGS;
	CEditUnit m_edtCycle;
	//CEditUnit m_edtEarth;
	CEditUnit m_edtTemp;
	CEditUnit m_edtThermal; 
	CEditUnit m_edtFWidth;
	CEditUnit m_edtFPress; 
	MComboBox m_cmbDirection;
	MComboBox m_cmbElemType;
	MComboBox m_cmbFace;
	MComboBox m_cmbMatl;
	MComboBox m_cmbRotation;
	CSelectEdit m_edtEList;
	CSelectEdit m_edtNList;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrIntegralAbut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitSubCtrl();
	BOOL GetParameter();
	virtual void UpdateBuffer();
	BOOL GetPosition(CString& strPos,double& Px ,double& Py ,double& Pz);
protected:
	BOOL GetDirectionVector(CString &strVector, double& Ux, double& Uy, double& Uz);
	BOOL GetEQDistance(CString& EQStr, double& Dx, double& Dy, double& Dz);
	void CheckMode(BOOL bCheck);
	void MakeMatlCombo();

	void ChangeDirCombo();
	void UseVectorOnFrame();
	void SwitchOnSolidNode();
protected:
	SpringAbutment* m_pData;

	// Generated message map functions
	//{{AFX_MSG(CBndrIntegralAbut)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTmElemTypeCombo();
	afx_msg void OnSelchangeTmBndrSospTypeSolidCbo();
	afx_msg void OnSelchangeTmBndrSospMaterialCbo();
	afx_msg void OnSetfocusNList();
	afx_msg void OnSetfocusEList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__BNDRINTEGRALABUT_H__)
