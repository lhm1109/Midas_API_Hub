#if !defined(AFX_STLDSSEIDLG_H__A5BECCE8_E277_4217_A667_1E9AE68173FE__INCLUDED_)
#define AFX_STLDSSEIDLG_H__A5BECCE8_E277_4217_A667_1E9AE68173FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldSseiDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditTemp.h"
#include "..\wg_db\wg_db_CobxGrup.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_db\SelectCtrl.h" // 추가 
#include "..\wg_base\wg_base_NumericOptimizer.h" // 추가 

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CStldSseiDlg dialog

class CStldSseiDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CStldSseiDlg(CWnd* pParent = NULL);   // standard constructor

	virtual void Execute() { }
virtual BOOL ExternalInit(UINT key);
// Dialog Data
	//{{AFX_DATA(CStldSseiDlg)
	enum { IDD = IDD_TM_STLD_SSEI };
	CEditUnit	m_editKh;
	BOOL m_bCheckUSE;
	CString m_strDescription; 
	int m_nDirectionMtd; 
	CMouseEdit	m_wndT1Edit; 
	CMouseEdit	m_wndT2Edit; 
	CEditUnit	m_wndT3Edit; 
	CTextUnit	m_wndT1Unit; 
	CTextUnit	m_wndT2Unit; 
	CTextUnit	m_wndT3Unit; 
	CSelectLC	m_LoadCase;
	CCobxGrup m_wndGrupCobx;
	CSelectEdit	m_editSelectElem;
	CListCtrl m_List;
	BOOL m_bAdditional;
	BOOL m_bSseiNodal;
	BOOL m_bSseiBeam;
	MComboBox	m_LoadCase2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldSseiDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL  
	
virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
virtual void UpdateBuffer();
virtual BOOL OnInitDialog();

public:
	//void OnTabSelect(WPARAM wParam,LPARAM lParam);
	//BOOL GetParameter(_NodeCreate& CS)

// Implementation
protected:
	CDBDoc* m_pDoc;
	void SetListCtrlHeader();
	BOOL GetSelectedItem(T_SSEI_K &key);
	void MakeItemEx();
	CString DataToStr(int i, T_SSEI_K key, T_SSEI_D &data);
	void SetItem(int nIndex, T_SSEI_K key, T_SSEI_D &data);
	BOOL ValidItem(T_SSEI_K key, T_SSEI_D &data);
	BOOL Dlg2Data(T_SSEI_K &key, T_SSEI_D &data);
	BOOL Data2Dlg(T_SSEI_K key, T_SSEI_D &data);
	BOOL GetSelectedSsei(T_SSEI_K &key);
	void GetAngle();
	BOOL GetSelectedData(int &nIndex);
	void OnMakeStldCmb();

	// Generated message map functions
	//{{AFX_MSG(CStldSseiDlg)
	afx_msg void OnTmBtnLc();
	afx_msg void OnTmDefineLdgrBtn();
	afx_msg void OnTmAdd();
	afx_msg void OnTmModify();
	afx_msg void OnTmDelete();
	afx_msg void OnTmChkUse();
	afx_msg void OnTmClose();
	afx_msg void OnTmSeiRdo();
	afx_msg void OnSelchangeTmGrupCboUse();
	afx_msg void OnUpdateTm2stEdt();
	afx_msg void OnChangeTm2stEdt();
	afx_msg void OnItemchangedTmList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmChkAdditional();
	//}}AFX_MSG
	//afx_msg LRESULT OnMouseEditEnter(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDSSEIDLG_H__A5BECCE8_E277_4217_A667_1E9AE68173FE__INCLUDED_)
