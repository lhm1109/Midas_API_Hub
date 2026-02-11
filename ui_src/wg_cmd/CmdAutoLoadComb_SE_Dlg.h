#if !defined(AFX_CmdAutoLoadComb_SE_Dlg_H__9E765C21_0CC3_11D4_888F_0000C0F30D4D__INCLUDED_)
#define AFX_CmdAutoLoadComb_SE_Dlg_H__9E765C21_0CC3_11D4_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadComb_SE_Dlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "LoadCombCtrl.h"
using namespace mit::frx;
class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadComb_SE_Dlg dialog

class CCmdAutoLoadComb_SE_Dlg : public CDialogMove
{
	// Construction
public:
	CCmdAutoLoadComb_SE_Dlg(CWnd* pParent = NULL);   // standard constructor

	enum EN_CODE_SELECT
	{
		EN_SEIS_EVL      ,
	};

	enum EN_SEIS_ORTHO_TYPE
	{
		EN_SEIS_ORTHO_NONE   = 0,
		EN_SEIS_ORTHO_100_30 = 1, // support only 100:30 rule.
		EN_SEIS_ORTHO_SRSS   = 2, // support only SRSS.
		EN_SEIS_ORTHO_BOTH   = 3  // support both 100:30 and SRSS.
	};

	enum EN_SEIS_LCOMADD_TYPE
	{
		EN_SEIS_LCOMADD_NONE = 0,
		EN_SEIS_LCOMADD_SPEC = 1, // support only special seismic load.
		EN_SEIS_LCOMADD_VERT = 2, // support only vertical seismic forces.
		EN_SEIS_LCOMADD_BOTH = 3
	};

public:
	void SetLcomType(int nLcomType) { m_nLcomType = nLcomType; }

	// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadComb_SE_Dlg)
	enum { IDD = IDD_CMD_LCOMBDEFAULT_SEISEVAL_DLG };
	MComboBox	m_SpecLcase;
	CListCtrl	m_CtrlList;
	MComboBox	m_Code;
	int		    m_nOption;
	MButton   m_wndOthoChk;
	int       m_nOthoType;
	MButton   m_wndVertForceChk;
	CArray<T_STLD_K, T_STLD_K> m_aSelGravLoad;
	CArray<T_STLD_K, T_STLD_K> m_aUnselGravLoad;
	CArray<_LCOM_GRAV_EFF, _LCOM_GRAV_EFF&> m_aEffectiveLoad;
	int       m_nDesignType;
	double    m_Factor;	

	//}}AFX_DATA

public:
	void Initial_Data();
//   void Initial_LCaseComboBox();
	void Set_InitCode();

protected:
	BOOL GetUsedStldKeyList(CArray<T_STLD_K, T_STLD_K>& aKeyList);
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadComb_SE_Dlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	void AlignControl(CString strCodeName);
	void CtrlManager();
	int GetSeisOrthoType(const CString& strCodeName);
	int GetSeisAddLcomType(const CString& strCodeName);
	void SetDefaultGravData();
	BOOL GetLoadCombTypeByLoadCaseType(int nLoadCaseType, int& nAnalType);
//   BOOL CheckLCName(CString str,int& Index);
	BOOL ErrorCheck();
//   void Initial_ListCtrlBox();
//   void Write_ListCtrlBox(int Index,double Factor,CString strLoadCase, int nType);
//   void OverWrite_ListCtrlBox(int Index,double Factor,CString strLoadCase, int nType);
//   void SaveLfcaListData();

protected:
	CDBDoc* m_pDoc;
	CString m_strCodeName;
	CString m_strOldCodeName;
	CArray<BOOL, BOOL> m_aCodeEnable;
	int m_nOldCodeIndex;

	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadComb_SE_Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdLoadcombAdd();
	afx_msg void OnCmdLoadcombReplace();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdDesignTypeRdo();
	afx_msg void OnCmdSetOtholcBtn();
	afx_msg void OnCmdOthoChk();
	afx_msg void OnCmdOthoType();
	afx_msg void OnCodeSelchange();
	afx_msg void OnCmdSpecialVertChk();
	afx_msg void OnCmdSeisFactorBtn();
	afx_msg void OnCmdLFforGravityBtn();

//   afx_msg void OnCmd_J_Add();
//   afx_msg void OnCmd_J_Del();
//   afx_msg void OnCmd_J_Modify();
//   afx_msg void OnClickCmd_J_List(NMHDR* pNMHDR, LRESULT* pResult);
//   afx_msg void OnItemchangedCmd_J_List(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nLcomType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CmdAutoLoadComb_SE_Dlg_H__9E765C21_0CC3_11D4_888F_0000C0F30D4D__INCLUDED_)
