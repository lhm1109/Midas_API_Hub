/*
#if !defined(AFX_CMMVHLITEMSTDEUROBSDLG_H__DA215B23_B502_4632_8642_FEE4A61D04FA__INCLUDED_)
#define AFX_CMMVHLITEMSTDEUROBSDLG_H__DA215B23_B502_4632_8642_FEE4A61D04FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdEuroBSDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "CMMvhlItemStdLaneGrid.h"
#include "CMMvhlItemStdLoadGrid.h"

#define D_MVHL_EURO_BS_ROADBRIDGE             0
#define D_MVHL_EURO_BS_FOOTBRIDGE             1
#define D_MVHL_EURO_BS_ROADBRIDGE_FATIGUE     2
#define D_MVHL_EURO_BS_RAIL_TRAFFIC_LOAD      3

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdEuroBSDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhlItemStdEuroBSDlg : public CDialogMove
{
// Construction
public:
	CCMMvhlItemStdEuroBSDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMMvhlItemStdEuroBSDlg();

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVHL_D &ParamData) 
	{ m_csOldName = ParamData.VehicleLoadName; 
		m_Data = ParamData; 
		m_bModify = TRUE; 
	}

//----------------------------------------------------------------------


protected:
	void ChangeBitmap(int nIndex, int nBitmap);
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
	int  GetIndex(CString &csName, int *sitem=NULL);  
	//void SetChinaAndAashtoLrfdCtrl(int nIndex, int sIndex);  

protected:
	static CString m_aTypeList[][18];
	static int m_aCountList[][4][18];
	static CString m_aDescList[][4][18];
	static CString m_aSelVehicleList[10];

	CDBDoc*   m_pDoc;
	T_MVHL_D  m_Data;
	int       m_nStndCode;
	
	CString   m_csOldName;
	BOOL      m_bModify;
	CBitmap*	m_pBitmap;
	CArray<UINT,UINT> m_aThreeGrid;
	CArray<UINT,UINT> m_aOneGrid;
	CArray<UINT,UINT> m_aOneGrid1;
	CArray<UINT,UINT> m_aPhi;
	CArray<UINT,UINT> m_aAdjust;
	CArray<UINT,UINT> m_aAdjust2;
	CArray<UINT,UINT> m_aDynamic;
	CArray<UINT,UINT> m_aInterval;
	CArray<UINT,UINT> m_aSOSOV;
	CArray<UINT,UINT> m_aSelVehicle;
	CArray<UINT,UINT> m_aListEtc;
	CArray<UINT,UINT> m_aHSLM_A;
	CArray<UINT,UINT> m_aHSLM_B;
	CArray<UINT,UINT> m_aDynEff;
	CArray<UINT,UINT> m_aETC;
	CCMMvhlItemStdLaneGrid m_wndOneGrid;
	CCMMvhlItemStdLoadGrid m_wndThreeGrid[3];
	BOOL m_bInit;
	
public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemStdEuroBSDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_EURO_BS };
	CComboBox	m_wndCodeName;
	CButton	m_wndBtnApply;
	CEdit     m_wndVehicleName;
	CComboBox	m_wndVehicleTypeList;  
	CButton	m_wndDynamic;
	CEdit m_wndPhi;
	CEdit m_wndAdjust;
	CEdit m_wndAdjust2;
	CEdit m_wndInterval;
	CEdit m_wndAmplificaton;
	CEditUnit m_edtTSFactor;
	CEditUnit m_edtUDLFactor;
	CComboBox m_cmbSelVihicle;
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
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdEuroBSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nCodeType;
	int m_nTrainType;
	int m_nLaneType;
	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdEuroBSDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdMvhlEuroApply();
	afx_msg void OnSelchangeCmdMvhlTypeCombo();
	afx_msg void OnSelchangeCmdMvhlCodeCmb();
	afx_msg void OnCmdMvhlEuroDynamicChk();
	afx_msg void OnCmdMvhlEuroAutoRdo();

	afx_msg void OnCmdMvhlEuroSelVehicleCmb();
	afx_msg void OnCmdMvhlEuroLongiDistChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMSTDEUROBSDLG_H__DA215B23_B502_4632_8642_FEE4A61D04FA__INCLUDED_)

*/