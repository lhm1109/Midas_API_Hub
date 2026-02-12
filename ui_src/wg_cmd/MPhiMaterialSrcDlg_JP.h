#if !defined(__MPHI_MATERIAL_SRC_DLG_JP_H__)
#define __MPHI_MATERIAL_SRC_DLG_JP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MPhiMaterialSrcDlg_JP.h : header file
//

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_DlgChild.h"
#include "..\mit_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CMPhiMaterialSrcDlg_JP dialog

class CDBDoc;
struct MPhiDataAll;

class CMPhiMaterialSrcDlg_JP : public CDlgChild, public CDBUpdateConnector
{
// Construction
public:
	CMPhiMaterialSrcDlg_JP(MPhiDataAll* pMPhiDataAll, CWnd* pParent = NULL, int nType=0);   // standard constructor
	
public:
	// Dialog Data
	enum {IDD = IDD_CMD_MPHI_ELEMENT_SRC_DLG};
	
	CListCtrl	m_ListCtrl;

	CTextUnit	m_EsUnit;
	CTextUnit	m_FuUnit;
	CTextUnit	m_Fy1Unit;
	CTextUnit	m_Fy2Unit;
	CTextUnit	m_FcUnit;
	CTextUnit	m_reFysUnit;
	CTextUnit	m_reFyUnit;
	CString	m_cMatName2;
	CString	m_sMatName2;
	CString	m_MainRebar2;
	CString	m_SubRebar2;
	mit::frx::MComboBox	m_cCodeCombo;
	mit::frx::MComboBox	m_sCodeCombo;
	mit::frx::MComboBox	m_sMatName;
	mit::frx::MComboBox	m_cMatName;
	mit::frx::MComboBox	m_RebarCode;
	mit::frx::MComboBox	m_MainRebar;
	mit::frx::MComboBox	m_SubRebar;
	double	m_Es;
	double	m_Fu;
	double	m_Fy1;
	double	m_Fy2;
	double	m_Fc;
	double	m_Fyr;
	double	m_Fys;

public:
	void Initial_ListCtrl();
	void Initial_Data();
	void Write_ListCtrlBox(int Index, CString strID, CString strName, CString strSteel, CString strConcrete, CString strMainRebar, CString strSubRebar);
	void Initial_Unit();

	void Initial_MaterialCombo(CString sCodeName, CString sCodeMatlName, CString cCodeName, CString cCodeMatlName);
	void Initial_SteelMatCombo(CString sCodeName, CString sCodeMatlName);
	void ShowSteelMatName(int Index);
	void Initial_SteelMatName(CString sCodeName, CString sCodeMatlName);
	void SetSteelStrength(CString strCode, CString strName);
	void Initial_ConcreteMatCombo(CString cCodeName, CString cCodeMatlName);
	void ShowConcreteMatName(int Index);
	void Initial_ConcreteMatName(CString cCodeName, CString cCodeMatlName);
	void SetConcreteStrength(CString strCode, CString strName);

	void Initial_RebarCombo(T_MATD_D mData);
	void SetRebarCode(T_MATD_D mData);
	void ShowRebarMatName(int Index);
	void Initial_MainRebarCombo(CString strCode, CString strName);
	void SetMainRebarCombo(CString strCode, CString strName);
	void SetMainRebarStrength(CString strCode, CString strName);
	void Initial_SubRebarCombo(CString strCode, CString strName);
	void SetSubRebarCombo(CString strCode, CString strName);
	void SetSubRebarStrength(CString strCode, CString strName);

	BOOL ErrorCheck();
	void ModifySteel(CString& strSCode, CString& strSName, T_MATD_D& mData);
	void ModifyConcrete(CString& strCCode, CString& strCName, T_MATD_D& mData);
	void ModifySRC(CString& strRebarCode, CString& strMainRName, CString& strSubRName, T_MATD_D& mData);
// Overrides
	// ClassWizard generated virtual function overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	MPhiDataAll* m_pMPhiDataAll;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg  void OnItemchangedDgnSrcmatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickDgnSrcmatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDgnSrcConcmat();
	afx_msg void OnSelchangeDgnSrcSteelmat();
	afx_msg void OnSelchangeDgnSrcmatConccode();
	afx_msg void OnSelchangeDgnSrcmatSteelcode();
	afx_msg void OnSelchangeDgnSrcMainname();
	afx_msg void OnSelchangeDgnSrcSubname();
	afx_msg void OnSelchangeDgnSrcRebarcode();
	afx_msg void OnDgnSrcmatModify();
	afx_msg void OnAddModBtn();
	afx_msg void OnDeleteBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL DlgToData(const T_MATL_K& key, OUT T_MATD_D& rMatd);
	void ModifyItem(const int& nLineNo, const T_MATL_K& Key, const T_MATD_D& mData);
	void InitRebarCode();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__MPHI_MATERIAL_SRC_DLG_JP_H__)
