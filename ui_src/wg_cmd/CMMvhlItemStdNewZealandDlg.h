#if !defined(__CMMVHLITEMSTDNEWZEALANDDLG_H__)
#define __CMMVHLITEMSTDNEWZEALANDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdNewZealandDlg.h : header file
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
// CCMMvhlItemStdNewZealandDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhlItemStdNewZealandDlg : public CDialogMove
{
	// Construction
public:
	CCMMvhlItemStdNewZealandDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMMvhlItemStdNewZealandDlg();

	//----------------------------------------------------------------------
	// Implementation
public:
	void SetParamData(T_MVHL_D& ParamData)
	{
		m_csOldName = ParamData.VehicleLoadName; m_Data = ParamData; m_bModify = TRUE;
	}

public:

	void InitUnit();
	void InitCombo();
	void AlignControl();
	void ControlsShowHide();

	void SetHeaderTitle();
	BOOL InsertItem(int nPos, int nNo, double dLoad, double dDist);
	void MakeItemEx();
	int  GetLoadCount();
	int  GetDistCount();

	void ChangeBitmap(int nIndex, int nBitmap);
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL ApplyOrOK();
	BOOL ChangeData();

	void SetLoadTypeList(int nIndex);
	void  GetIndex(CString& csName, int* sitem = NULL);

protected:

	CDBDoc* m_pDoc;
	T_MVHL_D  m_Data;
	int       m_nStndCode;

	CString   m_csOldName;
	BOOL      m_bModify;
	MillustViewer	m_wndPicture;

	CArray<UINT, UINT> m_aPicture;     // Picture
	CArray<UINT, UINT> m_aList;        // List
	CArray<UINT, UINT> m_aW;           // W
	CArray<UINT, UINT> m_aEtc;         // OK, Cancel, Apply
	CArray<UINT, UINT> m_aCtrlPatch;   // patch control

public:
	// Dialog Data
		//{{AFX_DATA(CCMMvhlItemStdNewZealandDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_NEWZEALAND };

	MComboBox	m_cmbCodeName;
	CEditUnit m_edtVehicleName;
	MComboBox	m_cmbVehicleType;

	CEditUnit m_edtW;
	CTextUnit m_untW;

	MButton	m_chkPatch;
	CEditUnit m_edtPatchW;
	CTextUnit m_untPatchW;
	CEditUnit m_edtPatchL;
	CTextUnit m_untPatchL;

	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdNewZealandDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdNewZealandDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdMvhlCodeCmb();
	afx_msg void OnCmdMvhlTypeCmb();
	afx_msg void OnCmdApply();
	afx_msg void OnPatchBtn();
	afx_msg void OnChkPatch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMvhlItemStdNewZealandDlg_H__)
