#if !defined(__ModifyColmLocPage_H__)
#define      __ModifyColmLocPage_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModifyColmLocPage.h : header file
//

// Column Location 대화상자

/////////////////////////////////////////////////////////////////////////////
// CModifyColmLocPage dialog
#include "ElemCreateDlg.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
class CModifyColmLocPage : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CModifyColmLocPage(CWnd* pParent = NULL);   // standard constructor
	~CModifyColmLocPage();

public:
	virtual void Execute();
	  
	// Dialog Data
	//{{AFX_DATA(CModifyColmLocPage)
	enum { IDD = IDD_TM_COLM_LOC_PAGE };
	
	MComboBox m_cmbDir1 ;
	MComboBox m_cmbDir2 ;
	CEditUnit m_edtDist1;
	CEditUnit m_edtDist2;
	CTextUnit m_untDist1;
	CTextUnit m_untDist2;

	
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModifyColmLocPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();
protected:
	CDBDoc* m_pDoc;
	int m_nAddDel; // 0:Add/Replace, 1:Delete
	int m_nColLoc; // 0:Interior, 1:Edge, 2:Corner
	CArray<UINT,UINT> m_aCtrlsDist1;
	CArray<UINT,UINT> m_aCtrlsDist2;
	CArray<UINT,UINT> m_aCtrlsAll;

	BOOL m_bEnable;
// Implementation
protected:
	void InitCtrls();
	void Initial_Data();
	void EnableDisableCtrls();
	// Generated message map functions
	//{{AFX_MSG(CModifyColmLocPage)
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ModifyColmLocPage_H__E3A50C03_6049_11D3_8899_0000C0A2329D__INCLUDED_)
