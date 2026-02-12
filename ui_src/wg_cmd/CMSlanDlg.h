#if !defined(__CMSLANDLG_H__)
#define __CMSLANDLG_H__

#include "CMDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MillustViewer.h"
#include "..\wg_db\FormulaEditUnitSpin.h"

using namespace mit::frx;

#include "CMGXGridWnd.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSlanDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMSlanDlg dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMSlanDlg : public CCMDlgBase
{
// Construction
public:
	CCMSlanDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_SLAN_D& data);
	void SetInitModify();

	BOOL ExternalSetting(T_SLAN_K SlanKey);

// Dialog Data
	//{{AFX_DATA(CCMSlanDlg)
	enum { IDD = IDD_CMD_ML_SLAN };
	MButton   m_chkLaneOptim;
	CTextUnit m_untWheelSpace;
	CEditUnit m_edtWheelSpace;
	CTextUnit	m_2PointUnit2;
	CTextUnit	m_2PointUnit1;
	CFormulaEditUnitSpin  m_edtSkewStart;
	CFormulaEditUnitSpin  m_edtSkewEnd;
	MEdit	m_wndNodeSelectEdit;
	MEdit	m_wndFactor;
	MEdit	m_wndCFEdit;
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
	int   m_nImpactorSpan;
	CEditUnit m_wndImpactFactorEdt;
	CEditUnit m_wndSpanLengthEdt;
	CTextUnit m_wndSpanLengthUnt;
	CEditUnit m_edtEccenVertLoad;
	CTextUnit m_untEccenVertLoad;
	CEditUnit m_edtAllowWidth;
	CTextUnit m_untAllowWidth;

	MillustViewer m_ImgViewer;
	//}}AFX_DATA


// Overrides
public:
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void UpdateUnit();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSlanDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AlignControl();
	void ResizeDialog();
	void SetHeaderTitle();
	void InitGrid();
	void SetGridTitle();
	void MakeItemEx();
	void InitItemDataVar();
	void Data2Dlg();
	BOOL Dlg2Data();
	void InsertBaseItem(int nPos, int nType);
	void Grid_InsertItem(int nPos, T_SLAN_BASE &item);
	BOOL Grid_IsExistKey(int nKey);
	int  FindInsertionPos(CString& strName);
	void SelectNode();
	void SelectMethodMan();
	void ShowHideByCode();
	BOOL IsRelevantCode(int nCode);
	BOOL IsUseImpactSpan();
	BOOL IsUseSpanStart();
	BOOL IsUseEccenVertLoad();
	BOOL IsUseCentrifugalForce();
	BOOL IsUseOptim(int nCode);
	BOOL IsUseLaneTypeRdo();

	void ButtonCtrl();

	BOOL InsertItem(T_SLAN_K Key, T_SLAN_D &Data);
	BOOL DeleteItem(T_SLAN_K Key, T_SLAN_D &Data);

	BOOL     m_bModify;

	CString DataToStr(int i, T_SLAN_K Key, T_SLAN_D &Data);

	CCMGXGridWnd m_wndGrid;
	CString m_csOldLaneName;

	T_SLAN_D m_Data;
	CArray<UINT, UINT> m_aCtrlList;
	CArray<UINT, UINT> m_aCtrlData;
	CArray<UINT, UINT> m_aCtrlSelect;
	CArray<UINT, UINT> m_aCtrlCF;

	CArray<UINT, UINT> m_aLaneWidth;
	CArray<UINT, UINT> m_aWheelSpacing; 
	CArray<UINT, UINT> m_aOffset; 
	CArray<UINT, UINT> m_aImpactFactor;
	CArray<UINT, UINT> m_aImpactorSpan; // INDIA ¾²´Â Impact Factor, Span Length
	
	CArray<UINT, UINT> m_aCtrlLaneType;
	CArray<UINT, UINT> m_aCtrlLaneTypeRdo;

	BOOL m_bListMode;
	BOOL m_bCodeNameINDIA;
	CString m_csTitle1;
	CString m_csTitle2;

	// Generated message map functions
	//{{AFX_MSG(CCMSlanDlg)
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
	afx_msg void OnCmdImpactorSpanRdo();
	afx_msg void OnCmdChangeOptimChk();
	afx_msg void OnSelLaneTypeRdo();

	//}}AFX_MSG
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSLANDLG_H__)
