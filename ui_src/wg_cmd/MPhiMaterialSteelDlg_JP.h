#if !defined(__MPHI_MATERIAL_STEEL_DLG_JP_H__)
#define __MPHI_MATERIAL_STEEL_DLG_JP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MPhiMaterialSteelDlg_JP.h : header file
//

#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MComboBox.h"
#include "..\wg_base\FoldGroupBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CMPhiMaterialSteelDlg_JP dialog

class CDBDoc;
struct MPhiDataAll;
class CMPhiMaterialDlg_JP;
class CMPhiMaterialSteelDlg_JP : public CDlgChild, public CDBUpdateConnector
{
// Construction
public:
	CMPhiMaterialSteelDlg_JP(MPhiDataAll* pMPhiDataAll, CMPhiMaterialDlg_JP* pParent = NULL, int nType=0);   // standard constructor
	
public:
	// Dialog Data
		//{{AFX_DATA(CDgnSrcMatDlg)
	enum {IDD = IDD_CMD_MPHI_ELEMENT_S_DLG};

	CFoldGroupBox_Cross	m_FoldGrp;
	CListCtrl	m_ListCtrl;
	mit::frx::MComboBox m_SheathSteelCode;
	mit::frx::MComboBox m_SheathSteelName;
	MComboBox	m_Code;
	MComboBox	m_MatName;
	CString	m_MatName2;
	CString m_SheathSteelName2;
	BOOL      m_bSheath;

	CTextUnit	m_Fy2Unit;
	CTextUnit	m_Fy1Unit;
	CTextUnit	m_FuUnit;
	CTextUnit	m_EsUnit;
	CTextUnit	m_PsUnit;
	CTextUnit	m_SteelSurrendUnit;
	CTextUnit	m_SheathEcUnit;
	CTextUnit	m_SheathAckUnit;
	CTextUnit	m_SheathHeightUnit;
	CTextUnit	m_SheathWeightUnit;

	double	m_Ps;
	double	m_Es;
	double	m_Fu;
	double	m_Fy1;
	double	m_Fy2;
	double m_SteelSurrend;
	double m_SheathEc;
	double m_SheathAck;
	double m_SheathHeight;
	double m_SheathWeight;

	//}}AFX_DATA

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void AlignBtnMove();
	void FoldResetVisible();
	void Initial_ListCtrl();
	void Initial_Data();
	void ShowEditBox(int nCheck);
	CString GetDesignCodeName();
	void Write_ListCtrlBox(int Index, CString strID, CString strName, CString strEs, CString strFu,CString strFy1, CString strFy2, BOOL bMatdRein);
	void EnableEditBox(BOOL bCheck);
	void Initial_Unit();
	void Init_SheathCase();
	void Init_SheathComboBox();
	void Set_SheathCase(const T_MATD_REIN_D& reinD);
	void SetCtrlAtReinSP(const T_MATD_REIN_SP* pSP);
	void SetCtrlAtReinCF(const T_FILL_MATL_SP* pFM);
	void Initial_MaterialCombo(CString strCode, CString strName);
	void EnableSheathEditBox(BOOL bCheck);
	BOOL ErrorCheck();
// Implementation
protected:
	CMPhiMaterialDlg_JP* m_pParent;
	CDBDoc* m_pDoc;
	int m_nScrollPos;
	MPhiDataAll* m_pMPhiDataAll;
	T_MATL_K m_kMatl;

	// Generated message map functions
	//{{AFX_MSG(CDgnSrcMatDlg)
	virtual BOOL OnInitDialog();
//	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnItemchangedDgnSteelmatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDgnSteelmatCode();
	afx_msg void OnSelchangeDgnSteelMat();
	afx_msg void OnClickDgnSteelmatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeSheathSteelCode();
	afx_msg void OnChangeSheathSteelName();
	afx_msg void OnClickbSheath();
	afx_msg void OnAddModBtn();
	afx_msg void OnDeleteBtn();
	afx_msg  LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL DlgToData(const T_MATL_K& key, OUT T_MATD_D& rMatd, OUT T_MATD_REIN_D& rRein);
	void DlgToDataRein(const T_MATL_K& key, OUT T_MATD_REIN_D& rRein);
	void ModifyItem(const int& nLineNo, const T_MATL_K& Key, const T_MATD_D& mData);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__MPHI_MATERIAL_STEEL_DLG_JP_H__)
