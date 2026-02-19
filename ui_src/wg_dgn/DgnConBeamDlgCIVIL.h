#if !defined(AFX_DGNCONBEAMDLGCIVIL_H__65C75BB5_8777_427F_8AAC_D4118931ED83__INCLUDED_)
#define AFX_DGNCONBEAMDLGCIVIL_H__65C75BB5_8777_427F_8AAC_D4118931ED83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBeamDlgCIVIL.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnBeamGridWnd.h"
#include "DgnSelSectBase.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamDlgCIVIL dialog

class CDBDoc;

class CDgnConBeamDlgCIVIL : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConBeamDlgCIVIL(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnConBeamDlgCIVIL)
	enum { IDD = IDD_DGN_CON_BEAM_DLG_CIVIL };
	CTextUnit	m_HfUnit;
	CEditUnit	m_Hf;
	CTextUnit	m_HcUnit;
	CEditUnit	m_Hc;
	CTextUnit	m_BfUnit;
	CEditUnit	m_Bf;
	CTextUnit	m_BcUnit;
	CEditUnit	m_Bc;
	int		    m_nOption;
	int		    m_bCrack;

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBeamDlgCIVIL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

protected:
	T_RCHK_K m_RchkKey;
	T_RCHK_D m_RchkData;
	CMap<int,int,int,int> m_arBeamData;   // Element 중 Concrete Beam의 Data 저장 

	CDBDoc* m_pDoc;
	CDgnBeamGridWnd m_InfoGrid;
	CDgnSelSectBase m_SelectGrid;

	CArray<UINT, UINT> m_aTobeMovedCtrl;

	void SetInitUnit();
	void SetData2Dlg();
	BOOL SetDlg2Data();
	void UpdateBuffer();
	void UpdateUnit();
	void ShowRECTBitmap();
	void ShowTEEBitmap();
	void WriteInfoGridTitle();
	void InitInfoGrid();
	void ChangeSectionInfo();
	void ChangeDlgItemInfo();
	BOOL CheckInputData();
	void ShowHideSectCtrl(BOOL bShow);
	void SelectElemOnView();
	void WriteNodeInfoOnGrid(int nNode);
	void CopyRebarData(int FromNode, int ToNode);
	BOOL Is2TypeRebar(int nNode);
	void AlignControls(int iCrackEnvNum);

	void InitSectGrid();
	void ResetSectGridData(BOOL bKeepChk = FALSE);

	// Generated message map functions
	//{{AFX_MSG(CDgnConBeamDlgCIVIL)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnConcInode();
	afx_msg void OnDgnConcJnode();
	afx_msg void OnDgnConcMiddle();
	afx_msg void OnDgnDesignSection();
	afx_msg void OnDgnBeamAr();
	afx_msg void OnDgnBeamDel();
	afx_msg void OnDgnBeamSelectAll();
	afx_msg void OnDgnBeamUnSelectAll();
	//}}AFX_MSG
	LRESULT OnSelectCheckSectionGrid(WPARAM wParam, LPARAM lParam);
	LRESULT OnChangeFocusSectionGrid(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONBEAMDLGCIVIL_H__65C75BB5_8777_427F_8AAC_D4118931ED83__INCLUDED_)
