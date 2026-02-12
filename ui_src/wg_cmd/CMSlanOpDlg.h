#if !defined(__CMSLANOPDLG_H__)
#define __CMSLANOPDLG_H__

#include "CMDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "CMGXGridWnd.h"

#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MillustViewer.h"
#include "..\wg_db\FormulaEditUnitSpin.h"

using namespace mit::frx;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSlanOpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMSlanOpDlg dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMSlanOpDlg : public CCMDlgBase
{
// Construction
public:
	CCMSlanOpDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_SLANop_D& data);
	void SetInitModify();

	BOOL ExternalSetting(T_SLANop_K SlanKey);

// Dialog Data
	//{{AFX_DATA(CCMSlanOpDlg)
	enum { IDD = IDD_CMD_ML_SLAN_OPTIM };

	MEdit	m_wndLaneName;
	CEditUnit	m_edtOptimWidth;
	CTextUnit	m_untOptimWidth;
	CMouseEdit	m_wndLWidthEdit;
	CTextUnit	m_wndLWidthUnit;
	CEditUnit	m_edtAnalOffset;
	CTextUnit	m_untAnalOffset;
	CEditUnit	m_edtEqualLaneDivide;
	CEditUnit m_edtWheelSpace;
	CTextUnit m_untWheelSpace;
	CEditUnit m_edtMargin;
	CTextUnit m_untMargin;
	CMouseEdit	m_wndOffsetEdit;
	CTextUnit	m_wndOffsetUnit;
	MEdit	m_wndFactor;
	MEdit	m_wndCFEdit;
	CEditUnit m_edtSpanLength;
	CTextUnit m_untSpanLength;
	BOOL m_bStadd;
	CFormulaEditUnitSpin  m_edtSkewStart;
	CFormulaEditUnitSpin  m_edtSkewEnd;
	int		m_nMovDir;
	int		m_nSelectionMethod;
	int		m_nOffsetType;
	CMouseEdit	m_wnd2PointsEdit1;
	CTextUnit	m_2PointUnit1;
	CMouseEdit	m_wnd2PointsEdit2;
	CTextUnit	m_2PointUnit2;  
	CMouseEdit	m_wndPickingEdit;	
	MEdit	m_wndNodeSelectEdit;	
	CListCtrl	m_List;	
	CFoldGroupBox_Cross	m_chkBmp;	

	MillustViewer m_ImgViewer;
	//}}AFX_DATA


// Overrides
public:
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void UpdateUnit();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSlanOpDlg)
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
	void Grid_InsertItem(int nPos, T_SLANop_BASE &item);
	BOOL Grid_IsExistKey(int nKey);
	int  FindInsertionPos(CString& strName);
	void SelectNode();
	void SelectMethodMan();
	void ShowHideByCode();
	BOOL IsRelevantCode(int nCode);
	BOOL IsUseImpactSpan();
	BOOL IsUseSpanStart();
	BOOL IsUseSpanLength();
	BOOL IsUseCentrifugalForce();
	void ButtonCtrl();

	BOOL InsertItem(T_SLANop_K Key, T_SLANop_D &Data);
	BOOL DeleteItem(T_SLANop_K Key, T_SLANop_D &Data);

	BOOL     m_bModify;

	CString DataToStr(int i, T_SLANop_K Key, T_SLANop_D &Data);

	CCMGXGridWnd m_wndGrid;
	CString m_csOldLaneName;

	T_SLANop_D m_Data;
	CArray<UINT, UINT> m_aCtrlList;
	CArray<UINT, UINT> m_aCtrlData;
	CArray<UINT, UINT> m_aCtrlSelect;
	CArray<UINT, UINT> m_aCtrlCF;

	CArray<UINT, UINT> m_aLaneWidth;
	CArray<UINT, UINT> m_aWheelSpacing; 
	CArray<UINT, UINT> m_aOffset; 
	
	BOOL m_bListMode;
	CString m_csTitle1;
	CString m_csTitle2;

	// Generated message map functions
	//{{AFX_MSG(CCMSlanOpDlg)
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
	afx_msg void OnAnalOffsetType();
	afx_msg void OnCmdBtnCopy();
	//}}AFX_MSG
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSLANOPDLG_H__)
