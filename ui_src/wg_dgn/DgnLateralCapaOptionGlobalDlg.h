#if !defined(AFX_DGNLATERALCAPAOPTIONGLOBALDLG_H__20F42ABD_566F_4361_8EBD_0C204F318FD9__INCLUDED_)
#define AFX_DGNLATERALCAPAOPTIONGLOBALDLG_H__20F42ABD_566F_4361_8EBD_0C204F318FD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnLateralCapaOptionGlobalDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\wg_base\wg_base_DlgChild.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "DgnLateralCapaOptionDef.h"

//#include "CMDlgBase.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnLateralCapaOptionGlobalDlg dialog
class CDgnLateralCapaOptionDlg;

class CDgnLateralCapaOptionGlobalDlg : public CDlgChild
{
// Construction
public:
	CDgnLateralCapaOptionGlobalDlg(LateralCapaDataAll* pDataAll, CWnd* pParent = NULL);   // standard constructor

public:
	CDBDoc* m_pDoc;
	CDgnLateralCapaOptionDlg *m_pParent;

	LateralCapaDataAll* m_pDataAll;	

// Dialog Data
	//{{AFX_DATA(CDgnLateralCapaOptionGlobalDlg)
	enum { IDD = IDD_DGN_LATERAL_CAPA_OPT_GLOBAL_DLG };
	CFormulaEditSpin	m_IterationSpin;
	MComboBox			m_cLoadCaseCobx;
	MButton				m_EarthquakeTypeIChk;
	MButton				m_EarthquakeTypeIIChk;
	int					m_nRegionalKind;
	int					m_nRegionalKind2;
	int					m_nGroundKind;	
	int					m_nInitForceType;
	int					m_nYieldPoint;
	MEdit				m_edtNoSectDivision;
	MEdit				m_edtMaxIteration;
	MEdit				m_edtTolerance;
	MComboBox			m_cCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnLateralCapaOptionGlobalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Dlg2Data(); 
		
protected:
	BOOL Data2Dlg();
	BOOL Apply();
	void InitComboBox();
	void AdjustComboListBox(CComboBox& Cbx) /* 콤보박스의 길이를 현재 사용되고 있는 리스트의 길이에 맞추어 변경한다. */;
	void EnableCtrl();
	void ShowHideCtrl();
	void SetCtrlPos();

	// Generated message map functions
	//{{AFX_MSG(CDgnLateralCapaOptionGlobalDlg)
	//afx_msg void OnRemoveDataBtn();	
	afx_msg void OnEquilibriumRdo();
	afx_msg void OnLoadcaceRdo();
	afx_msg void OnUserDefinedRdo();  
	afx_msg void OnTypeIChk();
	afx_msg void OnTypeIIChk();
	afx_msg void OnSelchangeModelCobx();
	afx_msg void OnDestroy();

	virtual BOOL OnInitDialog();  
	virtual void OnOK();
	virtual void OnCancel();  
	//afx_msg void OnDeltaposIterationSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CArray<UINT, UINT> m_aCtrlRsnCategory2; // 도로교 비상서V(평성 24년) 일 때 활성화 되는 Radio들
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNLATERALCAPAOPTIONGLOBALDLG_H__20F42ABD_566F_4361_8EBD_0C204F318FD9__INCLUDED_)
