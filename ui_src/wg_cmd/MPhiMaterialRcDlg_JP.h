#if !defined(__MPHI_MATERIAL_RC_DLG_JP_H__)
#define __MPHI_MATERIAL_RC_DLG_JP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MPhiMaterialRcDlg_JP.h : header file
//

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_DlgChild.h"
#include "..\mit_frx\MComboBox.h"
#include "..\wg_base\FoldGroupBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CMPhiMaterialRcDlg_JP dialog

class CDBDoc;
struct MPhiDataAll;
struct T_MATD_REIN_RC;
struct T_MATD_REIN_SP;
struct T_MATD_REIN_CF;
class CMPhiMaterialDlg_JP;
class CMPhiMaterialRcDlg_JP : public CDlgChild, public CDBUpdateConnector
{
// Construction
public:
	CMPhiMaterialRcDlg_JP(MPhiDataAll* pMPhiDataAll, CMPhiMaterialDlg_JP* pParent = NULL, int nType=0);   // standard constructor
	enum { IDD = IDD_CMD_MPHI_ELEMENT_RCPC_DLG };

	CFoldGroupBox_Cross	m_FoldGrp;
	mit::frx::MComboBox m_RebarCode;
	mit::frx::MComboBox m_MainBarName;
	mit::frx::MComboBox m_SubBarName;
	mit::frx::MComboBox m_Code;
	mit::frx::MComboBox m_MatName;
	mit::frx::MComboBox m_SheathRcCode;
	mit::frx::MComboBox m_SheathRcName;
	mit::frx::MComboBox m_SheathRc_SCode;
	mit::frx::MComboBox m_SheathRc_SNameMain;
	mit::frx::MComboBox m_SheathRc_SNameSub;
	mit::frx::MComboBox m_SheathSteelCode;
	mit::frx::MComboBox m_SheathSteelName;
	mit::frx::MComboBox m_SteelRebarCode;
	mit::frx::MComboBox m_SteelRebarName;
	mit::frx::MComboBox m_SheathCarbonSeat;
	mit::frx::MComboBox m_SheathCarbonType;
	mit::frx::MComboBox m_SheathCarbonVolume;
	CTextUnit	m_FyUnit;
	CTextUnit	m_FysUnit;
	CTextUnit	m_FcUnit;
	CTextUnit	m_TransferUnit; 
	CTextUnit	m_EcUnit;
	CTextUnit	m_AckUnit;
	CTextUnit	m_SheathFyUnit;
	CTextUnit	m_SheathFysUnit;
	CTextUnit	m_SteelSurrendUnit;
	CTextUnit	m_SteelRebarFyUnit;
	CTextUnit	m_CarbonHeightUnit;
	CTextUnit	m_CarbonFiberUnit;
	CTextUnit	m_CarbonTensUnit;
	BOOL      m_bSheath;
	BOOL      m_bChkTransfer;
	BOOL      m_bLambda;
	BOOL      m_bedtLambda = TRUE;
	CEditUnit m_edtLambda;
	CEditUnit m_edtTransfer;
	CListCtrl	m_ListCtrl;
	CString	m_MatName2;
	CString	m_SubBarName2;
	CString	m_MainBarName2;
	CString m_SheathRcName2;
	CString m_SheathRc_SNameMain2;
	CString m_SheathRc_SNameSub2;
	CString m_SheathSteelName2;
	CString m_SteelRebarName2;

	double	m_Fc;
	double	m_Fy;
	double	m_Fys;
	double	m_Ec;
	double	m_Ack;
	double	m_SheathFy;
	double	m_SheathFys;
	double m_SteelSurrend;
	double m_SteelRebarFy;
	double	m_CarbonHeight;
	double	m_CarbonFiber;
	double	m_CarbonTens;

