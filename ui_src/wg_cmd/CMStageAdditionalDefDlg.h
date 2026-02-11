#if !defined(__CMStageAdditionalDefDlg_H__)
#define __CMStageAdditionalDefDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif 

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_db\wg_db_SelectLC.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMStageDefineDlg dialog
/////////////////////////////////////
#include "CMDlgBarBase.h"
/////////////////////////////////////////////////////////////////////////////
// CCMStageAdditionalBuckDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "HeaderPre.h"

// CChildDialogMove ?
class  __MY_EXT_CLASS__ CCMStageAdditionalDefDlg : public CDialogMove, public CDBUpdateConnector
{
public:
	CCMStageAdditionalDefDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMStageAdditionalDefDlg();

public:
	enum { IDD = IDD_CMD_DEF_STAGE_ADDITIONAL_DEF_DLG };

	BOOL		m_bInitAdd;

	CSelectLC	m_LoadCase;

	MComboBox	m_cmbStage;
	MComboBox	m_cmbLoadCase;
	CListCtrl	m_List;
	//CListCtrl	m_wndLoadCaseList;
	MButton		m_bBucklingBtn;
	BOOL		m_bBucklingChk;

public:
	CDBDoc* m_pDoc;

	T_CSPA_D m_CspaData;
	T_CSPA_K m_CspaKey;

	T_STAG_K m_SelectedStageKey;
	CArray<T_STLD_K, T_STLD_K> m_aSelectedLoadCases;

public:
	virtual BOOL OnInitDialog();

protected:
	BOOL		m_bHasFinal;
	T_STAG_K	m_iFinalKey;
	CArray<T_STLD_K, T_STLD_K> m_aLoadCaseKeys;

	BOOL GetSelectedLoadCaseKey(T_STLD_K& outKey);
	void SetListCtrlHeader();
	BOOL GetSelectedItem(int& nIndex);
	void MakeItemEx();
	void SetItem(int nIndex, T_STLD_K& Key);

	void InitializeStageCombo();
	void InitializeLoadCaseCombo();
	//void InitializeLoadCaseList();

	BOOL IsExistCspa(T_CSPA_D& CspaOld, T_CSPA_D& CspaNew);

	//BOOL ValidItem(T_CSPA_D& data);
	BOOL Dlg2Data(T_CSPA_D& data);
	BOOL Data2Dlg(T_CSPA_D& data);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	void CtrlMan();

	//BOOL Dlg2Item(T_CSPA_K& Key);

	afx_msg void OnCmdSelectChangeStage();
	afx_msg void OnCmdBtnLdAdd();
	afx_msg void OnCmdBtnLdDel();
	afx_msg void OnCmdBucklingChk();
	afx_msg void OnCmdBucklingCtrl();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdClose();
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif