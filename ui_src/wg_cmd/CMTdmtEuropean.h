#if !defined(AFX_CMTDMTEUROPEAN_H__3788D25C_FA55_4AEF_8941_E888A84853DF__INCLUDED_)
#define AFX_CMTDMTEUROPEAN_H__3788D25C_FA55_4AEF_8941_E888A84853DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTdmtEuropean.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxTdmf.h"

#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\MIT_frx\MButton.h"
/////////////////////////////////////////////////////////////////////////////
// CCMTdmtEuropean dialog

class CCMTdmtEuropean : public CDlgChild
{
// Construction
public:
	CCMTdmtEuropean(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMTdmtEuropean)
	enum { IDD = IDD_ETC_HCRP_TDMT_EUROPEAN };
	/*CSpinButtonCtrl	m_wndRelativeHumiditySpin;*/
	CFormulaEditSpin	m_wndRelativeHumidity;
	/*CSpinButtonCtrl	m_wndConcreteAgeSpin;*/
	CFormulaEditSpin	m_wndConcreteAge;

	CTextUnit	m_wndConcreteAgeUnit;
	CTextUnit	m_wndNotationSizeUnit;
	CTextUnit	m_wndCompStrengthUnit;
	CEditUnit	m_wndNotationSize;
	CEditUnit	m_wndCompStrength;

	mit::frx::MButton   m_bSilicaFume;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTdmtEuropean)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	void SetDlg2Data();
	T_TDMT_D* m_pData;

	int m_nCementType;
	int m_nTypeofCode;

	CArray<UINT, UINT> m_aEuropeanRadioCtrl;
	CArray<UINT, UINT> m_aEuropeanTypeofCode;

// Implementation
protected:
	void SetUnitType();

	// Generated message map functions
	//{{AFX_MSG(CCMTdmtEuropean)
	virtual BOOL OnInitDialog();
	virtual void OnCmdHcrpEuropeanCementType(); 
	virtual void OnCmdHcrpEuropeanTypeofCode(); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTDMTEUROPEAN_H__3788D25C_FA55_4AEF_8941_E888A84853DF__INCLUDED_)
