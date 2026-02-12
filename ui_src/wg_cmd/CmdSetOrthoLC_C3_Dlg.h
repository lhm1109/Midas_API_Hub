#if !defined(__CMDSETORTHOLCC3DLG_H__)
#define __CMDSETORTHOLCC3DLG_H__

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

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "LoadCombCtrl.h"
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSeisFactor dialog

struct T_SEIS_LCTP	
{
	int		index;
	int		iLoadCaseType;			// 0: Static, 1: Response Spectrum 2 : None
	int   iXYLoadCase;			  // 0: X, 1:Y 2: Z

	void Initialize()
	{
		iLoadCaseType = 0;
	}
};

class __MY_EXT_CLASS__ CCmdSetOrthoLC_C3_Dlg : public CDialogMove, public CDBUpdateConnector
{
	// Construction
public:
	CCmdSetOrthoLC_C3_Dlg(CWnd* pParent = NULL);   // standard constructor
    CCmdSetOrthoLC_C3_Dlg(bool bConsiderST, CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CCmdSetOrthoLC_C3_Dlg)
	enum { IDD = IDD_ETC_SET_OTHO_LCG_C3_DLG };

	MComboBox	m_LoadCaseX;
	MComboBox	m_LoadCaseY;
	MComboBox	m_LoadCaseZ;
	CListCtrl	m_ListCtrl;
	//}}AFX_DATA

public:
	void SetData(int& nOrthoCombType, CArray<_LCOM_100_30_LC3, _LCOM_100_30_LC3&>& ar100_30_Load);
	void GetData(int& nOrthoCombType, CArray<_LCOM_100_30_LC3, _LCOM_100_30_LC3&>& ar100_30_Load);


public:

	void Initial_LCaseComboBox();

	void SetListCtrlHeader();
	BOOL GetSelectedItem(int &nIndex);
	void MakeItemEx();
	CString DataToStr(int iLCase, _LCOM_100_30_LC3 &data);
	void SetItem(int nIndex, _LCOM_100_30_LC3 &data);
	BOOL ValidItem(_LCOM_100_30_LC3& data);
	BOOL Dlg2Item(_LCOM_100_30_LC3 &data);
	BOOL Item2Dlg(int nIndex, _LCOM_100_30_LC3 &data);
	BOOL CheckLoadCaseName(CString str, int& Index);
	int  GetLoadCaseType(int iXY, int iIndex);
	void SetLoadCaseCombo(_LCOM_100_30_LC3 &data);

	// Coded by Seungjun MNet:No.2516 ('20061027)
	BOOL ValidItem(_LCOM_100_30_LC3& data, CString& strErrorMsg);

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
	CString m_strLCNameX, m_strLCNameY, m_strLCNameZ;

	// Generated message map functions
	//{{AFX_MSG(CDgnGenSeisFactor)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdOLGCombBoth();
	afx_msg void OnCmdOLGCombPlus();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClickDgnGenSeisList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedDgnGenSeisList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnSelchangeDgnGenSeis();
	afx_msg void OnCmdOLGAdd();
	afx_msg void OnCmdOLGModify();
	afx_msg void OnCmdOLGDelete();
	afx_msg void OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnCmdOLGAuto();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int m_nOrthoCombType;  // 0 : Both(+-), 1: Combination Plus Only(++)
	CArray<_LCOM_100_30_LC3, _LCOM_100_30_LC3&> m_a100_30_Load;

private:
    bool m_bConsiderST;
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CmdSetOrthoLC_C3_Dlg_H__)
