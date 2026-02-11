#if !defined(__CMDSETORTHOLCDLG_H__)
#define __CMDSETORTHOLCDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenSeisFac.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_CobxSpfc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "LoadCombDefineData.h"
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSeisFactor dialog

struct T_SEIS_LCTP	
{
	int		index;
	int		iLoadCaseType;			// 0: Static,		1: Response Spectrum
	int   iXYLoadCase;			  // 0: X, 1:Y

	void Initialize()
	{
		iLoadCaseType = 0;
	}
};

class __MY_EXT_CLASS__ CCmdSetOrthoLCDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCmdSetOrthoLCDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCmdSetOrthoLCDlg)
	enum { IDD = IDD_ETC_SET_OTHO_LCG_DLG };
	
	MComboBox	m_LoadCaseX;
	MComboBox	m_LoadCaseY;
	MComboBox	m_LoadCaseZ;
	CListCtrl	m_ListCtrl;
//	double	m_OSFac;
//	double	m_VFFac;
	//}}AFX_DATA

public:
	T_LCOE_D m_Data;
	BOOL m_bUser;
	std::map<LOAD_CASE_K, LOAD_CASE_K>  m_mapBiEarthquake;
public:
	void SetUserDefDataList(const std::map<LOAD_CASE_K, LOAD_CASE_K>  &mapBiEarthquake, BOOL bUserDef);
	void GetUserDefDataList(std::map<LOAD_CASE_K, LOAD_CASE_K>  &mapBiEarthquake);
	void Initial_LCaseComboBox();

	void SetListCtrlHeader();
	BOOL GetSelectedItem(int &nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_LCOE_BASE &data);
	void SetItem(int nIndex, T_LCOE_BASE &data);
	BOOL ValidItem(T_LCOE_BASE& data);
	BOOL Dlg2Item(T_LCOE_BASE &data);
	BOOL Item2Dlg(int nIndex, T_LCOE_BASE &data);
	BOOL CheckLoadCaseName(CString str, int& Index);
	int  GetLoadCaseType(int iXY, int iIndex);
	void SetLoadCaseCombo(T_LCOE_BASE &data);

	// Coded by Seungjun MNet:No.2516 ('20061027)
	BOOL GetUsedEQKeyList();
	BOOL ValidItem(T_LCOE_BASE& data, CString& strErrorMsg);
	void SetUseLoadCaseZCombo(BOOL bUse) { m_bUseLoadCaseZCombo = bUse; }
	void SetShowLoadCaseZ(BOOL bShow) { m_bShowLoadCaseZ = bShow; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenSeisFactor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<T_SEIS_LCTP, T_SEIS_LCTP&> m_arLCtype[3];
	// Coded by Seungjun MNet:No.2516 ('20061027)
	CArray<T_STLD_K, T_STLD_K> m_arEQKeyList;
	CMap<T_STLD_K, T_STLD_K, BOOL, BOOL> m_mInvalidEQKeyList;
	CString m_strLCNameX, m_strLCNameY, m_strLCNameZ;

	// Generated message map functions
	//{{AFX_MSG(CDgnGenSeisFactor)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnCmdOLGAdd();
	afx_msg void OnCmdOLGModify();
	afx_msg void OnCmdOLGDelete();
	afx_msg void OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdOLGAuto();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// [PMS 5388] 내진성능평가
public:
	void SetSeismicEvaluation(BOOL bSet) { m_bSeismicEvaluation=bSet; }

private:
	bool CheckAndSetInvalidItemList(T_LCOE_BASE& data);
	void CreateAutoGenerate_2Dir();
	void CreateAutoGenerate_3Dir();

protected:
	BOOL m_bSeismicEvaluation;
	BOOL m_bUseLoadCaseZCombo;
	BOOL m_bShowLoadCaseZ;
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDSETORTHOLCDLG_H__)
