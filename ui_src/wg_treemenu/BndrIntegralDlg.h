#if !defined(__BNDRINGEGRALDLG_H__)
#define __BNDRINGEGRALDLG_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrIntegralDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\wg_base\FoldGroupBox.h"

#include "BndrIntegralAbut.h"
#include "BndrIntegralPileDlg.h"
#include "..\MIT_frx\MillustViewer.h"

/////////////////////////////////////////////////////////////////////////////
// CBndrIntegralDlg dialog


class CBndrIntegralDlg : public CMenuBarChildDlg , public CDBUpdateConnector
{
// Construction
public:
	CBndrIntegralDlg(CWnd* pParent = NULL);   // standard constructor
	~CBndrIntegralDlg();
	virtual void Execute();
// Dialog Data
	//{{AFX_DATA(CBndrIntegralDlg)
	enum { IDD = IDD_TM_BNDR_INTEBR };
	CCobxBngr	m_wndGroupCombo;
	CBCGPStatic	m_wndPlaceHolder;
	//CFoldGroupBox_Cross	m_frmRefFigure;
	//}}AFX_DATA
public:
	 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrIntegralDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL	CreateChildDlgs(void);		
	BOOL	DestroyChildDlgs(void);		
	BOOL	ShowChildDlgs(int which);	
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	
protected:
	CDBDoc* m_pDoc;
	CBndrIntegralPileDlg* m_pPileDlg;
	CBndrIntegralAbut* m_pAbutDlg;
	SpringPile PileData;
	SpringAbutment AbutmentData;
	int m_nBottom[2]; 
	CArray<UINT,UINT> m_arType;
		
	void AlignControl();
	void InitSubCtrl();
	BOOL GetParameter();
	void ExecutePile();
	void ExecuteAbutment();
	void CustomizeControls_GEN();
	void GetTypeSel(int& nSel);

	//CBitmap* m_pBitmap;
	mit::frx::MillustViewer m_wndPicture;
	void ChangeBitmap(int nType);

	BOOL m_bFirstInit;
	
	// Generated message map functions
	//{{AFX_MSG(CBndrIntegralDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmBndrSospType();
	afx_msg void OnTmDefineGroupButton();
	afx_msg void OnTmClose();
	afx_msg void OnTmExecute();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__BNDRINGEGRALDLG_H__)
