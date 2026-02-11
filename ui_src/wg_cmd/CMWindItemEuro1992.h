#if !defined(__CMWINDITEMEURO1992_H__)
#define __CMWINDITEMEURO1992_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemEuro1992.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemEuro1992 dialog

class CCMWindItemDlg;
class CCMWindItemEuro1992 : public CChildDialog
{
// Construction
public:
	CCMWindItemEuro1992(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_WIND_EURO1992& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_EURO1992& data);

// Dialog Data
	//{{AFX_DATA(CCMWindItemEuro1992)
	enum { IDD = IDD_ETC_WIND_ITEM_EURO1992 };
	MButton	m_chkTopographic;
	CEditUnit	m_editWindward;
	CTextUnit	m_unitVCT;
	CEditUnit	m_editVCT;
	CBCGPStatic  	m_unitSpeed;
	CEditUnit	m_editSpeed;
	MButton	m_chkLoadEval;
	CEditUnit	m_editLeeward;
	CEditUnit	m_editGustFactor;
	CEditUnit	m_editFriction;
	CEditUnit	m_editForceCeof;
	CEditUnit	m_editCT;
	MComboBox	m_cobxCategory;
		CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemEuro1992)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitDefaultData();
	void InitCategoryCombo();
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	double m_dHEURO1992;
	double m_dBBEURO1992;			
	double m_dBDEURO1992;			
	double m_dFreqEURO1992;		
	double m_dDampingEURO1992; 

	BOOL m_bInit;
	BOOL m_bModify;
	T_WIND_EURO1992  m_Data;
	CCMWindItemDlg *m_pParent;
	
	CArray<UINT, UINT> m_aCtrlMethod;
	CArray<UINT, UINT> m_aCtrlDetail;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemEuro1992)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSimpleDetailRadio();
	afx_msg void OnCmdLoadevalChk();
	afx_msg void OnCmdTopoCheck();
	afx_msg void OnCmdGustfButton();
		afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMEURO1992_H__)
