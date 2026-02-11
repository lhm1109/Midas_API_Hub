#if !defined(__CMMVHLITEMUSRINDIADLG_H__)
#define __CMMVHLITEMUSRINDIADLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemUsrIndiaDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrIndiaDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhlItemUsrIndiaDlg : public CDialogMove
{
// Construction
public:
	CCMMvhlItemUsrIndiaDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMMvhlItemUsrIndiaDlg();

//----------------------------------------------------------------------
// Implementation
public:
	void SetParamData(T_MVHL_D &ParamData) 
	{ m_csOldName = ParamData.VehicleLoadName; m_Data = ParamData; m_bModify = TRUE; }

public:
 	void InitControl();
 	void InitUnit();
 	void InitCombo();

	void AlignControl();
 	void ControlsShowHide();
	void EnableDisableControls();

 	void ChangeBitmap();

 	void SetHeaderTitle();
 	void MakeItemEx();
 	BOOL InsertItem(int nListType, int nPos, int nNo, double dLoad, double dDist);
	BOOL DeleteItem(int nListType, int nPos);
	BOOL ModifyItem(int nListType, int nPos, int nNo, double dLoad, double dDist);

 	int  GetLoadCount(int nListType);
 	int  GetDistCount(int nListType);
	BOOL GetLoadAndDistValue(int nListType, double &dLoad, double &dDist);
 
	void Data2Dlg();
	BOOL Dlg2Data();  

 	BOOL ApplyOrOK();
	
protected:
	
	CDBDoc*   m_pDoc;
	T_MVHL_D  m_Data;

	CString   m_csOldName;
	BOOL      m_bModify;
	MillustViewer	m_wndPicture;

	int m_bPermitLoadCall;

	CArray<UINT, UINT> m_aIRCLoadType;
	CArray<UINT, UINT> m_aIRSLoadType;
	CArray<UINT, UINT> m_aWheel;
	CArray<UINT, UINT> m_aTrack;
	CArray<UINT, UINT> m_aBogie;
	CArray<UINT, UINT> m_aNoseTail;
	CArray<UINT, UINT> m_aNoseTail70R;
	CArray<UINT, UINT> m_aGondola;
	CArray<UINT, UINT> m_aTrain;
	CArray<UINT, UINT> m_aLongiForce;
	CArray<UINT, UINT> m_aBrakeLOCORatio;
	CArray<UINT, UINT> m_aBrakeTrainRatio;
	CArray<UINT, UINT> m_aBrakeLOCO;
	CArray<UINT, UINT> m_aIRC;
	CArray<UINT, UINT> m_aIRS; 
 	CArray<UINT, UINT> m_aOKCancel;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemUsrIndiaDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMU_INDIA };

	MComboBox	m_cmbCodeName;
	int       m_nIRCLoadType;
	CEditUnit m_edtVehicleName;
	CEditUnit m_edtIRC_P;
	CEditUnit m_edtIRC_D;
	CListCtrl	m_lstIRC;
	CEditUnit m_edtIRCdD1;
	CTextUnit m_untIRCdD1;
	CEditUnit m_edtIRCdD2;
	CTextUnit m_untIRCdD2;
	CEditUnit m_edtIRCP;
	CTextUnit m_untIRCP;
	CEditUnit m_edtIRCD;
	CTextUnit m_untIRCD;
	CEditUnit m_edtIRCPb;
	CTextUnit m_untIRCPb;
	CEditUnit m_edtIRCDb;
	CTextUnit m_untIRCDb;
	CEditUnit m_edtNoseTailDist;
	CTextUnit m_untNoseTailDist;
	int       m_nIRSLoadType;
	CEditUnit m_edtIRS1_P;
	CEditUnit m_edtIRS1_D;
	CListCtrl	m_lstIRS1;
	CEditUnit m_edtIRSdD12;
	CTextUnit m_untIRSdD12;
	CEditUnit m_edtIRSdD22;
	CTextUnit m_untIRSdD22;
	int       m_nTrainSubType;
	int       m_nGondolaPos;
	CEditUnit m_edtIRS2_P;
	CEditUnit m_edtIRS2_D;
	CListCtrl	m_lstIRS2;
	CEditUnit m_edtIRSdD3;
	CTextUnit m_untIRSdD3;
	int       m_nTrainPos;
	CEditUnit m_edtIRSdW;
	CTextUnit m_untIRSdW;
	CEditUnit m_edtTractive;
	CTextUnit m_untTractive;
	CEditUnit m_edtBrakeLOCORatio;
	CEditUnit m_edtBrakeTrainRatio;
	CEditUnit m_edtBrakeLOCO;
	CTextUnit m_untBrakeLOCO;
	CEditUnit m_edtNoseTailDistW;
	CTextUnit m_untNoseTailDistW;
	CEditUnit m_edtNoseTailDistT;
	CTextUnit m_untNoseTailDistT;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemUsrIndiaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemUsrIndiaDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
 	afx_msg void OnCmdMvhlCodeCmb();
	afx_msg void OnCmdMvhlIRCLoadTypeRdo();
	afx_msg void OnCmdMvhlIRCAddBtn();
	afx_msg void OnCmdMvhlIRCInsBtn();
	afx_msg void OnCmdMvhlIRCModBtn();
	afx_msg void OnCmdMvhlIRCDelBtn();
	afx_msg void OnCmdMvhlIRCItemChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdMvhlIRSLoadTypeRdo();
	afx_msg void OnCmdMvhlIRS1AddBtn();
	afx_msg void OnCmdMvhlIRS1InsBtn();
	afx_msg void OnCmdMvhlIRS1ModBtn();
	afx_msg void OnCmdMvhlIRS1DelBtn();
	afx_msg void OnCmdMvhlIRS1ItemChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdMvhlIRSTrainSubTypeRdo();
	afx_msg void OnCmdMvhlIRS2AddBtn();
	afx_msg void OnCmdMvhlIRS2InsBtn();
	afx_msg void OnCmdMvhlIRS2ModBtn();
	afx_msg void OnCmdMvhlIRS2DelBtn();
	afx_msg void OnCmdMvhlIRS2ItemChange(NMHDR* pNMHDR, LRESULT* pResult);
 	afx_msg void OnCmdMvhlApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVHLITEMUSRINDIADLG_H__)
