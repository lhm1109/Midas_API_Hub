#if !defined(__DGNSRCMATDLG_H__)
#define __DGNSRCMATDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSrcMatDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

#include "HeaderPre.h"


class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnSrcMatDlg dialog

class __MY_EXT_CLASS__ CDgnSrcMatDlg : public CDialogMove
{
// Construction
public:
	CDgnSrcMatDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnSrcMatDlg)
	enum { IDD = IDD_DGN_SRC_MAT_DLG };
	CTextUnit	m_FuUnit;
	CTextUnit	m_Fy1Unit;
	CTextUnit	m_Fy2Unit;
	CTextUnit	m_Fy3Unit;
	CTextUnit	m_Fy4Unit;
	CTextUnit	m_Fy5Unit;  // add by Seungjun (`06.05.30).
	CTextUnit	m_Fy6Unit;  // add by Seungjun (`06.05.30).
	CTextUnit	m_Fy7Unit;
	CTextUnit	m_Fy8Unit;
	CTextUnit	m_Fy9Unit;
	CTextUnit	m_Fy10Unit;
	CTextUnit	m_EsUnit;
	CTextUnit	m_FcUnit;
	mit::frx::MComboBox	m_SubRebar;
	mit::frx::MComboBox	m_MainRebar;
	mit::frx::MComboBox	m_RebarCode;
	mit::frx::MComboBox	m_sMatName;
	mit::frx::MComboBox	m_cMatName;
	mit::frx::MComboBox	m_cCodeCombo;
	mit::frx::MComboBox	m_sCodeCombo;
	CTextUnit	m_reFysUnit;
	CTextUnit	m_reFyUnit;
	CListCtrl	m_ListCtrl;
	CString	m_cMatName2;
	CString	m_sMatName2;
	CString	m_MainRebar2;
	CString	m_SubRebar2;
	double	m_Fu;
	double	m_Es;
	double	m_Fy1;
	double	m_Fy2;
	double	m_Fy3;
	double	m_Fy4;
	double	m_Fy5;  // add by Seungjun (`06.05.30).
	double	m_Fy6;  // add by Seungjun (`06.05.30).
	double	m_Fy7;
	double	m_Fy8;
	double	m_Fy9;
	double	m_Fy10;
	double	m_Fc;
	double	m_Fyr;
	double	m_Fys;
	//}}AFX_DATA

public:
	void SetMainRebarStrength(CString strCode, CString strName);
	void SetSubRebarStrength(CString strCode, CString strName);
	void SetRebarCode(T_MATD_D mData);
	void Initial_MainRebarCombo(CString strCode, CString strName);
	void Initial_SubRebarCombo(CString strCode, CString strName);
	void SetMainRebarCombo(CString strCode, CString strName);
	void SetSubRebarCombo(CString strCode, CString strName);
	void Initial_SteelMatName(CString sCodeName, CString sCodeMatlName);
	void Initial_ConcreteMatName(CString cCodeName, CString cCodeMatlName);
	void SetConcreteStrength(CString strCode, CString strName);
	void SetSteelStrength(CString strCode, CString strName);
	void Initial_RebarCombo(T_MATD_D mData);
	void ShowSteelMatName(int Index);
	void ShowConcreteMatName(int Index);
	void ShowRebarMatName(int Index);
	void Initial_SteelMatCombo(CString sCodeName, CString sCodeMatlName);
	void Initial_ConcreteMatCombo(CString cCodeName, CString cCodeMatlName);
	void Initial_MaterialCombo(CString sCodeName, CString sCodeMatlName, CString cCodeName, 
												CString cCodeMatlName);
	void Initial_ListCtrl();
	void Initial_Data();
	void Initial_Unit();
	void Write_ListCtrlBox(int Index, CString strID, CString strName, CString strSteel,
												CString strConcrete, CString strMainRebar, CString strSubRebar);

	int GetChkKind(CString strMatlCode);
	BOOL ErrorCheck();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSrcMatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnSrcMatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedDgnSrcmatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickDgnSrcmatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDgnSrcConcmat();
	afx_msg void OnSelchangeDgnSrcSteelmat();
	afx_msg void OnSelchangeDgnSrcmatConccode();
	afx_msg void OnSelchangeDgnSrcmatSteelcode();
	afx_msg void OnSelchangeDgnSrcMainname();
	afx_msg void OnSelchangeDgnSrcSubname();
	afx_msg void OnSelchangeDgnSrcRebarcode();
	afx_msg void OnDgnSrcmatModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"


#endif
