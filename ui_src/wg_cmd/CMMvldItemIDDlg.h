#if !defined(__CMMVLDITEMIDDLG_H__)
#define __CMMVLDITEMIDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemIDDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemIDDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvldItemIDDlg : public CDialogMove
{
// Construction
public:
	CCMMvldItemIDDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVLDid_D &ParamData) 
	{ m_csOldName = ParamData.LoadCaseName; m_Data = ParamData; m_bModify = TRUE; }
	BOOL OnApplySubDlg(BOOL bModify, T_MVLDid_BASE& data, int nPos);
	int  GetLaneNum();
	void AlignControl();
	void CtrlEnableDisable();

//----------------------------------------------------------------------
// Implementation
protected:
	void InitUnit();
	void InitCombo();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL ApplyOrOK();
	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(int nIndex, T_MVLDid_BASE &Data);
	BOOL DeleteItem(int nIndex, T_MVLDid_BASE &Data);
	BOOL ModifyItem(int nIndex, T_MVLDid_BASE &Data);
	CString DataToStr(int i, T_MVLDid_BASE &Data);
	CString GetLaneName(UINT nLaneKey);
	CString GetMvhlName(UINT nMvhlKey);

protected:
	CDBDoc*     m_pDoc;
	T_MVLDid_D  m_Data;
	CString     m_csOldName;
	BOOL        m_bModify;
	BOOL        m_bSurface;
	
	CArray<UINT, UINT> m_aMulti;
	CArray<UINT, UINT> m_aDesignLane;
	CArray<UINT, UINT> m_aSubLoadCase;
	CArray<UINT, UINT> m_aSubLoadCase1;
	CArray<UINT, UINT> m_aSubLoadCase2;
	CArray<UINT, UINT> m_aPermit;
	CArray<UINT, UINT> m_aEtc;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvldItemIDDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_INDIA };
	MEdit	m_wndDesc;
	CListCtrl	m_List;
	CListCtrl m_List2;
	MEdit	m_wndSF4;
	MEdit	m_wndSF3;
	MEdit	m_wndSF2;
	MEdit	m_wndSF1;
	MEdit	m_wndLoadCase;
	CFormulaEditSpin m_wndLaneNumEdit;
	BOOL  m_bAutoLiveLoad;     // 761Ãß°¡
	BOOL  m_bLCPV;
	MComboBox	m_cmbVehicle;
	MComboBox	m_cmbRefLane;
	CEditUnit	m_edtEccen;
	CTextUnit	m_untEccen;
	CEditUnit	m_edtScaleFactor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemIDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//CArray<UINT, UINT> m_aCombOption;

	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemIDDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeCmdLaneNumEdit();
	afx_msg void OnCheckAutoLiveLoad();
	afx_msg void OnCheckPermit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVLDITEMIDDLG_H__)
