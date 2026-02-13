#if !defined(AFX_DGNCONCOLMDLGCIVIL_H__875BCF2D_E462_412B_A523_88365A5D2D8E__INCLUDED_)
#define AFX_DGNCONCOLMDLGCIVIL_H__875BCF2D_E462_412B_A523_88365A5D2D8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConColmDlgCIVIL.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnColmGridWnd.h"
#include "DgnSelSectBase.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmDlgCIVIL dialog

class CDgnConColmDlgCIVIL : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConColmDlgCIVIL(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Dialog Data
	//{{AFX_DATA(CDgnConColmDlgCIVIL)
	enum { IDD = IDD_DGN_CON_COLM_DLG_CIVIL };
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
	int		m_AddorDel;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConColmDlgCIVIL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_RCHK_K m_RchkKey;
	T_RCHK_D m_RchkData;
	CMap<int,int,int,int> m_arColmData;   // Element 중 Concrete Beam의 Data 저장 
	
	
	CString     m_Shape;

	CDBDoc*         m_pDoc;
	CDgnColmGridWnd m_InfoGrid;
	CDgnSelSectBase  m_SelectGrid;

	CArray<UINT, UINT>    m_BGroup, m_HGroup, m_tGroup, m_abGroup;

	void SetInitGrid();
	void SetInitUnit();
	void SetData2Dlg();
	BOOL SetDlg2Data();
	void WriteInfoGridTitle();
	void ShowBitmap(CString& colType);
	void ChangeSectionInfo();
	void SelectElemOnView();
	void ShowHideControl(CString& colType);
	void WriteRebarInfoOnGrid();
	BOOL CheckInputData();
	void SetControlText(BOOL bBox);
	void UpdateBuffer();
	void UpdateUnit();
	BOOL Is2TypeRebar(int nPos);

	void InitSectGrid();
	void ResetSectGridData(BOOL bKeepChk = FALSE);

	// Generated message map functions
	//{{AFX_MSG(CDgnConColmDlgCIVIL)
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnDesignSection();
	afx_msg void OnDgnRcCinputbar();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnColumnAr();
	afx_msg void OnDgnColumnDel();
	afx_msg void OnDgnColmSelectAll();
	afx_msg void OnDgnColmUnSelectAll();
	//}}AFX_MSG
	LRESULT OnSelectCheckSectionGrid(WPARAM wParam, LPARAM lParam);
	LRESULT OnChangeFocusSectionGrid(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONCOLMDLGCIVIL_H__875BCF2D_E462_412B_A523_88365A5D2D8E__INCLUDED_)