	enum eSheathType
	{
		kAll = 0,
		kRcCode,
		kRc_SCode,
		kSteelCode,
		kSteelRebarCode,
		kSeat
	};
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSrcMatDlg)
protected:
	CMPhiMaterialDlg_JP* m_pParent;
	CDBDoc* m_pDoc;
	CString m_strCurrentItem;
	BOOL m_bTransfer;
	int m_nScrollPos;
	MPhiDataAll* m_pMPhiDataAll;
	std::map<CString, std::vector<std::tuple<double, double, double>>> m_mMaterial;
	T_MATL_K m_kMatl;

	void AlignBtnMove();
	void FoldResetVisible();
	void Init_Scroll();
	void AlignControl();
	void Init_GridData();
	void Write_ListBox(int iIndex, CString strID, CString strNa, CString strFc, CString strMbar, CString strSbar, BOOL bMatdRein);
	void Init_Unit();
	void Init_MatlCode();
	void EnableEditBox(BOOL bCheck);
	void EnableRebarEditBox(BOOL bCheck);
	void Init_Items();
	void Init_SheathCase();
	void SetCtrlAtReinRC(const T_MATD_REIN_RC* pRC);
	void SetCtrlAtReinSP(const T_MATD_REIN_SP* pSP);
	void SetCtrlAtReinCF(const T_MATD_REIN_CF* pCF);
	void Set_SheathCase(const T_MATD_REIN_D& reinD);
	void Init_SheathComboBox();
	void EnableSheathEditBox(BOOL bCheck, int sheathType);
	//void ControlsEnableDisable();
	void Init_MatlNameCombo(CString strCode, CString strNa);
	void Init_RebarCombo(CString strRebarCode, T_MATD_D rData);
	void ChangeDlgItemInfo();
	BOOL ErrorCheck();
	void ModifyMaterial( int& Index, CString& strCode, CString& strMatName, CString& strFc, CString& strChk, CString& strLambda, T_MATD_D& mData);
	void ModifyRebar(int& Index, int& reIndex, BOOL& bCheck, CString& strRebarCode, CString& strMain, CString& strSub, T_MATD_D& mData);
	void ModifyMaterialRein(const T_MATL_K& key, OUT T_MATD_REIN_D& rRein);
	void SheathShowInit();
// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDgnSrcMatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	afx_msg void OnClickDgnConcmatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedDgnConcmatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDgnConcMat();
	afx_msg void OnSelchangeDgnConcmatCode();
	afx_msg void OnSelchangeDgnConcMainname();
	afx_msg void OnSelchangeDgnConcSubname();
	//afx_msg void OnClickLambdaChk();
	//afx_msg void OnClickTransferChk();
	//afx_msg void OnDgnConcmatClose();
	afx_msg void OnSelchangeDgnConcRebarcode();
	//afx_msg void OnDgnConcElasticity();
	afx_msg void OnClickbSheath();
	afx_msg void OnChangeSheathRcCode();
	afx_msg void OnChangeSheathRc_SCode();
	afx_msg void OnChangeSheathSteelCode();
	afx_msg void OnChangeSheathSteelName();
	afx_msg void OnChangeSheathSteelRebarCode();
	afx_msg void OnChangeSheathSteelRebarName();
	afx_msg void OnChangeSheathSeatCode();
	afx_msg void OnChangeSheathType();
	afx_msg void OnChangeSheathVolume();
	afx_msg void OnChangeSheathRc_SMainName();
	afx_msg void OnChangeSheathRc_SSubName();
	afx_msg void OnChangeSheathRcName();
	afx_msg void OnAddModBtn();
	afx_msg void OnDeleteBtn();
	afx_msg  LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL Dlg2Data(OUT T_MATL_K& rKey, OUT T_MATD_D& rData, OUT T_MATD_REIN_D& rRein);
	void ModifyItem(const int& nLineNo, const T_MATL_K& Key, const T_MATD_D& mData);
	void InitRebarCode();

	void ModifyMaterialReinRC(OUT T_MATD_REIN_RC* pRC);
	void ModifyMaterialReinSP(OUT T_MATD_REIN_SP* pSP);
	void ModifyMaterialReinCF(OUT T_MATD_REIN_CF* pCF);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__MPHI_MATERIAL_RC_DLG_JP_H__)
