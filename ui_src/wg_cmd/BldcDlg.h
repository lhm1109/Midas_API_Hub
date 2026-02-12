#if !defined(__BLDCDLG_H__)
#define __BLDCDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BldcDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CBldcDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CBldcDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CBldcDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBldcDlg)
	enum { IDD = IDD_ETC_BLDC_DLG };
	CSelectLC	m_wndXStifCombo;
	CSelectLC	m_wndYStifCombo;
	CSelectLC	m_wndYLcCombo;
	CSelectLC	m_wndXLcCombo;
	MButton	m_wndShearFlag;
	MButton	m_chkWindSeisOnFlexible;    // Consider Wind and Seismic Loads for Flexible Floors
	MEdit	m_wndScaleFactor;
	CListCtrl	m_List;
	CSelectLC	m_wndLcCombo;
	MButton	m_wndUseFlag;
	MButton	m_wndUseBedRockFlag;
	MButton	m_wndMassFlag;
	CTextUnit	m_wndBLevelUnit;
	CTextUnit	m_wndBLevelUnit2;
	CEditUnit	m_wndBLevelEdit;
	CEditUnit	m_wndBedRockLevelEdit;
	BOOL	m_bStoryCenter;
	int		m_nCentOpt;
	BOOL  m_bResponse;
	int   m_nStorCenterMethod;
	BOOL  m_bStorDriftByMaxVertElem;
	MButton	m_wndConsiderStorMdul;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBldcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	CDBDoc* m_pDoc;
	T_BLDC_D m_Data;

	void Data2DlgVar();
	void StoryCentMan();
	BOOL DlgVar2Data();

	void SetListCtrlHeader();
	BOOL GetSelectedItem(int &nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_BLDC_BASE &data);
	void SetItem(int nIndex, T_BLDC_BASE &data);
	BOOL ValidItem(T_BLDC_BASE& data);
	BOOL Dlg2Item(T_BLDC_BASE &data);
	BOOL Item2Dlg(T_BLDC_BASE &data);

	void AlignControls();
	void ControlStoryCenterMethod();

	CArray<UINT, UINT> aAxialGroup;
	CArray<UINT, UINT> aShearGroup;
	CArray<UINT, UINT> aStiffGroup;

	CArray<UINT, UINT> aJUDGroup;
	//void JUDCtrlGroupDisable();
	/*
	CArray<UINT, UINT> aAxialForceGroup;
	CArray<UINT, UINT> aShearForceGroup;
	*/

	// Generated message map functions
	//{{AFX_MSG(CBldcDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnEtcBldcUseFlag();
	afx_msg void OnEtcBldcUnderSeisLoad();
	afx_msg void OnCmdStorCentCheck();
	afx_msg void OnCmdLcButton();
	afx_msg void OnCmdOptionRadio();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdResponseChk();
	afx_msg void OnEtcBldcShearFlag();
	afx_msg void OnCmdStorTHDriftRadio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__BLDCDLG_H__)
