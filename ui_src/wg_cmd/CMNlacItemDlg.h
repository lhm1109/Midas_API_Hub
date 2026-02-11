#if !defined(AFX_CMNLACITEMDLG_H__C0A357D9_B193_4EE0_B17A_2CE67FA7726A__INCLUDED_)
#define AFX_CMNLACITEMDLG_H__C0A357D9_B193_4EE0_B17A_2CE67FA7726A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMNlacItemDlg.h : header file
//
#include "CMDlgBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_SelectLC.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "../wg_base/MouseEdit.h"

using namespace mit::frx;

class CCMNlacDlg;
class CCMNlacItemGrid;

/////////////////////////////////////////////////////////////////////////////
// CCMNlacItemDlg dialog

class CCMNlacItemDlg : public CCMDlgBase
{
// Construction
public:
	CCMNlacItemDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMNlacItemDlg();  

	T_NLCT_NEWTON m_DataNewton;
	T_NLCT_ARCLEN m_DataArclen;
	T_NLCT_DISPCT m_DataDispct;  
	CArray<T_NLCT_NEWTON, T_NLCT_NEWTON&> m_arNewton;
	CArray<T_NLCT_ARCLEN, T_NLCT_ARCLEN&> m_arArclen;
	CArray<T_NLCT_DISPCT, T_NLCT_DISPCT&> m_arDispct;

	BOOL m_bModify;
	int  m_nMethod;
	
	void Data2Dlg();
	void AlignControls();  
	BOOL Dlg2Data();  
	void SetInitData();
	BOOL ApplyOrOK();
	void CtrlManager();
	BOOL CheckDataNewton(T_NLCT_NEWTON DataNewton);
	BOOL CheckDataArclen(T_NLCT_ARCLEN DataArclen);
	BOOL CheckDataDispct(T_NLCT_DISPCT DataDispct);

	void SetParamData(bool isModify, int nMethod, const T_NLCT_NEWTON& dataNewton, const T_NLCT_DISPCT& dataDispct, const CArray<T_NLCT_NEWTON, T_NLCT_NEWTON&>& arNewton, const CArray<T_NLCT_DISPCT, T_NLCT_DISPCT&>& arDispct);
		
// Dialog Data
	//{{AFX_DATA(CCMNlacItemDlg)
	enum { IDD = IDD_ETC_NLAC_ITEM };
	CTextUnit	m_maxdispunit;
	CFormulaEditSpin	m_maxiter;
	CEditUnit	m_maxdisp;
	CFormulaEditSpin	m_maxincr;
	CEditUnit	m_initforce;
	CFormulaEditSpin	m_maximum;
	CFormulaEditSpin	m_number;
	CSelectLC m_NewtonLoadCaseCmb;
	CSelectLC m_DispctLoadCaseCmb;
	CSelectLC m_ArclenLoadCaseCmb;
	CFormulaEditSpin m_DispCtrlMaxIter;
	CEditUnit m_DispCtrlMaxDisp;
	CTextUnit m_DispCtrlMaxDispUnit;
	CFormulaEditSpin m_DispCtrlDispStep;
	CMouseEdit m_DispCtrlMasterNode;
	MComboBox m_DispCtrlDirectionCmb;
	int m_nInterMethodRdo; // 0:Newton-Raphson, 1:Displacement-Control
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMNlacItemDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT, UINT> m_aCtrlNewton, m_aCtrlArcLeng, m_aCtrlDispCtrl;
	CDBDoc* m_pDoc;  
	T_STLD_K m_OriginKeyStld;
	// Generated message map functions
	//{{AFX_MSG(CCMNlacItemDlg)
	virtual BOOL OnInitDialog();
 	afx_msg void OnNewtonGenBtn();
	afx_msg void OnDispCtrlGenBtn();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdLoadcaseBtn();
	afx_msg void OnChangeCmdNlacNewtonNumEd();
	afx_msg void OnChangeCmdNlacDispctrlDispstepEd();
	afx_msg void OnChangeCmdNlacDispctrlMaxdispEd();
	afx_msg void OnMethodRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void GridData2Dlg();
	void GridDlg2Data();

private:
	CCMNlacDlg* m_pParentWnd;

	CCMNlacItemGrid* m_wndGrid;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMNLACITEMDLG_H__C0A357D9_B193_4EE0_B17A_2CE67FA7726A__INCLUDED_)
