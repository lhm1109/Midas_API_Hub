#if !defined(__CMSLANCHDLG_H__)
#define __CMSLANCHDLG_H__

#include "CMDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\wg_base\wg_base_MouseEdit.h"
#include "CMGXGridWnd.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MillustViewer.h"
#include "..\wg_db\FormulaEditUnitSpin.h"

using namespace mit::frx;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSlanCHDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMSlanCHDlg dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMSlanCHDlg : public CCMDlgBase
{
// Construction
public:
	CCMSlanCHDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_SLANch_D& data);
	void SetInitModify();

	BOOL ExternalSetting(T_SLANch_K SlanKey);

// Dialog Data
	//{{AFX_DATA(CCMSlanCHDlg)
	enum { IDD = IDD_CMD_ML_SLAN_CHINA };
	MButton   m_chkLaneOptim;
	CTextUnit m_untWheelSpace;
	CEditUnit m_edtWheelSpace;
	CTextUnit	m_unitSpace;
	CEditUnit	m_editSpace;
	CTextUnit	m_2PointUnit2;
	CTextUnit	m_2PointUnit1;
	CFormulaEditUnitSpin  m_edtSkewStart;
	CFormulaEditUnitSpin  m_edtSkewEnd;
	MEdit	m_wndNodeSelectEdit;
	MEdit	m_wndFactor;
	CMouseEdit	m_wndPickingEdit;
	CMouseEdit	m_wnd2PointsEdit2;
	CMouseEdit	m_wnd2PointsEdit1;
	CMouseEdit	m_wndOffsetEdit;
	CTextUnit	m_wndOffsetUnit;
	CMouseEdit	m_wndLWidthEdit;
	CTextUnit	m_wndLWidthUnit;
	CListCtrl	m_List;
	MEdit	m_wndLaneName;
	int		m_nMovDir;
	int		m_nSelectionMethod;
	CEditUnit m_edtAllowWidth;
	CTextUnit m_untAllowWidth;
	CFoldGroupBox_Cross	m_chkBmp;

	MillustViewer m_ImgViewer;
	//}}AFX_DATA


// Overrides
public:
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void UpdateUnit();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSlanCHDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AlignControl();
	void AlignControl_Fold();
	void ResizeDialog();
	void SetHeaderTitle();
	void InitGrid();
	void SetGridTitle();
	void MakeItemEx();
	void InitItemDataVar();
	void Data2Dlg();
	BOOL Dlg2Data();
	void InsertBaseItem(int nPos, int nType);
	void Grid_InsertItem(int nPos, T_SLANch_BASE &item);
	BOOL Grid_IsExistKey(int nKey);
	int  FindInsertionPos(CString& strName);
	void SelectNode();
	void SelectMethodMan();
	void ButtonCtrl();
	//BOOL IsRelevantCode(int nCode);
	void ShowHideByCode();
	
	BOOL InsertItem(T_SLANch_K Key, T_SLANch_D &Data);
	BOOL DeleteItem(T_SLANch_K Key, T_SLANch_D &Data);

	BOOL     m_bModify;

	CString DataToStr(int i, T_SLANch_K Key, T_SLANch_D &Data);

	CCMGXGridWnd m_wndGrid;
	CString m_csOldLaneName;

	T_SLANch_D m_Data;
	CArray<UINT, UINT> m_aCtrlList;
	CArray<UINT, UINT> m_aCtrlData;
	CArray<UINT, UINT> m_aCtrlSelect;
	BOOL m_bListMode;

	// Generated message map functions
	//{{AFX_MSG(CCMSlanCHDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnClose();
	afx_msg void OnCmdBtnCancel();
	afx_msg void OnPaint();
	afx_msg void OnCmdChangeSelectBy();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdOpAdd();
	afx_msg void OnCmdOpInsert();
	afx_msg void OnCmdOpDelete();
	afx_msg void OnCmdBtnOk();
	afx_msg void OnCmdBtnApply();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdBtnCopy();
	afx_msg void OnCmdChangeOptimChk();
	//}}AFX_MSG
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSLANDLG_H__)
