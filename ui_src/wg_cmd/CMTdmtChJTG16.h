#if !defined(AFX_CMTDMTCHJTG16_H__3788D25C_FA55_4AEF_8941_E888A84853DF__INCLUDED_)
#define AFX_CMTDMTCHJTG16_H__3788D25C_FA55_4AEF_8941_E888A84853DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTdmtChJTG16.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\SpinBtnExCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCMTdmtChJTG16 dialog

class CCMTdmtChJTG16 : public CDlgChild
{
// Construction
public:
	CCMTdmtChJTG16(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMTdmtChJTG16)
	enum { IDD = IDD_ETC_HCRP_TDMT_CHJTG16 };

	CEditUnit	m_edtCompStrength;
	CTextUnit	m_untCompStrength;
	
	/*CSpinButtonCtrl	m_spnRelativeHumidity;*/
	CFormulaEditSpin	m_edtRelativeHumidity;
	CEditUnit	m_edtNotationSize;
	CTextUnit	m_untNotationSize;
	CEditUnit	m_edtCementTypeCoef;
	/*CSpinButtonCtrl	m_spnConcreteAge;*/
	CFormulaEditSpin	m_edtConcreteAge;
	CTextUnit	m_untConcreteAge;
	CEditUnit	m_edtAddFlyash;
	CEditUnit	m_edtR_Elast;
	CEditUnit	m_edtR_Area;
	CEditUnit	m_edtEqvElast;
	CTextUnit	m_untEqvElast;

	BOOL	m_bEqiv;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTdmtChJTG16)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	void SetDlg2Data();
	void SetData2DlgByCodeChange(int nCode);
	T_TDMT_D* m_pData;

// Implementation
protected:
	void SetUnitType();

	// Generated message map functions
	//{{AFX_MSG(CCMTdmtChJTG16)
	virtual BOOL OnInitDialog();

	afx_msg void OnEqivChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTDMTCHJTG16_H__3788D25C_FA55_4AEF_8941_E888A84853DF__INCLUDED_)
