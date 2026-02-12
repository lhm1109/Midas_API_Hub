#if !defined(__SOILPROP_ADDMOD_DLG_H__)
#define __SOILPROP_ADDMOD_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindPressureProfileDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\Grid\GXWND.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CSoilPropAddModDlg dialog
class CSoilParamAddLoadGrid;
#include "HeaderPre.h"      /////////

class __MY_EXT_CLASS__ CSoilPropAddModDlg : public CDialogMove
{
	// Construction
public:
	CSoilPropAddModDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSoilPropAddModDlg();

	enum { IDD = IDD_ETC_EP_PARAM_OF_SOIL_PROP };

public:
	CDBDoc* m_pDoc;
	MEdit	m_editName;
	MEdit	m_editDesc;
	BOOL	m_bUseN;
	CEditUnit	m_editGroundLevel;
	CTextUnit	m_wndGLevelUnit;
	CEditUnit	m_editBedrockLevel;
	CTextUnit	m_wndBedLevelUnit;
	CEditUnit	m_editFootingLevel;
	CTextUnit	m_wndFootingLevelUnit;
	CEditUnit	m_editHeight;
	CTextUnit	m_wndHeightUnit;
	CTextUnit	m_wndStWdithUnit;
	CTextUnit	m_wndStHeightUnit;
	CFormulaEditSpin	m_editCopies;
	CDlgUtil* m_pDlgUtil;
	CEditUnit	m_editStWidth;
	CEditUnit	m_editStHeight;
	BOOL m_bCivil;
	CArray<UINT, UINT> m_aCtrl_ShowOnKDS2018;
	CArray<UINT, UINT> m_aCtrl_HideOnKDS2018;

	CSoilParamAddLoadGrid* m_pGridWnd;
	T_SEIS_K m_Key, m_OldKey;
	T_POSP_D m_Data;
	BOOL m_bModify;
public:
	void SetParamData(T_POSP_K Key, T_POSP_D &ParamData);

protected:
	void InitGrid();
	BOOL Dlg2Data();
	BOOL Data2Dlg();
	void MakeNewData();
	void InitHeightCtrls();
	BOOL GetHeightData(double &dHeight, int &nRowCnt);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void OnCmdApply();

	afx_msg void OnChangeGL();
	afx_msg void OnCmdUseNChk();
// 	afx_msg void OnDeltaposCmdCopies(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdAdd();
	afx_msg void OnCmdDel();
	afx_msg void OnCmdInsert();
	afx_msg void OnCmdCalc();
	afx_msg void OnCmdFFA();
	afx_msg void OnCmdFTFImport();
	afx_msg void AlignControls();
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"      /////////

#endif // !defined(__SOILPROP_ADDMOD_DLG_H__)
