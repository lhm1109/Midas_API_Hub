#if !defined(__CMLLANOPDLG_H__)
#define __CMLLANOPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLlanOpDlg.h : header file
//

#include "CMDlgBase.h"
#include "CMGXGridWnd.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_db\wg_db_cobxgrup.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMLlanOpDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMLlanOpDlg : public CCMDlgBase
{
// Construction
public:
	CCMLlanOpDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Overrides
public:
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void UpdateUnit();

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_LLANop_D& data);
	void SetInitModify();
	BOOL ExternalSetting(T_LLANop_K LlanKey);

//----------------------------------------------------------------------
// Implementation
protected:
	void AlignControl();
	void ResizeDialog();
	void ButtonCtrl();
	void SetHeaderTitle();
	void MakeItemEx();
	int  FindInsertionPos(CString& strName);
	BOOL InsertItem(T_LLANop_K Key, T_LLANop_D &Data);
	BOOL DeleteItem(T_LLANop_K Key, T_LLANop_D &Data);
	BOOL ModifyItem(T_LLANop_K KeyOld, T_LLANop_K Key, T_LLANop_D &Data);
	CString DataToStr(int i, T_LLANop_K Key, T_LLANop_D &Data);

	void Data2Dlg();
	BOOL Dlg2Data();
	void InitItemDataVar();
	void InitCombo();
	void InitGrid();
	void SetGridTitle();
	BOOL Grid_IsExistKey(int nKey);
	void Grid_InsertItem(int nPos, T_LLANop_BASE &item);
	void InsertBaseItem(int nPos, int nType);
	BOOL GetEQDistance(CString& EQStr, double& Dx, double& Dy, double& Dz);
	void SelectMethodMan();
	void SelectElem();				// added by KYM
	void ShowHideByCode();
	BOOL IsUseImpactSpan();
	BOOL IsUseSpanStart();
	BOOL IsUseSpanLength();
	BOOL IsUseScaleFactor();
	BOOL IsUseCentrifugalForce();

protected:
	T_LLANop_D m_Data;

	BOOL m_bModify;
	BOOL m_bListMode;
	CString m_csOldLineLaneName;
	CString m_csTitle1;
	CString m_csTitle2;

	CArray<UINT, UINT> m_aCtrlList;
	CArray<UINT, UINT> m_aCtrlData;
	CArray<UINT, UINT> m_aCtrlSelect;
	CArray<UINT, UINT> m_aCtrlCF;
	
	CCMGXGridWnd m_wndGrid;

public:
// Dialog Data
	//{{AFX_DATA(CCMLlanOpDlg)
	enum { IDD = IDD_CMD_ML_LLAN_OPTIM };

	MEdit	m_wndLaneName;
	CEditUnit	m_edtOptimWidth;
	CTextUnit	m_untOptimWidth;
	CEditUnit	m_edtWidth;
	CTextUnit	m_untWidth;
	CEditUnit	m_edtAnalOffset;
	CTextUnit	m_untAnalOffset;
	CEditUnit	m_edtEqualLaneDivide;
	CEditUnit m_edtWheelSpace;
	CTextUnit m_untWheelSpace;
	CEditUnit m_edtMargin;
	CTextUnit m_untMargin;
	CMouseEdit	m_wndEccenEdit;
	CTextUnit	m_wndEccenUnit;
	MEdit	m_wndFactor;
	MEdit	m_wndCFEdit;
	CEditUnit m_edtSpanLength;
	CTextUnit m_untSpanLength;
	CEditUnit m_edtScaleFactor;
	BOOL m_bStadd;
	int		m_nLoadDistType;
	int		m_nOffsetType;
	MComboBox	m_cmbCrossBeamElemGroup;
	CFormulaEditSpin  m_edtSkewStart;
	CFormulaEditSpin  m_edtSkewEnd;
	int		m_nMovDir;
	int		m_nSelectionMethod;
	CMouseEdit	m_wnd2PointsEdit1;
	CTextUnit	m_2PointUnit1;
	CMouseEdit	m_wnd2PointsEdit2;
	CTextUnit	m_2PointUnit2;
	MEdit	m_wndElemSelectEdit;
	CMouseEdit	m_wndPickingEdit;
	CListCtrl	m_List;
	CFoldGroupBox_Cross	m_chkBmp;	

	MillustViewer m_ImgViewer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLlanOpDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMLlanOpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnClose();
	afx_msg void OnCmdBtnOk();
	afx_msg void OnCmdBtnCancel();
	afx_msg void OnCmdBtnApply();
	afx_msg void OnCmdChangeSelectBy();
	afx_msg void OnCmdOpAdd();
	afx_msg void OnCmdOpInsert();
	afx_msg void OnCmdOpDelete();
	afx_msg void OnPaint();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdLoadDistType();
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

#endif // !defined(__CMLLANOPDLG_H__)
