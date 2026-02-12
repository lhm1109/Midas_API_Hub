#if !defined(AFX_CMTDMTCHJTG04_H__3788D25C_FA55_4AEF_8941_E888A84853DF__INCLUDED_)
#define AFX_CMTDMTCHJTG04_H__3788D25C_FA55_4AEF_8941_E888A84853DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTdmtChJTG04.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxTdmf.h"

#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\SpinBtnExCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCMTdmtChJTG04 dialog

class CCMTdmtChJTG04 : public CDlgChild
{
// Construction
public:
	CCMTdmtChJTG04(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMTdmtChJTG04)
	enum { IDD = IDD_ETC_HCRP_TDMT_CHJTG04 };
	CEditUnit	m_wndCementTypeCoef;
	/*CSpinButtonCtrl	m_wndRelativeHumiditySpin;*/
	CFormulaEditSpin	m_wndRelativeHumidity;
	/*CSpinButtonCtrl	m_wndConcreteAgeSpin;*/
	CFormulaEditSpin	m_wndConcreteAge;
	CTextUnit	m_wndConcreteAgeUnit;
	CTextUnit	m_wndNotationSizeUnit;
	CTextUnit	m_wndCompStrengthUnit;
	CEditUnit	m_wndNotationSize;
	
	CEditUnit	m_wndCompStrength;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTdmtChJTG04)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	void SetDlg2Data();
	T_TDMT_D* m_pData;

// Implementation
protected:
	void SetUnitType();

	// Generated message map functions
	//{{AFX_MSG(CCMTdmtChJTG04)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTDMTCHJTG04_H__3788D25C_FA55_4AEF_8941_E888A84853DF__INCLUDED_)
