#if !defined(AFX_CMLLANTRDLG_H__981B4015_0D6A_4632_AFD2_5A2D0ABE7B74__INCLUDED_)
#define AFX_CMLLANTRDLG_H__981B4015_0D6A_4632_AFD2_5A2D0ABE7B74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLlanTrDlg.h : header file
//

#include "CMDlgBase.h"
//#include "..\wg_db\wg_db_DlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_cobxgrup.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

#include "CMGXGridWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CCMLlanTrDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMLlanTrDlg : public CCMDlgBase
{
// Construction
public:
	CCMLlanTrDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Overrides
public:
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_LLANtr_D& data);
	void SetInitModify();
	BOOL ExternalSetting(T_LLAN_K LlanKey);

//----------------------------------------------------------------------
// Implementation
protected:
	void InitGrid();
	void SetGridTitle();
	BOOL Grid_IsExistKey(int nKey);
	void Grid_InsertItem(int nPos, T_LLANtr_BASE &item);
	
	void AlignControl();
	void ResizeDialog();
	void SetHeaderTitle();
	void MakeItemEx();
	void EccentCtrlMan();
	void ButtonCtrl();
	int  FindInsertionPos(CString& strName);
	BOOL InsertItem(T_LLAN_K Key, T_LLANtr_D &Data);
	BOOL DeleteItem(T_LLAN_K Key, T_LLANtr_D &Data);
	BOOL ModifyItem(T_LLAN_K KeyOld, T_LLAN_K Key, T_LLANtr_D &Data);
	CString DataToStr(int i, T_LLAN_K Key, T_LLANtr_D &Data);

	void Data2Dlg();
	BOOL Dlg2Data();
	void InitItemDataVar();
	void InsertBaseItem(int nPos, int nType);
	BOOL GetEQDistance(CString& EQStr, double& Dx, double& Dy, double& Dz);
	void SelectMethodMan();
	
	BOOL IsRelevantCode(int nCode);
	BOOL IsUseImpactSpan();
	
	BOOL SelectElem();

protected:
	BOOL     m_bModify;
	T_LLANtr_D m_Data;
	CArray<UINT, UINT> m_aCtrlList;
	CArray<UINT, UINT> m_aCtrlData;
	CArray<UINT, UINT> m_aCtrlSelect;
	
	BOOL m_bListMode;
	CString m_csOldLineLaneName;
	CString m_csTitle1;
	CString m_csTitle2;
	BOOL    m_bInitDlg;

	CCMGXGridWnd m_wndGrid;

public:
// Dialog Data
	//{{AFX_DATA(CCMLlanTrDlg)
	enum { IDD = IDD_CMD_ML_LLAN_TR };
	CMouseEdit	m_wndPickingEdit;
	CTextUnit	m_2PointUnit2;
	CTextUnit	m_2PointUnit1;
	CMouseEdit	m_wnd2PointsEdit2;
	CMouseEdit	m_wnd2PointsEdit1;
	MEdit	m_wndLaneName;
	CListCtrl	m_List;
	int		m_nSelectionMethod;
	CEditUnit m_edtFactor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLlanTrDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMLlanTrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnCopy();
	afx_msg void OnCmdBtnClose();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdBtnOk();
	afx_msg void OnCmdBtnCancel();
	afx_msg void OnCmdBtnApply();
	afx_msg void OnCmdChangeSelectBy();
	afx_msg void OnCmdOpAdd();
	afx_msg void OnCmdOpInsert();
	afx_msg void OnCmdOpDelete();
	//}}AFX_MSG
	//afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLLANTRDLG_H__981B4015_0D6A_4632_AFD2_5A2D0ABE7B74__INCLUDED_)
