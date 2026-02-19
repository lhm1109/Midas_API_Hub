#if !defined(__DGNSEISMPHIITEMDLG_H__)
#define __DGNSEISMPHIITEMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisMPhiItemDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"

#include "DgnMPhiCurveWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnSeisMPhiItemDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CDgnSeisMPhiItemDlg(CWnd* pParent = NULL);   // standard constructor
	
	//----------------------------------------------------------------------
	// Interface
public:
	void SetParamData(T_MPCC_K Key, T_MPCC_D &ParamData) 
	{ m_Key = Key; m_Data = ParamData; m_bModify = TRUE; }

public:
// Dialog Data
	//{{AFX_DATA(CDgnSeisMPhiItemDlg)
	enum { IDD = IDD_DGN_SEIS_MPHI_ITEM_DLG };
	
	mit::frx::MEdit     m_edtName;
	BOOL      m_bEvalGrup;
	mit::frx::MComboBox m_cmbEvalGrup;
	mit::frx::MComboBox m_cmbEvalColm;
	mit::frx::MComboBox m_cmbEvalPos;
	mit::frx::MComboBox m_cmbSect;
	mit::frx::MComboBox m_cmbSectPos;
	mit::frx::MComboBox m_cmbConc1;
	mit::frx::MComboBox m_cmbConc2;
	mit::frx::MComboBox m_cmbConc1Trans;
	mit::frx::MComboBox m_cmbConc2Trans;
	mit::frx::MComboBox m_cmbSteel;
	int       m_nAxialForce;
	CSelectLC m_cmbLcom;
	CEditUnit m_edtAxialForce;
	CTextUnit m_untAxialForce;
	CEditUnit m_edtAxisAngle;
	CTextUnit m_untAxisAngle;
	CEditUnit m_edtPoint;
	BOOL      m_bIdealized;
	BOOL      m_bUserCurve;
	CEditUnit m_edtUserCurve;
	CTextUnit m_untUserCurve;
	int       m_nIdealOpt;
	CFoldGroupBox_Cross	m_chkSect;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisMPhiItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitUnit();
	void InitCombo();
	void InitEvgpColmCombo();

	void SetInMatlCobx();

	void EnableDisableControls();

	void Data2Dlg();
	BOOL Dlg2Data();

	void DrawSection();

	void UpdateName();
	void UpdateColumnName();
	void UpdateSectKey();	
	void UpdateAxialForcebyLcom();
	
	CDBDoc* m_pDoc;

	CDgnMPhiCurveWnd   m_SectWnd;
	
	T_MPCC_K m_Key;
	T_MPCC_D m_Data;
	BOOL m_bModify;

	T_SYEF_D m_SyefD;

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisMPhiItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSeisMPhiEvalGrupChk();
	afx_msg void OnDgnSeisMPhiEvalGrupCmb();
	afx_msg void OnDgnSeisMPhiEvalColmCmb();
	afx_msg void OnDgnSeisMPhiEvalPosCmb();
	afx_msg void OnDgnSeisMPhiSectCmb();
	afx_msg void OnDgnSeisMPhiSectPosCmb();
	afx_msg void OnDgnSeisMPhiRebarBtn();
	afx_msg void OnDgnSeisMPhiConcCmb();
	afx_msg void OnDgnSeisMPhiConcTransCmb();
	afx_msg void OnDgnSeisMPhiAxialForceRdo();
	afx_msg void OnDgnSeisMPhiLcomCmb();
	afx_msg void OnDgnSeisMPhiLcomBtn();
	afx_msg void OnDgnSeisMPhiIdealizedChk();
	afx_msg void OnDgnSeisMPhiUserCurveChk();
	afx_msg void OnDgnSeisMPhiOptGuideBtn();
	afx_msg void OnDgnSeisMPhiECUCBtn();
	afx_msg void OnDgnSeisMPhiBtn();
	afx_msg void OnDgnSeisMPhiOKBtn();
	afx_msg void OnDgnSeisMPhiCancelBtn();
	afx_msg void OnDgnSeisMPhiApplyBtn();
	afx_msg void OnIMatlBtn();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISMPHIITEMDLG_H__)
