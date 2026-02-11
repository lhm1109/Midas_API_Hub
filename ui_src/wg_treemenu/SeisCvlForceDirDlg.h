#if !defined(__SEISCVLFORCEDIRDLG_H__)
#define __SEISCVLFORCEDIRDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisCvlForceDirDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_dgn\BrdgSeisEval.h"

#include "SeisCvlForceDirGrid.h"
#include "SeisCvlForceDirDlgDrawEditBtn.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlForceDirDlg dialog
class CSeisCvlForceDirDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:  
	CSeisCvlForceDirDlg(CWnd* pParent = NULL);   // standard constructor
	~CSeisCvlForceDirDlg();

// Dialog Data
	//{{AFX_DATA(CSeisCvlForceDirDlg)
	enum { IDD = IDD_TM_SEIS_CVL_MISC_FORCE_DIR };
	
	MComboBox m_cmbLcom;
	CEdit     m_edtBehav;
	int       m_nCalcMethod;
	MComboBox m_cmbMPhiTop;
	MComboBox m_cmbMPhiBot;
	MComboBox m_cmbBearing;
	CEditUnit m_edtSum;
	CTextUnit m_untSum;
	CEditUnit m_edtHe1;
	CTextUnit m_untHe1;
	CEditUnit m_edtHe2;
	CTextUnit m_untHe2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCvlForceDirDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	T_EVGP_K m_EvgpK;
	int m_nEvgpColm;
	int m_nDir; // 0:Longitudinal Dir. 1:Transverse Dir.

	int m_nBehav; // 거동 특성 0:라멘거동, 1:캔틸레버거동
	double m_dHe1;
	double m_dHe2;
	
protected:
	void InitUnit();
	void InitCombo();
	void SetMPhiCbxInitSel(MComboBox& Cobx, int nPos);
	void InitView();
	void InitGrid();
	void InitDialogTitle();

	void AlignControl();
	void ControlsShowHide();
	void EnableDisableControls();

	void GetEffLengthParam();

	void UpdateGraphData();
	
	CDBDoc* m_pDoc;

	CSeisCvlForceDirGrid* m_pGrid;
	CSeisCvlForceDirDlgDrawEditBtn m_ContourView;

	T_ELEM_K m_TopElemK, m_BotElemK;
	double m_dTopMy, m_dBotMy;
	double m_dHc, m_dHp;

	CBrdgSeisEval* m_pSeisEval;
	
	// Generated message map functions
	//{{AFX_MSG(CSeisCvlForceDirDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCvlForceDirLcomCmb();
	afx_msg void OnCvlForceDirLcomBtn();
	afx_msg void OnCvlForceDirBearingCmb();
	afx_msg void OnCvlForceDirCalcMethodRdo();
	afx_msg void OnCvlForceDirCalcBtn();  
	afx_msg void OnCvlForceDirOKBtn();
	afx_msg void OnCvlForceDirCancelBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEISCVLFORCEDIRDLG_H__)
