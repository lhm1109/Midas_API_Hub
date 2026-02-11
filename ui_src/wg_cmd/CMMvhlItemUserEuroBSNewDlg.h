#if !defined(AFX_CMMVHLITEMUSEREUROBSNEWDLG_H__A94DF37F_0ED5_454E_924E_9149D3052495__INCLUDED_)
#define AFX_CMMVHLITEMUSEREUROBSNEWDLG_H__A94DF37F_0ED5_454E_924E_9149D3052495__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemUserEuroBSNewDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

#include "CMMvhlItemUserLoadGrid.h"
#include "CMMvhlItemStdLaneGrid.h"
#include "CMMvhlItemUserLoadGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUserEuroBSNewDlg dialog
#include "HeaderPre.h"


class __MY_EXT_CLASS__ CCMMvhlItemUserEuroBSNewDlg : public CDialogMove
{
// Construction
public:
	CCMMvhlItemUserEuroBSNewDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMMvhlItemUserEuroBSNewDlg();

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVHL_D &ParamData);      
//----------------------------------------------------------------------
// Implementation
protected:
	void Data2Dlg();
	BOOL Dlg2Data(int nSubType);
	void Data2DlgLM1();
	BOOL Dlg2DataLM1();
	void Data2DlgLM2();
	BOOL Dlg2DataLM2();
	void Data2DlgLM3();
	BOOL Dlg2DataLM3();
	void Data2DlgFLM3();
	BOOL Dlg2DataFLM3();
	void Data2DlgLM3Stradd();
	BOOL Dlg2DataLM3Stradd();
	void Data2DlgRailTrafficLoad();
	BOOL Dlg2DataRailTrafficLoad();
	int  GetCodeType();
	int  GetSubType(int nCodeType=0);
	void SetSubType(int nCodeType, int nSubType);

	
	BOOL ApplyOrOK();
	void AlignControl(BOOL bInitial=FALSE);
	void ShowHideControls();
	void EnableDisableControls();
	void InitControls();
	void InitUnit();

	void SetHeaderTitle4TrainList();
	void MakeItemEx4TrainList();
	BOOL InsertItem4TrainList(int nPos, int nNo, double dLoad, double dDist);
	int  GetTrainLoadCount();
	int  GetTrainDistCount();

	void SetHeaderTitle4TruckList();
	void MakeItemEx4TruckList(int nSubType);
	BOOL InsertItem4TruckList(int nPos, int nNo, double dLoad, double dDist);
	BOOL DeleteItem4TruckList(int nPos);
	BOOL ModifyItem4TruckList(int nPos, int nNo, double dLoad, double dDist);

	BOOL GetLoadAndDistValue(double &dLoad, double &dDist);
	int GetTruckLoadCount(int nSubType);
	int GetTruckDistanceCount(int nSubType);

	void  ChangeData();
	void  ChangeEditText();
	void	ChangeBitMap(int which);	
	void  ChangeCtrlText();
	
protected:
	CDBDoc*   m_pDoc;
	T_MVHL_D  m_Data;
	T_MVHL_D  m_Data_RoadFootBridge[6]; // 이건 아마 DLG 정보를 유지하기 위해서 만든 구조체 같은데...음...
	T_MVHL_D  m_Data_RailTrafficLoad[6];
	CString   m_csOldName;
	BOOL      m_bModify;
	MillustViewer  m_wndPicture;

	int       m_nCodeType;
	int       m_nSubType;
	CCMMvhlItemStdLaneGrid m_wndLM1Grid;
	CCMMvhlItemUserLoadGrid m_wndLM3Grid[3];
	
public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemUserEuroBSNewDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMU_EURO_BS };
	MComboBox	m_wndCodeName;
	MEdit	m_wndVehicleName;
	MEdit   	m_LM1Dedt;	
	CTextUnit m_LM1Dunit;
	MEdit   	m_LM1DAFedt;	

	MEdit	    m_LM2Pedt;
	MEdit	    m_LM2Dedt;
	CListCtrl	m_lstTruckL;
	MEdit	    m_LM2AFedt;
	MEdit	    m_LM2AFedt2;
	MEdit     m_LM2Wedt;
	CTextUnit m_LM2Wunit;
	CEditUnit m_edtLM2TSFact;
	CEditUnit m_edtLM2UDLFact;

	MButton   m_LM3LCchk[3];
	CFormulaEditSpin m_LM3NOPedt[3];
	MButton   m_LM3DAFchk[3];
	MEdit     m_LM3DAFedt[3];
	
	CEditUnit m_edtFLM3Interval;
	CTextUnit m_untFLM3Interval;
	MButton   m_FLM3DAFchk[2];
	MEdit     m_FLM3DAFedt[2];

	CEditUnit m_edtTSFact;
	CEditUnit m_edtUDLFact;

	CEditUnit m_edtWheelSpacing;
	CTextUnit m_untWheelSpacing;
	BOOL      m_bAxleSpacing;
	CEditUnit m_edtAxleNum;
	CEditUnit m_edtMinSpacing;
	CTextUnit m_untMinSpacing;
	CEditUnit m_edtMaxSpacing;
	CTextUnit m_untMaxSpacing;
	BOOL      m_bDynamic;
	int       m_nDynaType;
	CEditUnit m_edtPhi;

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
	CEditUnit m_edtHSLMBNum;
	CEditUnit m_edtHSLMBForce;
	CTextUnit m_untHSLMBForce;
	CEditUnit m_edtHSLMBDist;
	CTextUnit m_untHSLMBDist;
	CEditUnit m_edtDynEffFact1;
	CEditUnit m_edtDynEffFact2;
	CEditUnit m_edtAlpha;
	BOOL      m_bLongiDist;
	CEditUnit m_edtDistPoint;
	CTextUnit m_untDistPoint;
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
	//{{AFX_VIRTUAL(CCMMvhlItemUserEuroBSNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strText;
	CArray<UINT,UINT> m_aCtrlLoadTypeCode1;
	CArray<UINT,UINT> m_aCtrlLoadTypeCode2;
	CArray<UINT,UINT> m_aCtrlCom1;
	CArray<UINT,UINT> m_aCtrlCom2;
	CArray<UINT,UINT> m_aCtrlCom3;
	CArray<UINT,UINT> m_aCtrlCom4;
	CArray<UINT,UINT> m_aCtrlCom5;
	CArray<UINT,UINT> m_aCtrlCom6;
	CArray<UINT,UINT> m_aCtrlCom3_1;
	CArray<UINT,UINT> m_aCtrlCom3_2;
	CArray<UINT,UINT> m_aCtrlCom3_3;

	CArray<UINT,UINT> m_aListEtc;
	CArray<UINT,UINT> m_aHSLM_A;
	CArray<UINT,UINT> m_aHSLM_B;
	CArray<UINT,UINT> m_aDynEff;
	CArray<UINT,UINT> m_aETC;
	CArray<UINT,UINT> m_aCtrlPatch;

	int m_bPermitLoadCall;


	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemUserEuroBSNewDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeCmdMvhlCodeCmb();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdBtnLM2Delete();
	afx_msg void OnCmdBtnLM2Insert();
	afx_msg void OnCmdBtnLM2Modify();
	afx_msg void OnCmdBtnLM2Add();
	afx_msg void OnCmdMlMvhlItemTypeRdo();
	afx_msg void OnItemchangedCmdLM2List(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeCmdMlcNumEdt1();
	afx_msg void OnChangeCmdMlcNumEdt2();
	afx_msg void OnChangeCmdMlcNumEdt3();
	afx_msg void OnThreeDynamicChk1();
	afx_msg void OnThreeCmdAutoRdo1();
	afx_msg void OnThreeDynamicChk2();
	afx_msg void OnThreeCmdAutoRdo2();
	afx_msg void OnThreeDynamicChk3();
	afx_msg void OnThreeCmdAutoRdo3();
	afx_msg void OnTwoDynamicChk1();
	afx_msg void OnTwoDynamicChk2();
	afx_msg void OnThreeUseChk1();
	afx_msg void OnThreeUseChk2();
	afx_msg void OnThreeUseChk3();
	afx_msg void OnCmdLM3StraddAxleSpacingChk();
	afx_msg void OnCmdLM3StraddDynamicChk();
	afx_msg void OnCmdLM3StraddPhiTypeRdo();
	afx_msg void OnCmdMvhlEuroLongiDistChk();
	afx_msg void OnCmdMvhlEuroEccenVertLoadChk();
	afx_msg void OnChkPatch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMUSEREUROBSNEWDLG_H__A94DF37F_0ED5_454E_924E_9149D3052495__INCLUDED_)
