#if !defined(__CMLlanJPDlg_H__)
#define __CMLlanJPDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLlanJPDlg.h : header file
//

#include "CMDlgBase.h"
//#include "..\wg_db\wg_db_DlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "CMGXGridWnd.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMLlanJPDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMLlanJPDlg : public CCMDlgBase
{
// Construction
public:
	CCMLlanJPDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Overrides
public:
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void UpdateUnit();

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_LLANjp_D& data);
	void SetInitModify();
	BOOL ExternalSetting(T_LLANjp_K LlanKey);

//----------------------------------------------------------------------
// Implementation
protected:
	void AlignControl();
	void ResizeDialog();
	void SetHeaderTitle();
	void MakeItemEx();
	void ButtonCtrl();
	int  FindInsertionPos(CString& strName);
	BOOL InsertItem(T_LLANjp_K Key, T_LLANjp_D& Data);
	BOOL DeleteItem(T_LLANjp_K Key, T_LLANjp_D& Data);
	BOOL ModifyItem(T_LLANjp_K KeyOld, T_LLANjp_K Key, T_LLANjp_D &Data);
	CString DataToStr(int i, T_LLANjp_K Key, T_LLANjp_D &Data);

	void Data2Dlg();
	BOOL Dlg2Data();
	void InitItemDataVar();
	void InitCombo();
	void InitGrid();
	void SetGridTitle();
	BOOL Grid_IsExistKey(int nKey);
	void Grid_InsertItem(int nPos, BOOL bFirst, T_LLANjp_BASE &item);
	void InsertBaseItem(int nPos, int nType);
	BOOL GetEQDistance(CString& EQStr, double& Dx, double& Dy, double& Dz);
	void SelectMethodMan();
	void SelectElem();				// added by KYM

	BOOL IsUseCentrifugalForce();

protected:
	BOOL     m_bModify;
	T_LLANjp_D m_Data;
	CArray<UINT, UINT> m_aCtrlList;
	CArray<UINT, UINT> m_aCtrlData;
	CArray<UINT, UINT> m_aCtrlSelect;

	BOOL m_bListMode;
	CCMGXGridWnd m_wndGrid;
	CString m_csOldLineLaneName;
	CString m_csTitle1;
	CString m_csTitle2;

public:
// Dialog Data
	//{{AFX_DATA(CCMLlanJPDlg)
	enum { IDD = IDD_CMD_ML_LLAN_JAPAN };
	MEdit	    m_wndLaneName;
	CListCtrl	m_List;

	CEditUnit	m_edtWidth;
	CTextUnit	m_untWidth;
	CEditUnit	m_wndEccenEdit;
	CTextUnit	m_wndEccenUnit;
	CEditUnit	m_wndIFEdit;

	CEditUnit	m_wndCF_s;
	MEdit	    m_untCF_s;
	CEditUnit	m_wndCF_r;
	CEdit       m_untCF_r;

	CTextUnit	m_2PointUnit1;
	CTextUnit	m_2PointUnit2;

	MComboBox 	m_cmbCrossBeamElemGroup;
	MEdit	    m_wndElemSelectEdit;
	CMouseEdit	m_wnd2PointsEdit1;
	CMouseEdit	m_wnd2PointsEdit2;
	CMouseEdit	m_wndPickingEdit;

	BOOL    m_bCentForce;
	BOOL    m_bCentIF;
	int		m_nLoadDistType;
	int		m_nMovDir;
	int		m_nSelectionMethod;

	CFormulaEditSpin  m_edtSkewStart;
	CFormulaEditSpin  m_edtSkewEnd;
	CFoldGroupBox_Cross	m_chkBmp;

	MillustViewer m_ImgViewer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLlanJPDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMLlanJPDlg)
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
	afx_msg void OnCmdBtnCopy();
	afx_msg void OnCmdCentForceChk();

	//}}AFX_MSG
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMLlanJPDlg_H__)
