#if !defined(AFX_CMWINDITEMEURO2005CSCD_H__C8D308B7_8BBD_4D38_8128_E457C0FFDB36__INCLUDED_)
#define AFX_CMWINDITEMEURO2005CSCD_H__C8D308B7_8BBD_4D38_8128_E457C0FFDB36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemEuro2005CsCd.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MEdit.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemEuro2005CsCd dialog

class CCMWindItemEuro2005CsCd : public CDialogMove
{
// Construction
public:
	CCMWindItemEuro2005CsCd(T_WIND_EURO2005* pData, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMWindItemEuro2005CsCd)
	enum { IDD = IDD_ETC_WIND_ITEM_EURO2005_CSCD };
	MEdit   	m_editTC;
	CEditUnit	m_editVb;
	CEditUnit	m_editKl;  
	CEditUnit	m_editH;
	MEdit	    m_editOro;
	MEdit	    m_editNei;
	MEdit 	  m_editDis;
	CEditUnit	m_editB;
	CEditUnit	m_editD;
	CEditUnit	m_editN1;
	CEditUnit	m_editDt;
	CEditUnit	m_editZs;
	CEditUnit	m_editVm;
	CEditUnit	m_editL;
	CEditUnit	m_editLv;
	CEditUnit	m_editFl;
	CEditUnit	m_editSl;
	CEditUnit	m_editRh;
	CEditUnit	m_editRb;
	CEditUnit	m_editEh;
	CEditUnit	m_editEb;
	CEditUnit	m_editBB;
	CEditUnit	m_editRR;
	CEditUnit	m_editV;
	CEditUnit	m_editKp;
	CEditUnit	m_editCd;	
	CTextUnit	m_unitH;
	CTextUnit	m_unitB;
	CTextUnit	m_unitD;
	CTextUnit	m_unitZs;    
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemEuro2005CsCd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	T_WIND_EURO2005* m_pData;

protected:
	void InitControls();
	void Data2Dlg();
	BOOL Dlg2Data();
	void SetTerrainCategory();
	void SetEffectsData();  

public:
	
	// Generated message map functions
	//{{AFX_MSG(CCMWindItemEuro2005CsCd)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdCalcBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMWINDITEMEURO2005CSCD_H__C8D308B7_8BBD_4D38_8128_E457C0FFDB36__INCLUDED_)
