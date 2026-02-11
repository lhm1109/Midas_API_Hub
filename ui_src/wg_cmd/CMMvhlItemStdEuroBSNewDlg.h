#if !defined(__CMMVHLITEMSTDEUROBSNEWDLG_H__)
#define __CMMVHLITEMSTDEUROBSNEWDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdEuroBSNewDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

#include "CMMvhlItemStdLaneGrid.h"
#include "CMMvhlItemStdLoadGrid.h"

#define D_MVHL_EURO_BS_ROADBRIDGE             0
#define D_MVHL_EURO_BS_FOOTBRIDGE             1
#define D_MVHL_EURO_BS_ROADBRIDGE_FATIGUE     2
#define D_MVHL_EURO_BS_RAIL_TRAFFIC_LOAD      3

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdEuroBSNewDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhlItemStdEuroBSNewDlg : public CDialogMove
{
// Construction
public:
	CCMMvhlItemStdEuroBSNewDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMMvhlItemStdEuroBSNewDlg();

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVHL_D &ParamData) 
	{ m_csOldName = ParamData.VehicleLoadName; m_Data = ParamData; m_bModify = TRUE; }

//----------------------------------------------------------------------
protected:
	void ChangeBitmap(int nCodeType, int nLoadType, int nSelVehicle);
	BOOL ChangeData();
	void ChangeCtrlText();
	void ChangeEditText();

//----------------------------------------------------------------------
// Implementation
protected:
	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(int nPos, int nNo, double dLoad, double dDist);
	int  GetTrainLoadCount();
	int  GetTrainDistCount();
	
	void InitControls();
	void InitUnit();
	void Data2Dlg();
	BOOL Dlg2Data();  
	void AlignControl();
	void ControlsShowHide();
	void EnableDisableControls();
	//void SetCtrl(int nIndex, int sIndex);
	BOOL ApplyOrOK();
	void SetLoadTypeList(int nIndex);
	void SetSelVehicleList();
	int  GetIndex(int &nLoadType, int &nSelVehicle);  


protected:
	CDBDoc*   m_pDoc;
	T_MVHL_D  m_Data;
	int       m_nStndCode;
	
	CString   m_csOldName;
	BOOL      m_bModify;
	MillustViewer	m_wndPicture;
	CArray<UINT,UINT> m_aThreeGrid;
	CArray<UINT,UINT> m_aThreeGridTxt;
	CArray<UINT,UINT> m_aThreeGridChk1;
	CArray<UINT,UINT> m_aThreeGridChk2;
	CArray<UINT,UINT> m_aOneGrid;
	CArray<UINT,UINT> m_aOneGrid1;
	CArray<UINT,UINT> m_aPhi;
	CArray<UINT,UINT> m_aAdjust;
	CArray<UINT,UINT> m_aAdjust2;
	CArray<UINT,UINT> m_aFootway;
	CArray<UINT,UINT> m_aDynamic;
	CArray<UINT,UINT> m_aInterval;
	CArray<UINT,UINT> m_aSOSOV;
	CArray<UINT,UINT> m_aSelVehicle;
	CArray<UINT,UINT> m_aListEtc;
	CArray<UINT,UINT> m_aHSLM_A;
	CArray<UINT,UINT> m_aHSLM_B;
	CArray<UINT,UINT> m_aDynEff;
	CArray<UINT,UINT> m_aETC;
	CArray<UINT,UINT> m_aCtrlPatch;
	CCMMvhlItemStdLaneGrid m_wndOneGrid;
	CCMMvhlItemStdLoadGrid m_wndThreeGrid[3];
	BOOL m_bInit;
	
public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemStdEuroBSNewDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_EURO_BS };
	MComboBox	m_cmbCodeName;
	MEdit     m_edtVehicleName;
	MComboBox	m_cmbVehicleTypeList;
	MButton	m_wndDynamic;
	MEdit m_wndPhi;
	MEdit m_wndAdjust;
	MEdit m_wndAdjust2;
	CEditUnit m_edtInterval;
	CTextUnit m_untInterval;
	MEdit m_wndAmplificaton;
	CEditUnit m_edtTSFactor;
	CEditUnit m_edtUDLFactor;
	MComboBox m_cmbSelVihicle;
	CListCtrl m_lstTrain;
	CEditUnit m_edtTrainW1;
	CTextUnit m_untTrainW1;
	CEditUnit m_edtTrainDD1;
	CTextUnit m_untTrainDD1;
	CEditUnit m_edtTrainD1;
	CTextUnit m_untTrainD1;
	CEditUnit m_edtTrainW2;
	CTextUnit m_untTrainW2;
	CEditUnit m_edtTrainDD2;
	CTextUnit m_untTrainDD2;
	CEditUnit m_edtTrainD2;
	CTextUnit m_untTrainD2;
	CEditUnit m_edtHSLMANum;
	CEditUnit m_edtHSLMALength;
	CTextUnit m_untHSLMALength;
	CEditUnit m_edtHSLMASpacing;
	CTextUnit m_untHSLMASpacing;
	CEditUnit m_edtHSLMAForce;
	CTextUnit m_untHSLMAForce;
	CEditUnit m_edtDynEffFact1;
	CEditUnit m_edtDynEffFact2;
	CEditUnit m_edtHSLMBNum;
	CEditUnit m_edtHSLMBForce;
	CTextUnit m_untHSLMBForce;
	CEditUnit m_edtHSLMBDist;
	CTextUnit m_untHSLMBDist;
	CEditUnit m_edtAlpha;
	BOOL      m_bLongiDist;
	CEditUnit m_edtDistPoint;
	CTextUnit m_untDistPoint;
	BOOL      m_bLM3LoadCase1;
	BOOL      m_bLM3LoadCase2;
	CEditUnit m_edtFootway;
	CTextUnit m_untFootway;
	BOOL      m_bEccenVertLoad;
	CEditUnit m_edtEccenVertLoad;
	CTextUnit m_untEccenVertLoad;

	MButton	m_chkPatch;
	CEditUnit m_edtPatchW;
	CTextUnit m_untPatchW;
	CEditUnit m_edtPatchL;
	CTextUnit m_untPatchL;

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdEuroBSNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nTrainType;
	int m_nLaneType;
	BOOL  m_bPatchGrid;

	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdEuroBSNewDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdMvhlEuroApply();
	afx_msg void OnChkPatch();
	afx_msg void OnSelchangeCmdMvhlTypeCombo();
	afx_msg void OnSelchangeCmdMvhlCodeCmb();
	afx_msg void OnCmdMvhlEuroDynamicChk();
	afx_msg void OnCmdMvhlEuroAutoRdo();
	afx_msg void OnCmdMvhlEuroSelVehicleCmb();
	afx_msg void OnCmdMvhlEuroLongiDistChk();
	afx_msg void OnCmdMvhlEuroEccenVertLoadChk();
	afx_msg void OnPatchBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString ConvVehicleTypeStrRaw2Trans(const CString& strRaw) const;
	CString ConvVehicleTypeStrTrans2Raw(const CString& strTrans) const;

	CString GetVehicleTypeStr() const;
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVHLITEMSTDEUROBSNEWDLG_H__)
