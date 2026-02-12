#if !defined(__CMWINDITEMKOREA2000_H__)
#define __CMWINDITEMKOREA2000_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemKorea2000.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemKorea2000 dialog
class CCMWindItemDlg;
class CCMWindItemKorea2000 : public CChildDialog
{
// Construction
public:
	CCMWindItemKorea2000(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_WIND_KS2000& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_KS2000& data);

// Dialog Data
	//{{AFX_DATA(CCMWindItemKorea2000)
	enum { IDD = IDD_ETC_WIND_ITEM_KOREA2000 };
	MButton	m_chkTopoEffect;
	MButton	m_chkLoadEval;
	CTextUnit	m_unitVKZT;
	CEditUnit	m_editVKZT;
	CTextUnit	m_unitRoofHeight;
	CEditUnit	m_editRoofHeight;
	CBCGPStatic 	m_unitSpeed;        // by hp, 단위변환 안되야함..
	CEditUnit	m_editSpeed;
	CEditUnit	m_editPresCoef;
	CEditUnit	m_editKZT;
	CEditUnit	m_editGustFactor;
	CEditUnit	m_editGustFactorY;  // new 6.5.0
	CEditUnit	m_editForceCoef;
	MComboBox	m_cobxCategory;
		CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA

protected:
	void InitDefaultData();
	void InitCategoryCombo();
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	double m_dBXKOR2000 ;		
	double m_dBYKOR2000 ;		
	double m_dFreqXKOR2000;	
	double m_dFreqYKOR2000;	
	double m_dDampingKOR2000;
	double _wndKor2000GustRdo;

	BOOL m_bInit;
	BOOL m_bModify;
	T_WIND_KS2000  m_Data;
	CCMWindItemDlg *m_pParent;

	CArray<UINT, UINT> m_aCtrlType;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemKorea2000)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemKorea2000)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdGustRadio();
	afx_msg void OnCmdGustFactorBtn();
	afx_msg void OnCmdLoadEvalCheck();
	afx_msg void OnCmdTopoEffectCheck();
	afx_msg void OnSelchangeCmdCategoryCombo();
		afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMKOREA2000_H__)
