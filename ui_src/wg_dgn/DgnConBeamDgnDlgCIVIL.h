#if !defined(AFX_DGNCONBEAMDGNDLGCIVIL_H__5848ACEB_4FAE_4F3D_B0DC_6F4407E23B94__INCLUDED_)
#define AFX_DGNCONBEAMDGNDLGCIVIL_H__5848ACEB_4FAE_4F3D_B0DC_6F4407E23B94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBeamDgnDlgCIVIL.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnSelSectBase.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamDgnDlgCIVIL dialog
class CDBDoc;

class CDgnConBeamDgnDlgCIVIL : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConBeamDgnDlgCIVIL(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnConBeamDgnDlgCIVIL)
	enum { IDD = IDD_DGN_CON_BEAM_DGN_DLG_CIVIL };
	CEditUnit m_SubNum;
	CComboBox	m_SubCB;
	CTextUnit	m_dTunit;
	CEditUnit	m_dT;
	CTextUnit	m_hfunit;
	CEditUnit	m_hf;
	CTextUnit	m_Hcunit;
	CEditUnit	m_Hc;
	CTextUnit	m_bfunit;
	CEditUnit	m_bf;
	CTextUnit	m_Bcunit;
	CEditUnit	m_Bc;
	CTextUnit	m_dBunit;
	CEditUnit	m_dB;
	int		m_AddorDel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBeamDgnDlgCIVIL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_RDGN_K  m_RdgnKey;
	T_RDGN_D  m_RdgnData;

	CMap<int,int,int,int> m_arBeamData;   // Element 중 Concrete Beam의 Data 저장 

	CDgnSelSectBase m_SelectGrid;

	CDBDoc* m_pDoc;

	void SetInitStirCB();
	void SetInitUnit();
	void SetData2Dlg();
	void ShowRECTBitmap();
	void ShowTEEBitmap();
	void ShowHideSectCtrl(BOOL bShow);
	BOOL CheckInputData();
	void SetDlg2Data();
	void ChangeSectionInfo();
	void SelectElemOnView();
	void UpdateBuffer();

	void InitSectGrid();
	void ResetSectGridData(BOOL bKeepChk = FALSE);

	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);

	// Generated message map functions
	//{{AFX_MSG(CDgnConBeamDgnDlgCIVIL)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnDesignSection();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnBeamSelectAll();
	afx_msg void OnDgnBeamUnSelectAll();
	//}}AFX_MSG
	LRESULT OnSelectCheckSectionGrid(WPARAM wParam, LPARAM lParam);
	LRESULT OnChangeFocusSectionGrid(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONBEAMDGNDLGCIVIL_H__5848ACEB_4FAE_4F3D_B0DC_6F4407E23B94__INCLUDED_)
