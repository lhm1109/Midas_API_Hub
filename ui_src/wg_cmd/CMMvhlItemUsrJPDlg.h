#if !defined(__CMMvhlItemUsrJPDlg_H__)
#define __CMMvhlItemUsrJPDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemUsrJPDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrJPDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMMvhlItemUsrJPDlg : public CDialogMove
{
// Construction
public:
	CCMMvhlItemUsrJPDlg(CWnd* pParent = NULL);   // standard constructor

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
	
	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(int nPos, int nNo, double dLoad, double dDist);
	BOOL DeleteItem(int nPos);
	BOOL ModifyItem(int nPos, int nNo, double dLoad, double dDist);

	void ChangeData();	
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

public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemUsrJPDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMU_JP };

	int       m_nTruckType; // 0=wheel axle load  1:EA load

	CEditUnit m_edtVehicleName;
	CComboBox m_cmbVehicleType;
	CListCtrl m_List;
	CFormulaEditSpin m_ProcessNumEdt;

	CTextUnit m_untH;
	CEditUnit m_edtH;

	CEditUnit m_edtTrkP;
	CEditUnit m_edtTrkD;
	
	CEditUnit m_edtLaneW1;
	CTextUnit m_untLaneW1;
	CEditUnit m_edtLaneD1;
	CTextUnit m_untLaneD1;
	CEditUnit m_edtLaneD2;
	CTextUnit m_untLaneD2;

	CArray<UINT, UINT> m_aNum;
	CArray<UINT, UINT> m_aDD1;
	//CArray<UINT, UINT> m_aDD2;
	CArray<UINT, UINT> m_aDW1;


	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemUsrJPDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemUsrJPDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnCmdMvhlTypeCmb();
	afx_msg void OnCmdMvhlLoadTypeRdo();
	afx_msg void OnCmdMvhlAddBtn();
	afx_msg void OnCmdMvhlInsBtn();
	afx_msg void OnCmdMvhlModBtn();
	afx_msg void OnCmdMvhlDelBtn();
	//afx_msg void OnCmdMvhlFatigueChk();
	afx_msg void OnCmdMvhlApplyBtn();
	afx_msg void OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMvhlItemUsrJPDlg_H__)
