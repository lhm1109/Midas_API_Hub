#if !defined(AFX_CMMVHLITEMUSEREUROBSDLG_H__A94DF37F_0ED5_454E_924E_9149D3052495__INCLUDED_)
#define AFX_CMMVHLITEMUSEREUROBSDLG_H__A94DF37F_0ED5_454E_924E_9149D3052495__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemUserEuroBSDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "CMMvhlItemUserLoadGrid.h"
#include "CMMvhlItemStdLaneGrid.h"
#include "CMMvhlItemUserLoadGrid.h"


/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUserEuroBSDlg dialog
#include "HeaderPre.h"


class __MY_EXT_CLASS__ CCMMvhlItemUserEuroBSDlg : public CDialogMove
{
// Construction
public:
	CCMMvhlItemUserEuroBSDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVHL_D &ParamData);      
//----------------------------------------------------------------------
// Implementation
protected:
	void Data2Dlg(int nSubType);
	BOOL Dlg2Data(int nSubType);
	void Data2DlgLM1();
	BOOL Dlg2DataLM1();
	void Data2DlgLM2();
	BOOL Dlg2DataLM2();
	void Data2DlgLM3();
	BOOL Dlg2DataLM3();
	void Data2DlgFLM3();
	BOOL Dlg2DataFLM3();
	int GetLoadType();
	void SetLoadType(int nSubType);

	
	BOOL ApplyOrOK();
	void AlignControl();

	void SetHeaderTitle();
	virtual void MakeItemEx();

	BOOL InsertItem(int nPos, int nNo, double dLoad, double dDist);
	BOOL DeleteItem(int nPos);
	BOOL ModifyItem(int nPos, int nNo, double dLoad, double dDist);

	BOOL GetLoadAndDistValue(double &dLoad, double &dDist);
	int GetTruckLoadCount();
	int GetTruckDistanceCount();

	void	ChangeBitMap(int which);	
	void  ChangeCtrlText();
	
protected:
	CDBDoc*   m_pDoc;
	T_MVHL_D  m_Data;
	T_MVHL_D  m_Data4[4];
	CString   m_csOldName;
	BOOL      m_bModify;
	CBitmap*  m_pBitmap;
	int m_nSubType;
	CCMMvhlItemStdLaneGrid m_wndLM1Grid;
	CCMMvhlItemUserLoadGrid m_wndLM3Grid[3];
	
public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemUserEuroBSDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMU_EURO_BS };
	CEdit	m_wndVehicleName;
	CEdit   	m_LM1Dedt;	
	CTextUnit m_LM1Dunit;
	CEdit   	m_LM1DAFedt;	

	CEdit	    m_LM2Pedt;
	CEdit	    m_LM2Dedt;
	CListCtrl	m_LM2List;
	CEdit	    m_LM2AFedt;
	CEdit	    m_LM2AFedt2;
	CEdit     m_LM2Wedt;
	CTextUnit m_LM2Wunit;
	CEditUnit m_edtLM2TSFact;
	CEditUnit m_edtLM2UDLFact;

	CButton   m_LM3LCchk[3];
	CSpinButtonCtrl	m_LM3NOPspn[3];
	CEdit	    m_LM3NOPedt[3];
	CButton   m_LM3DAFchk[3];
	CEdit     m_LM3DAFedt[3];
	
	CEdit     m_FLM3Intervaledt;
	CButton   m_FLM3DAFchk[2];
	CEdit     m_FLM3DAFedt[2];
 	CButton	m_wndBtnApply;

	CEditUnit m_edtTSFact;
	CEditUnit m_edtUDLFact;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemUserEuroBSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strText;
	CArray<UINT,UINT> m_aCtrlCom1;
	CArray<UINT,UINT> m_aCtrlCom2;
	CArray<UINT,UINT> m_aCtrlCom3;
	CArray<UINT,UINT> m_aCtrlCom4;
	CArray<UINT,UINT> m_aCtrlCom5;
	CArray<UINT,UINT> m_aCtrlCom3_1;
	CArray<UINT,UINT> m_aCtrlCom3_2;
	CArray<UINT,UINT> m_aCtrlCom3_3;


	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemUserEuroBSDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnInsert();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdMlMvhlItemTypeRdo();
	afx_msg void OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult);
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
	
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMUSEREUROBSDLG_H__A94DF37F_0ED5_454E_924E_9149D3052495__INCLUDED_)
