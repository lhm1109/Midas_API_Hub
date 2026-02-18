#if !defined(AFX_DGNCONCOLMBARDLGCIVIL_H__C790B45C_414F_468E_AA31_1ABA41416700__INCLUDED_)
#define AFX_DGNCONCOLMBARDLGCIVIL_H__C790B45C_414F_468E_AA31_1ABA41416700__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConColmBarDlgCIVIL.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnSectionDrawWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmBarDlgCIVIL dialog
class CDgnColumnGridWndCIVIL;

class CDgnConColmBarDlgCIVIL : public CDialogMove
{
// Construction
public:
	CDgnConColmBarDlgCIVIL(CWnd* pParent = NULL);   // standard constructor

	void SetRchkData(T_RCHK_K RchkKey, CMap<int,int,T_RCHK_K,T_RCHK_K>& arRchkKey, T_RCHK_COLM* pColmData);

// Dialog Data
	//{{AFX_DATA(CDgnConColmBarDlgCIVIL)
	enum { IDD = IDD_DGN_CON_COLM_BAR_DLG_CIVIL };
	CTextUnit	m_AsUT;
	CEditUnit	m_As;
	CEditUnit	m_Num;
	CTextUnit	m_SpaceUT;
	CEditUnit	m_Space;
	CComboBox	m_SizeCB;
	CComboBox	m_LayerCB;
	int		    m_nType;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConColmBarDlgCIVIL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	int                      m_nLayerNum;
	T_RCHK_K                 m_RchkKey;
	T_RCHK_COLM*             m_pColmData;
	CMap<int,int,T_RCHK_K,T_RCHK_K> m_arRchkKey;

	CDgnSectionDrawWnd       m_SectWnd;
	CDgnColumnGridWndCIVIL*  m_ColGrid;  

	void SetInitRebarCB();
	void SetInitLayerCB();
	void SetData2Dlg();
	void SetDlg2Data();
	void SetDataToGrid();
	void GetDataFromGrid();
	BOOL CheckInputData();
	void UpdateViewWnd();
	void UpdateAsInfo(double dAsc);

	// Generated message map functions
	//{{AFX_MSG(CDgnConColmBarDlgCIVIL)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDgnLayerCb();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnRedrawBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONCOLMBARDLGCIVIL_H__C790B45C_414F_468E_AA31_1ABA41416700__INCLUDED_)
