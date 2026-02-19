#if !defined(AFX_DGNCONBEAMBARDLGCIVIL_H__46AC7672_1199_445C_8790_AB7FD0249E4F__INCLUDED_)
#define AFX_DGNCONBEAMBARDLGCIVIL_H__46AC7672_1199_445C_8790_AB7FD0249E4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBeamBarDlgCIVIL.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnSectionDrawWnd.h"

/////////////////////////////////////////////////////////////////////////////
// DgnConBeamBarDlgCIVIL dialog
class CDgnBeamGridWndCIVIL;

class DgnConBeamBarDlgCIVIL : public CDialogMove
{
// Construction
public:
	DgnConBeamBarDlgCIVIL(CWnd* pParent = NULL);   // standard constructor

	void SetRchkData(int nIMJ, T_RCHK_K m_RchkKey, CMap<int,int,T_RCHK_K,T_RCHK_K>& arRchkKey, T_RCHK_BEAM* pBeamData);
	void UpdateViewWnd();

// Dialog Data
	//{{AFX_DATA(DgnConBeamBarDlgCIVIL)
	enum { IDD = IDD_DGN_CON_BEAM_BAR_DLG_CIVIL };
	CTextUnit	m_AsTopUT;
	CEditUnit	m_AsTop;
	CTextUnit	m_AsBotUT;
	CEditUnit	m_AsBot;
	CTextUnit	m_cStirrupSpaceUT;
	CEditUnit	m_cStirrupSpace;
	CEditUnit	m_cStirrupNum;
	CComboBox	m_cStirrupSize;
	CComboBox	m_cTopLayerCB;
	CComboBox	m_cBotLayerCB;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DgnConBeamBarDlgCIVIL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	int           m_nTopLayer, m_nBotLayer;
	int           m_nIMJ;     // 0:I  1:M  2:J

	T_RCHK_K      m_RchkKey;
	T_RCHK_BEAM*  m_pBeamData;
	CMap<int,int,T_RCHK_K,T_RCHK_K> m_arRchkKey;

	CDgnSectionDrawWnd     m_SectWnd;  // Veiw Window
	CDgnBeamGridWndCIVIL*  m_TopGrid;  // Top   Grid
	CDgnBeamGridWndCIVIL*  m_BotGrid;  // Botom Grid

	void SetData2Dlg();
	void SetDlg2Data();
	void InitStirrupCB();
	void InitLayerCB();
	BOOL CheckInputData();
	void UpdateAsInfo(double dAst, double Asb);

	void SetDataToGrid();
	void GetDataFromGrid();

	// Generated message map functions
	//{{AFX_MSG(DgnConBeamBarDlgCIVIL)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnSelchangeDgnRcBotLayerCb();
	afx_msg void OnSelchangeDgnRcTopLayerCb();
	afx_msg void OnRedrawBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONBEAMBARDLGCIVIL_H__46AC7672_1199_445C_8790_AB7FD0249E4F__INCLUDED_)
