#if !defined(__CMSLANFRDLG_H__)
#define __CMSLANFRDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLlanFRDlg.h : header file
//

#include "CMDlgBase.h"
//#include "..\wg_db\wg_db_DlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_cobxgrup.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "CMGXGridWnd.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMSlanFRDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMSlanFRDlg : public CCMDlgBase
{
// Construction
public:
	CCMSlanFRDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Overrides
public:
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void UpdateUnit();

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_SLANfr_D& data);
	void SetInitModify();
	BOOL ExternalSetting(T_SLANfr_K LlanKey);

//----------------------------------------------------------------------
// Implementation
protected:
	void AlignControl();
	void ResizeDialog();
	void SetHeaderTitle();
	void MakeItemEx();
	void ButtonCtrl();
	int  FindInsertionPos(CString& strName);
	BOOL InsertItem(T_SLANfr_K Key, T_SLANfr_D &Data);
	BOOL DeleteItem(T_SLANfr_K Key, T_SLANfr_D &Data);
	BOOL ModifyItem(T_SLANfr_K KeyOld, T_SLANfr_K Key, T_SLANfr_D &Data);
	CString DataToStr(int i, T_SLANfr_K Key, T_SLANfr_D &Data);

	void Data2Dlg();
	BOOL Dlg2Data();
	void InitItemDataVar();
	void InitGrid();
	void SetGridTitle();
	BOOL Grid_IsExistKey(int nKey);
	void Grid_InsertItem(int nPos, BOOL bFirst, T_SLANfr_BASE &item);
	void InsertBaseItem(int nPos, int nType);
	BOOL GetEQDistance(CString& EQStr, double& Dx, double& Dy, double& Dz);
	void SelectMethodMan();
	void SelectNode();				// added by KYM
	void ShowHideByCode();
	void ShowEnableDisable();
	//void ShowHideByCode_LaneWidth();
	//BOOL IsUseImpactSpan();

	BOOL IsSystemLoadType();
	BOOL IsUseCentrifugalForce();
	BOOL IsUseFactor();

protected:
	BOOL     m_bModify;
	T_SLANfr_D m_Data;
	CArray<UINT, UINT> m_aCtrlList;
	CArray<UINT, UINT> m_aCtrlData;
	CArray<UINT, UINT> m_aCtrlSelect;

	BOOL m_bListMode;
	CCMGXGridWnd m_wndGrid;
	CString m_csOldLaneName;
	CString m_csTitle1;
	CString m_csTitle2;

public:
// Dialog Data
	//{{AFX_DATA(CCMSlanFRDlg)
	enum { IDD = IDD_CMD_ML_SLAN_FRANCE };
	MEdit	    m_wndLaneName;
	CListCtrl	m_List;

	CEditUnit	m_edtWidth;
	CTextUnit	m_untWidth;
	CEditUnit	m_edtLaneNum;
	CMouseEdit	m_wndOffsetEdit;
	CTextUnit	m_wndOffsetUnit;
	CEditUnit	m_wndFactor;
	CEditUnit	m_wndFactorL;
	CTextUnit	m_untFactorL;
	CEditUnit	m_wndFactorG;
	CTextUnit	m_untFactorG;
	CEditUnit	m_wndCFEdit;

	CTextUnit	m_2PointUnit1;
	CTextUnit	m_2PointUnit2;

	MEdit	    m_wndNodeSelectEdit;
	CMouseEdit	m_wnd2PointsEdit1;
	CMouseEdit	m_wnd2PointsEdit2;
	CMouseEdit	m_wndPickingEdit;

	BOOL    m_bUseFactor;
	BOOL    m_bCentForce;
	int		m_nLaneType;
	int		m_nMovDir;
	int		m_nSelectionMethod;

	CFormulaEditSpin  m_edtSkewStart;
	CFormulaEditSpin  m_edtSkewEnd;
	CFoldGroupBox_Cross	m_chkBmp;

	MillustViewer m_ImgViewer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSlanFRDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSlanFRDlg)
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
	
	afx_msg void OnCmdLaneType();
	afx_msg void OnCmdBtnCopy();
	afx_msg void OnCmdCentForceChk();
	afx_msg void OnCmdUseFactorChk();

	//}}AFX_MSG
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSLANFRDLG_H__)
