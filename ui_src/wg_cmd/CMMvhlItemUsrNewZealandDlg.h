#if !defined(__CMMVHLITEMUSRNEWZEALANDDLG_H__)
#define __CMMVHLITEMUSRNEWZEALANDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemUsrAustraliaDlg.h : header file
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
// CMMvhlItemUsrNewZealandDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CMMvhlItemUsrNewZealandDlg : public CDialogMove
{
	// Construction
public:
	CMMvhlItemUsrNewZealandDlg(CWnd* pParent = NULL);   // standard constructor

	//----------------------------------------------------------------------
	// Interface
public:
	void SetParamData(T_MVHL_D& ParamData) { m_csOldName = ParamData.VehicleLoadName; m_Data = ParamData; m_bModify = TRUE; }

	//----------------------------------------------------------------------
	// Implementation
protected:

	void InitUnit();
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
	BOOL GetLoadAndDistValue(double& dLoad, double& dDist);

	BOOL ApplyOrOK();

protected:
	CDBDoc* m_pDoc;
	T_MVHL_D  m_Data;

	CString   m_csOldName;
	BOOL      m_bModify;
	MillustViewer m_wndPicture;

	int m_bPermitLoadCall;

public:
	// Dialog Data
		//{{AFX_DATA(CMMvhlItemUsrNewZealandDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMU_NEWZEALAND };

	int       m_nLoadType; //0=truck  1:Permit

	CEditUnit m_edtVehicleName;
	CEditUnit	m_edtTrkP;
	CEditUnit	m_edtTrkD;
	CListCtrl	m_List;

	CEditUnit m_edtLaneW;
	CTextUnit m_untLaneW;

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMMvhlItemUsrNewZealandDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMMvhlItemUsrNewZealandDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdMvhlLoadTypeRdo();
	afx_msg void OnCmdMvhlAddBtn();
	afx_msg void OnCmdMvhlInsBtn();
	afx_msg void OnCmdMvhlModBtn();
	afx_msg void OnCmdMvhlDelBtn();
	afx_msg void OnCmdMvhlApplyBtn();
	afx_msg void OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVHLITEMUSRAUSTRALIADLG_H__)
