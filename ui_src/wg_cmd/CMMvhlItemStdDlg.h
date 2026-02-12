#if !defined(AFX_CMMVHLITEMSTDDLG_H__BA9D23AB_21D8_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMMVHLITEMSTDDLG_H__BA9D23AB_21D8_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

#include "..\wg_db\VehlDef.h"
#include "CMMvhlItemStdLoadGrid.h"

//hslee : 여기있던 Standard Code Define들 VehlDef.h로 옮겼음
 
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhlItemStdDlg : public CDialogMove
{
// Construction
public:
	CCMMvhlItemStdDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMMvhlItemStdDlg();

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVHL_D &ParamData) 
	{ m_csOldName = ParamData.VehicleLoadName; m_Data = ParamData; m_bModify = TRUE; }

//----------------------------------------------------------------------
// picture window <--- nowlee *^o^*
public:
	void SetHeaderTitle();
	void MakeItemEx();
	void MakeTruckData();
	BOOL InsertItem(int nPos, int nNo, double dLoad, double dDist);
	int  GetTruckLoadCount();
	int  GetTruckDistCount();

//----------------------------------------------------------------------
// Implementation
protected:
	void ChangeBitmap(int nIndex, int nLoadType);
	BOOL ChangeData();
	void Data2Dlg();
	BOOL Dlg2Data();  
	void AlignControl();
	void AlignControlDynamic(BOOL bResize = TRUE);  
	BOOL ApplyOrOK();  
	void SetLoadTypeList(int nIndex);
	void GetIndex(int& nLoadType);
	void SetChinaAndAashtoLrfdCtrl(int nIndex, int sIndex);  
	BOOL IsLegal(int nIndex, int nLoadType);
	BOOL IsPermit(int nIndex, int nLoadType);
	void EnableDisableIncreAxleSpace();
	void InitRcAr();
	void UpdateLegalDescription1STC();
	void UpdateLegalDescription2STC();
protected:
		
	CDBDoc*   m_pDoc;
	T_MVHL_D  m_Data;
	int       m_nStndCode;
	
	CString   m_csOldName;
	BOOL      m_bModify;
	MillustViewer	m_wndPicture;
	
	CArray<UINT, UINT> m_aCtrlCom, m_aCtrlCh, m_aCtrlAashtoLrfd;
	CArray<UINT, UINT> m_aCtrlIrc1;

	CArray<UINT, UINT> m_aCtrlBS5400, m_aCtrlBS3701, m_aCtrlBSAddData;
	CArray<UINT, UINT> m_aCtrlBSCSAddData;

	CArray<UINT, UINT> m_aCtrlBS, m_aCtrlJTGLane, m_aCtrlJTGTruck, m_aCtrlJTGCrowd;
	CArray<UINT, UINT> m_aCtrlSoSov;

	CArray<UINT, UINT> m_aBSBD3701LaneFactor; // BS BD37/01 Lane Factor
	CArray<UINT, UINT> m_aGroupBox1;          // 전체 Group Box
	CArray<UINT, UINT> m_aGroupBox2;          // BS BD37/01 Group Box
	CArray<UINT, UINT> m_aOKCancelApply;      // OK, Cancel, Apply Button

	CArray<UINT, UINT> m_aBSCS454LaneFactor, m_aBSCS454LaneFactorCmb; // BS CS 454 Lane Factor

	CArray<UINT, UINT> m_aCtrlBS_UnitNum;
	CArray<UINT, UINT> m_aCtrlBS_Pa;
	CArray<UINT, UINT> m_aCtrlBS_Pb;
	CArray<UINT, UINT> m_aCtrlBS_dd;
	CArray<UINT, UINT> m_aCtrlBS_Ped;
	CArray<UINT, UINT> m_aCtrlBS_HA, m_aCtrlBS_HB;

	CArray<UINT, UINT> m_aDyna; // Dynamic Load Allowance

	CArray<UINT, UINT> m_aCJJ11Crowd;
	
	CArray<UINT, UINT> m_aCtrlPhi;
	CArray<UINT, UINT> m_aCtrlDD;
	CArray<UINT, UINT> m_aCtrlz;

	CArray<UINT, UINT> m_aCtrlPenndot;

	CArray<UINT, UINT> m_aCtrlPatch;

	CRect m_rectPosVehiLoadProCommon;
	CRect m_rectPosVehiLoadProBS_BD8611;  

public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemStdDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMS };
	CEditUnit m_edtBSdd;
	CTextUnit m_untBSdd;
	CEditUnit m_edtPedW1;
	CTextUnit m_untPedW1;
	MEdit     m_edtPedW2;
	CTextUnit m_untPedW2;
	CEditUnit m_edtPedL1;
	CTextUnit m_untPedL1;
	MEdit     m_edtPedL2;
	CTextUnit m_untPedL2;
	CEditUnit m_edtNumUnit;
	CEditUnit m_untDynLoadAllowance;
	MEdit     m_edtVehicleName;
	CTextUnit	m_stQq;
	CTextUnit	m_stQm;
	CTextUnit	m_stP;
	MComboBox	m_cbxCodeName;
	MComboBox	m_cmbSelVihicle;
	CTextUnit	m_stW;
	CTextUnit	m_stPs;
	CTextUnit	m_stPm;
	CTextUnit	m_stdW2;
	CTextUnit	m_stdW1;
	CTextUnit	m_stdD2;
	CTextUnit	m_stdD1;
	CListCtrl	m_List;
	MComboBox	m_wndVehicleTypeList;
	double	m_dD1;
	double	m_dD2;
	double	m_dW1;
	double	m_dW2;
	double	m_W;
	double	m_Ps;
	double	m_Pm;
	double	m_dP;
	double	m_dQm;
	double	m_dQq;
	double	m_dBSW1;
	double	m_dBSW2;
	double	m_dBSW3;
	double	m_dBSL1;
	double	m_dBSL2;
	double	m_dBSPa;
	double	m_dBSPb;
	double	m_dBSD1;
	double	m_dBSD2;
	double	m_dBSD;
	double  m_dBSD3;
	double	m_dBSD4;
	double	m_dBSD5;
	double	m_dBSD6;

	double	m_dBS37W1;
	double	m_dBS37W2;
	double	m_dBS37W3;
	double	m_dBS37L1;
	double	m_dBS37L2;
	double	m_dBS37L21;
	double	m_dBS37L3;
	CString 	m_strBS37W1_exp;
	CString 	m_strBS37W2_exp;
	CTextUnit	m_untBS37W1;
	CTextUnit	m_untBS37W2;
	CTextUnit	m_untBS37W3;
	CTextUnit	m_untBS37L1;
	CTextUnit	m_untBS37L2;
	CTextUnit	m_untBS37L3;

	CString 	m_strBSW2_exp;
	CTextUnit	m_untBSW1;
	CTextUnit	m_untBSW2;
	CTextUnit	m_untBSW3;
	CTextUnit	m_untBSL1;
	CTextUnit	m_untBSL2;
	CTextUnit	m_untBSPa;
	CTextUnit	m_untBSPb;
	CTextUnit	m_untBSD1;
	CTextUnit	m_untBSD2;
	CTextUnit	m_untBSD;
	CTextUnit	m_untBSD3;
	CTextUnit	m_untBSD4;
	CTextUnit	m_untBSD5;
	CTextUnit	m_untBSD6;
	CTextUnit	m_untJTGQkLane;
	CTextUnit	m_untJTGPk1Lane;
	CTextUnit	m_untJTGPk2Lane;
	CTextUnit	m_untJTGL1Lane;
	CTextUnit	m_untJTGL2Lane;
	CTextUnit	m_untJTGW1Crowd;
	CTextUnit	m_untJTGW2Crowd;
	CTextUnit	m_untJTGL1Crowd;
	CTextUnit	m_untJTGL2Crowd;
	CTextUnit m_untJTGWdCrowd;
	double	m_dQkJTGLane;
	double	m_dPk1JTGLane;
	double	m_dPk2JTGLane;
	double	m_dL1JTGLane;
	double	m_dL2JTGLane;
	double	m_dW1JTGCrowd;
	double	m_dW2JTGCrowd;
	double	m_dL1JTGCrowd;
	double	m_dL2JTGCrowd;
	//double  m_dWdJTGCrowd;
	CEditUnit m_edtJTGCrowd;
 	MButton	m_wndBtnApply;
	
	// BS BD 37/01 Lane Factor
	int   m_nLaneFactor;

	// BS CS 454 Lane Factor
	int   m_nLaneFactorCS;

	MButton   m_chkIncreAxleSpace;
	CEditUnit m_edtIncreAxleSpace;
	CTextUnit m_untIncreAxleSpace;
	int       m_nDynaType;
	CEditUnit m_edt1Axle;
	CEditUnit m_edt2Axle;
	CEditUnit m_edt3Axle;
	
	CEditUnit m_edtCJJ11wp;
	CTextUnit m_undCJJ11wp;

	MButton	m_wndDynamic;
	MEdit m_wndPhi;
	MButton	m_chkOverLoadFactor;
	CEditUnit m_edtOverCriAxle;
	CEditUnit m_edtOverOtherAxle;

	CCMMvhlItemStdLoadGrid m_wndThreeGrid[3];

	CEditUnit m_edtDD;
	CTextUnit m_untDD;
	
	// BS BD73/01 Add Data
	CEditUnit m_edtaL;
	CTextUnit m_untaL;
	MComboBox m_cbxCategory;
	MComboBox m_cbxLoadLevel;
	MButton   m_chkAddData;

	// BS CS 454 Add Data
	MComboBox m_cbxCategoryCS;
	MComboBox m_cbxLoadLevelCS;

	CEditUnit m_edtLegalW;
	CTextUnit m_untLegalW;
	CEditUnit m_edtLegalR;
	CTextUnit m_untLegalR;
	CEditUnit m_edtLegalDist;
	CTextUnit m_untLegalDist;
	CEditUnit m_edtz;

	CEditUnit m_edtPenndotW;
	CTextUnit m_untPenndotW;
	CEditUnit m_edtPenndotR1;
	CTextUnit m_untPenndotR1;
	CEditUnit m_edtPenndotR2;
	CTextUnit m_untPenndotR2;
	CEditUnit m_edtPenndotDist;
	CTextUnit m_untPenndotDist;

	CEditUnit m_edtTdmW;
	CTextUnit m_untTdmW;
	CEditUnit m_edtTdmD2;
	CTextUnit m_untTdmD2;
	CEditUnit m_edtTdmD3;
	CTextUnit m_untTdmD3;

	// AASHTO-LRFD
	MButton	m_chkCentF;
	MButton	m_chkTDM;

	MButton	m_chkPatch;
	CEditUnit m_edtPatchW;
	CTextUnit m_untPatchW;
	CEditUnit m_edtPatchL;
	CTextUnit m_untPatchL;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nMovingType;
	int m_nCodeType;
	int m_nTrainType;
	int m_nLaneType;
	BOOL m_bSurface;
	BOOL m_bPatchLoad;
	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdDlg)
	virtual BOOL OnInitDialog();

	void SetDefValAndCbxItem();
	void SetBS_BD3701AddData();
	void SetBS_CS454AddData();

	void SetCtrlUnit();
	void SaveCtrlPos();
	void InitThreeGrid(int nLoadType);
	CString GetDescListTxt(int nIndex);
	CString GetDescListChk(int nIndex);
	void InitGrid();
	void ChangeCtrlText();
	void SetEnableBS();
	void SetEnableBS_CS();

	virtual void OnOK();
	afx_msg void OnCmdApply();
	afx_msg void OnSelchangeCmdMvhlCodeCmb();
	afx_msg void ResetVehicleTypeCmb();
	afx_msg void OnChangeLaneFactor();
	afx_msg void OnChangeLaneFactorCS();
	afx_msg void OnChangeDynaTypeRdo();
	afx_msg void OnBnClickedIncreAxleSpaceChk();  
	afx_msg void OnCmdMvhlEuroDynamicChk();
	afx_msg void OnCmdMvhlEuroAutoRdo();
	afx_msg void OnCmdMvhlOverLoadRdo();
	afx_msg void OnCmdMvhlOverLoadChk();
	afx_msg void OnSelchangeCmdMvhlTypeCombo();
	afx_msg void OnHALaneFactor();
	afx_msg void OnChkAddData();
	afx_msg void OnChkPatch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMSTDDLG_H__BA9D23AB_21D8_11D4_92DE_0000C0B0E6B3__INCLUDED_)
