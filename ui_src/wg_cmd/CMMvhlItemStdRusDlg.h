#if !defined(__CMMVHLITEMSTDRUSDLG_H__)
#define __CMMVHLITEMSTDRUSDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdRusDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdRusDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhlItemStdRusDlg : public CDialogMove
{
// Construction
public:
	CCMMvhlItemStdRusDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMMvhlItemStdRusDlg();

//----------------------------------------------------------------------
// Implementation
public:
	void SetParamData(T_MVHL_D &ParamData) 
	{ m_csOldName = ParamData.VehicleLoadName; m_Data = ParamData; m_bModify = TRUE; }

public:
	void InitUnit();
	void InitCombo();
	void ChangeCombo();
	void SetDynaFactEditBox();
	void ChangeDynaFactText();
	void ChangeLoadFactText();
	void AlignControl();
	void ControlsShowHide();
	void EnableDisableControls();

	void SetHeaderTitle();
	BOOL InsertItem(int nPos, int nNo, double dLoad, double dDist);
	void MakeItemEx();
	int  GetLoadCount();
	int  GetDistCount();

	void ChangeBitmap(int nIndex, int nBitmap, int nBridgeType);

	void Data2Dlg();
	BOOL Dlg2Data();  
	BOOL ApplyOrOK();
	BOOL ChangeData();

	void SetLoadTypeList(int nIndex);
	void  GetIndex(CString &csName, int *sitem=NULL);  
	int  GetIndex_BridgeType();
	void AdjustComboListBox(CComboBox& Cbx);

protected:
	
	CDBDoc*   m_pDoc;
	T_MVHL_D  m_Data;
	int       m_nStndCode;

	int       m_nVehicleIndex;
	
	CString   m_csOldName;
	BOOL      m_bModify;
	MillustViewer	m_wndPicture;

	CArray<UINT, UINT> m_aCtrl_BridgeType;
	CArray<UINT, UINT> m_aCtrl_K;
	CArray<UINT, UINT> m_aCtrl_W;
	CArray<UINT, UINT> m_aCtrl_List;
	CArray<UINT, UINT> m_aCtrl_Fatigue;
	CArray<UINT, UINT> m_aCtrl_SameLoadedL;
	CArray<UINT, UINT> m_ACtrl_TwoVehi;
	CArray<UINT, UINT> m_aCtrl_DynaFact;
	CArray<UINT, UINT> m_aCtrl_DynaFact_UDL;
	CArray<UINT, UINT> m_aCtrl_LoadFact;
	CArray<UINT, UINT> m_aCtrl_LoadFact_UDL;
	CArray<UINT, UINT> m_aCtrl_2ndGrpReduc;
	CArray<UINT, UINT> m_aCtrl_TramTop;
	CArray<UINT, UINT> m_aCtrl_TramBot;
	CArray<UINT, UINT> m_aCtrl_LaneFacts1;
	CArray<UINT, UINT> m_aCtrl_EpsilonFact;
	CArray<UINT, UINT> m_aCtrl_OKCancel;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemStdRusDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_RUS };

	MComboBox	m_cmbCodeName;
	CEditUnit m_edtVehicleName;
	MComboBox	m_cmbVehicleType;
	MComboBox m_cmbBridgeType;
	CListCtrl	m_List;
	CEditUnit m_edtK;
	CEditUnit m_edtW;
	CTextUnit m_untW;
	CEditUnit m_edtD;
	CTextUnit m_untD;
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
	CEditUnit m_edtVariableD5;
	CTextUnit m_untVariableD5;
	BOOL      m_bTwoVehi;
	CEditUnit m_edtTwoVehi;
	BOOL      m_b2ndGrpReduc;
	CEditUnit m_edt2ndGrpReduc;
	CEditUnit m_edtLaneFacts1_Bogie[3];
	CEditUnit m_edtLaneFacts1_UDL[3];
	int       m_nEpsilonFact;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdRusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdRusDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdMvhlCodeCmb();
	afx_msg void OnCmdMvhlTypeCmb();
	afx_msg void OnCmdMvhlBridgeTypeCmb();
	afx_msg void OnCmdMvhlFatigueChk();
	afx_msg void OnCmdMvhlDynaFactRdo();
	afx_msg void OnCmdMvhlLoadFactRdo();
	afx_msg void OnCmdMvhlTwoVehiChk();
	afx_msg void OnCmdMvhl2ndReducChk();
	afx_msg void OnCmdMvhlApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    CString ConvVehicleTypeStrRaw2Trans(const CString& strRaw) const;
    CString ConvVehicleTypeStrTrans2Raw(const CString& strTrans) const;
    CString ConvBridgeTypeStrRaw2Trans(const CString& strRaw) const;
    CString ConvBridgeTypeStrTrans2Raw(const CString& strTrans) const;

    CString GetVehicleTypeStr() const;
    CString GetBridgeTypeStr() const;
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVHLITEMSTDRUSDLG_H__)
