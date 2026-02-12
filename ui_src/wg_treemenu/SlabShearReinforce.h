#if !defined(__SLAB_SHEAR_REINFORCE_PAGE_H__)
#define      __SLAB_SHEAR_REINFORCE_PAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SlabShearReinforce.h : header file
//

// Slab Shear Reinforcement 대화상자

/////////////////////////////////////////////////////////////////////////////
// CSlabShearReinforce dialog

#include "..\MIT_frx\MillustViewer.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

class CSlabShearReinforcePage : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CSlabShearReinforcePage(CWnd* pParent = NULL);   // standard constructor
	~CSlabShearReinforcePage();

public:
	virtual void Execute();
	  
	// Dialog Data
	//{{AFX_DATA(CSlabShearReinforcePage)
	enum { IDD = IDD_TM_SLAB_SHEAR_REINFORCE };
	
	CEditUnit m_edtAvY;
	CEditUnit m_edtAvZ;
	CEditUnit m_edtSpace;
	CEditUnit m_edtYield;
	
	CTextUnit m_untAvY;
	CTextUnit m_untAvZ;
	CTextUnit m_untSpace;
	CTextUnit m_untYield;

	mit::frx::MillustViewer m_wndShearImg;

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSlabShearReinforcePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();

protected:
	CDBDoc* m_pDoc;
	int m_nAddDel; // 0:Add/Replace, 1:Delete

	CArray <UINT, UINT> m_aCtrlID;
	CArray <UINT, UINT> m_aCtrlID2;
// Implementation
protected:
	void InitCtrls();
	void 	Initial_Data();
	// Generated message map functions
	//{{AFX_MSG(CSlabShearReinforcePage)
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnChangeOption();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SLAB_SHEAR_REINFORCE_PAGE_H__)
