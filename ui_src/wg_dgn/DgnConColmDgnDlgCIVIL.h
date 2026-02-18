#if !defined(AFX_DGNCONCOLMDGNDLGCIVIL_H__7D3570EA_93EF_47D0_8452_460D5EE73A6E__INCLUDED_)
#define AFX_DGNCONCOLMDGNDLGCIVIL_H__7D3570EA_93EF_47D0_8452_460D5EE73A6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConColmDgnDlgCIVIL.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnSectGridWnd.h"
#include "DgnSelSectBase.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmDgnDlgCIVIL dialog

class CDBDoc;

class CDgnConColmDgnDlgCIVIL : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConColmDgnDlgCIVIL(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnConColmDgnDlgCIVIL)
	enum { IDD = IDD_DGN_CON_COLM_DGN_DLG_CIVIL };
	CEditUnit	m_barnum;
	CComboBox	m_StirrupCB;
	CComboBox	m_PropNoCB;
	CTextUnit	m_tunit;
	CEditUnit	m_t;
	CTextUnit	m_Hunit;
	CEditUnit	m_H;
	CTextUnit	m_bunit;
	CEditUnit	m_b;
	CTextUnit	m_Bunit;
	CEditUnit	m_B;
	CTextUnit	m_aunit;
	CEditUnit	m_a;
	CTextUnit	m_Dcunit;
	CEditUnit	m_Dc;
	int		m_AddorDel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConColmDgnDlgCIVIL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_RDGN_K              m_RdgnKey;
	T_RDGN_D              m_RdgnData;

	CMap<int,int,int,int> m_arColmData;   // Element 중 Concrete Beam의 Data 저장 

	CDgnSelSectBase m_SelectGrid;

	CDBDoc*               m_pDoc;
	CArray<UINT, UINT>    m_BGroup, m_HGroup, m_tGroup, m_abGroup;

	void SetInitStirCB();
	void SetInitUnit();
	void SetData2Dlg();
	void SetDlg2Data();
	BOOL CheckInputData();
	void ChangeSectionInfo();
	void ShowBitmap(CString& colType);
	void ShowHideControl(CString& colType);
	void SelectElemOnView();
	void SetControlText(BOOL bBox);
	void UpdateBuffer();

	void InitSectGrid();
	void ResetSectGridData(BOOL bKeepChk = FALSE);


	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);

	// Generated message map functions
	//{{AFX_MSG(CDgnConColmDgnDlgCIVIL)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnDesignSection();
	afx_msg void OnDgnColmSelectAll();
	afx_msg void OnDgnColmUnSelectAll();
	//}}AFX_MSG
	LRESULT OnSelectCheckSectionGrid(WPARAM wParam, LPARAM lParam);
	LRESULT OnChangeFocusSectionGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONCOLMDGNDLGCIVIL_H__7D3570EA_93EF_47D0_8452_460D5EE73A6E__INCLUDED_)
