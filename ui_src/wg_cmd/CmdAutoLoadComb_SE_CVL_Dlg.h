#if !defined(AFX_CmdAutoLoadComb_SE_CVL_Dlg_H__9E765C21_0CC3_11D4_888F_0000C0F30D4D__INCLUDED_)
#define AFX_CmdAutoLoadComb_SE_CVL_Dlg_H__9E765C21_0CC3_11D4_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadComb_SE_CVL_Dlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "LoadCombCtrl.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadComb_SE_CVL_Dlg dialog

class CCmdAutoLoadComb_SE_CVL_Dlg : public CDialogMove
{
	// Construction
public:
	CCmdAutoLoadComb_SE_CVL_Dlg(CWnd* pParent = NULL);   // standard constructor

	enum EN_CODE_SELECT
	{
		EN_SEIS_EVL_CVL = 6, // Envelope ½Ã Design Code
	};

public:
	void SetLcomType(int nLcomType) { m_nLcomType = nLcomType; }

	// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadComb_SE_CVL_Dlg)
	enum { IDD = IDD_CMD_LCOMBDEFAULT_SEISEVAL_CVL_DLG };
	MComboBox	m_Code;
	int		    m_nOption;
	MButton   m_wndOthoChk;
	int       m_nGrvLF_DC;
	int       m_nGrvLF_DW;
	int       m_nGrvLF_DC_Num;
	int       m_nGrvLF_DW_Num;
	double    m_dGrvLF_DC1;
	double    m_dGrvLF_DC2;
	double    m_dGrvLF_DW1;
	double    m_dGrvLF_DW2;
	BOOL      m_bAddEnvelope;
	int       m_nOrthoCombType;  // 0 : Both(+-), 1: Combination Plus Only(++)

	CArray<T_STLD_K, T_STLD_K> m_aSelGravLoad;
	CArray<T_STLD_K, T_STLD_K> m_aUnselGravLoad;
	CArray<_LCOM_GRAV_EFF, _LCOM_GRAV_EFF&> m_aEffectiveLoad;
	CArray<_LCOM_GRAV_EFF_MV, _LCOM_GRAV_EFF_MV&> m_aEffectiveMVLoad;
	CArray<_LCOM_100_30_LC3, _LCOM_100_30_LC3&> m_a100_30_Load;
	int       m_nDesignType;
	double    m_Factor;	
	//}}AFX_DATA

public:
	void Initial_Data();
	void Set_InitCode();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CListCtrl	m_lstGrvSel       ;
	CListCtrl	m_lstGrvUnsel     ;
	CListCtrl	m_lstEffLoad      ;
	MComboBox m_cmbEffLoadCase  ;
	double    m_dEffLoadFactor  ;

	afx_msg void OnGrvLeftBtn   ();
	afx_msg void OnGrvRightBtn  ();
	afx_msg void OnEffLoadAddBtn();
	afx_msg void OnEffLoadModBtn();
	afx_msg void OnEffLoadDelBtn();

	void Data2Dlg();
	BOOL Dlg2Data();
	void InitCtrl();
	void InitMVCombo();
	void SetListCtrlHeader(CListCtrl& list, const int nColNum, const CString* aTitle, const int* nColWidth);
	BOOL GetUsedStldKeyList(CArray<T_STLD_K, T_STLD_K>& aKeyList);

	void AlignControl();
	void CtrlManager();
	void SetDefaultGravData();
	BOOL GetLoadCombTypeByLoadCaseType(int nLoadCaseType, int& nAnalType);
	BOOL ErrorCheck();
	BOOL CheckMovingCaseName(CString str,int& Index);
	BOOL IsEff_MV_Type(CString str);
	BOOL IsEff_LL_Type(CString str);


	CDBDoc* m_pDoc;
	CString m_strCodeName;
	CString m_strOldCodeName;
	CArray<BOOL, BOOL> m_aCodeEnable;
	int m_nOldCodeIndex;

	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadComb_SE_CVL_Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdLoadcombAdd();
	afx_msg void OnCmdLoadcombReplace();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdSetOtholcBtn();
	afx_msg void OnCmdOthoChk();
	afx_msg void OnCodeSelchange();
	afx_msg void OnCmdGrvLFType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nLcomType;
	CStringArray m_arrEff_MVData;
	CStringArray m_arrEff_LLData;
	void OnCmdGravLFEnableDisable();
	void GetGravLoadFactor(CString strCodeName);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CmdAutoLoadComb_SE_CVL_Dlg_H__9E765C21_0CC3_11D4_888F_0000C0F30D4D__INCLUDED_)
