#if !defined(__CMMVHLITEMUSRRUSDLG_H__)
#define __CMMVHLITEMUSRRUSDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemUsrRusDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrRusDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMMvhlItemUsrRusDlg : public CDialogMove
{
// Construction
public:
	CCMMvhlItemUsrRusDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVHL_D &ParamData) { m_csOldName = ParamData.VehicleLoadName; m_Data = ParamData; m_bModify = TRUE; }

//----------------------------------------------------------------------
// Implementation
protected:

	void InitUnit();
	void InitCombo();
	void AlignControl();
	void ControlsShowHide();
	void EnableDisableControls();
	void SetDynaFactEditBox();
	void ChangeDynaFactText();
	void ChangeLoadFactText();

	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(int nPos, int nNo, double dLoad, double dDist);
	BOOL DeleteItem(int nPos);
	BOOL ModifyItem(int nPos, int nNo, double dLoad, double dDist);

	void ChangeBitMap();	

	void Data2Dlg();
	BOOL Dlg2Data();

	int  GetLoadCount();
	int  GetDistCount();
	BOOL GetLoadAndDistValue(double &dLoad, double &dDist);

	BOOL ApplyOrOK();
	
protected:
	CDBDoc*   m_pDoc;
	T_MVHL_D  m_Data;
	
	CString   m_csOldName;
	BOOL      m_bModify;
	MillustViewer  m_wndPicture;
	
	int m_nRussiaUserType;

	CArray<UINT, UINT> m_aTrkLoad;
	CArray<UINT, UINT> m_aVariable;
	CArray<UINT, UINT> m_aLaneP;
	CArray<UINT, UINT> m_aLaneW1;
	CArray<UINT, UINT> m_aLaneW2;
	CArray<UINT, UINT> m_aLaneL;
	CArray<UINT, UINT> m_aCtrl_Fatigue;
	CArray<UINT, UINT> m_aCtrl_SameLoadedL;
	CArray<UINT, UINT> m_ACtrl_TwoVehi;
	CArray<UINT, UINT> m_aCtrl_DynaFact;
	CArray<UINT, UINT> m_aCtrl_DynaFact_UDL;
	CArray<UINT, UINT> m_aCtrl_LoadFact;
	CArray<UINT, UINT> m_aCtrl_LoadFact_UDL;
	CArray<UINT, UINT> m_aCtrl_2ndGrpReduc;
	CArray<UINT, UINT> m_aRatioEmptyCar;
	CArray<UINT, UINT> m_aCtrl_LaneFacts1;
	CArray<UINT, UINT> m_aCtrl_OKCancel;
	
public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemUsrRusDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMU_RUS };

	CEditUnit m_edtVehicleName;
	MComboBox m_cmbVehicleType;
	CEditUnit	m_edtTrkP;
	CEditUnit	m_edtTrkD;
	CListCtrl	m_List;
	CEditUnit m_edtVariable;
	CTextUnit m_untVariable;
	CEditUnit m_edtLaneW1;
	CTextUnit m_untLaneW1;
	CEditUnit m_edtLaneL;
	CTextUnit m_untLaneL;
	CEditUnit m_edtLaneW2;
	CTextUnit m_untLaneW2;
	CEditUnit m_edtLaneP;
	CTextUnit m_untLaneP;
	int       m_nDynaFact;
	CEditUnit m_edtMatlType;
	CEditUnit m_edtBridgeType;
	CEditUnit m_edtAutoDynaFact;
	CEditUnit m_edtUserDynaFact;
	CEditUnit m_edtUserDynaFact_UDL;
	BOOL      m_bFatigue;
	BOOL      m_bSameLoadedL;
	int       m_nLoadFact;
	CEditUnit m_edtUserLoadFact;
	CEditUnit m_edtUserLoadFact_UDL;
	CEditUnit m_edtRatioEmptyCar;
	BOOL      m_bTwoVehi;
	CEditUnit m_edtTwoVehi;
	BOOL      m_b2ndGrpReduc;
	CEditUnit m_edt2ndGrpReduc;
	CEditUnit m_edtLaneFacts1_Bogie[3];
	CEditUnit m_edtLaneFacts1_UDL[3];
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemUsrRusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemUsrRusDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdMvhlVehicleTypeCmb();
	afx_msg void OnCmdMvhlAddBtn();
	afx_msg void OnCmdMvhlInsBtn();
	afx_msg void OnCmdMvhlModBtn();
	afx_msg void OnCmdMvhlDelBtn();
	afx_msg void OnCmdMvhlApplyBtn();
	afx_msg void OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdMvhlFatigueChk();
	afx_msg void OnCmdMvhlDynaFactRdo();
	afx_msg void OnCmdMvhlLoadFactRdo();
	afx_msg void OnCmdMvhlTwoVehiChk();
	afx_msg void OnCmdMvhl2ndReducChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVHLITEMUSRRUSDLG_H__)
