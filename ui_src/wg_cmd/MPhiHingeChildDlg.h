#if !defined(AFX_MPHIHINGECHILDDLG_H__E5B29378_F957_4B5A_B664_60551D103415__INCLUDED_)
#define AFX_MPHIHINGECHILDDLG_H__E5B29378_F957_4B5A_B664_60551D103415__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MPhiHingeChildDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_DlgChild.h"
#include "MPhiParameterDef.h"
#include "MPhiSectionImageDlg.h"

#include "..\mit_frx\MEdit.h"
#include "..\mit_frx\MComboBox.h"
#include "..\wg_base\FoldGroupBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CMPhiHingeChildDlg dialog
struct ListData
{
	int nId;
	CString cName;
	CString cGradeName;
	double	dElast;
	double	dSig;
	CString cSGradeName;
	double	dSElast;
	double	dSigSy;
	BOOL bUse;
	void initialize()
	{
		nId = 0;
		cName = _T("");
		cGradeName = _T("");
		dElast = 0.0;
		dSig = 0.0;
		cSGradeName = _T("");
		dSElast = 0.0;
		dSigSy = 0.0;
		bUse = FALSE;
	}
};
class CMPhiHingeDlg;
class CMPhiHingeChildDlg : public CDlgChild, public CDBUpdateConnector
{
// Construction
public:
	CMPhiHingeChildDlg(MPhiDataAll* pMPhiDataAll, CWnd* pParent = NULL, int nType=0);   // standard constructor
	
public:
	CDBDoc* m_pDoc;
	CMPhiHingeDlg *m_pParent;
	int m_nType;
	int m_nCurSelectedKey;

	T_MPHG_D m_Data;
	MPhiDataAll* m_pMPhiDataAll;
// Dialog Data
	//{{AFX_DATA(CMPhiHingeChildDlg)
	enum { IDD = IDD_CMD_MPHI_HINGE_RCPSC_DLG };
	
	MComboBox	m_PmmModelCobx;
	MComboBox	m_cCode;
	CListCtrl	m_cPropertyListCtrl;
	MEdit			m_NumPmmEdit;
	MEdit			m_NumMzEdit;
	MEdit			m_NumMyEdit;
	BOOL			m_bMzChk;
	BOOL			m_bMyChk;
	BOOL			m_bYbPointCbx;
	int       m_nLimitState;	
	int       m_nStrAfterLimit;	
	int				m_nConsider;
	BOOL			m_bPmmChk;
	int		m_nEarthquakeType;
	int		m_nExamTarget;
	int		m_nSectionConsider;
	int		m_nScrollPos; // 스크롤 위치를 저장하기 위한 변수
	//}}AFX_DATA

	MComboBox m_cmbModel[2];
	MComboBox m_cmbFirstBrokenLine;
	MComboBox m_cmbSecondBrokenLine;
	CFoldGroupBox_Cross	m_FoldGrp;
	CArray<UINT, UINT> m_aFoldGrpResource;
	CArray<UINT, UINT> m_aExamTargetCtrl;
	CArray<UINT, UINT> m_aExamTypeCtrl;
	CArray<UINT, UINT> m_aBtnCtrl;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMPhiHingeChildDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateList(LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer(); 
	
	
	void SetModifyData(T_MATL_K Key);

protected:
	BOOL Data2Dlg();
	BOOL Dlg2Data();
	BOOL Apply();

	void InitCtrls();
	void InitListCtrl();
	BOOL ExistMphg(T_MPHG_K MphgK);
	void ShowHideCtrl();
	void AlignCtrl();
	void InitData();
	void SetListCtrl(int ListLineNo, ListData data);
	void InitCombx();
	void AdjustComboListBox(CComboBox& Cbx) /* 콤보박스의 길이를 현재 사용되고 있는 리스트의 길이에 맞추어 변경한다. */;
	void SetPmmChk() ;
	CString GetHysModelName(int nHysModel);
	void GetEsFromPreference(double& dEs);

	BOOL GetSelectedItem(int &nIndex);
	BOOL ModifyItem(T_MATL_K KeyOld, T_MATL_K Key, T_MATD_D &mData);
	CString DataToStr(int i, int Key, ListData &Data);
	void OnSelchangeStressStrainCobx();
	void EnableCtrl();
	void ResetPmmMyMzComboBox();
	void Init_Scroll();
	void ShowHildeForFold();

	// Generated message map functions
	//{{AFX_MSG(CMPhiHingeChildDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddReplaceBtn();
	afx_msg void OnDelBtn();
	afx_msg void OnMyChk();
	afx_msg void OnMzChk();
	afx_msg void OnPmmChk();
	afx_msg void OnRcpscBtn();
	afx_msg void OnSteelBtn();
	afx_msg void OnSRCBtn();
	afx_msg void OnKeepStrGuideBtn();  
	afx_msg void OnAferLimBtn();
	afx_msg void OnSelchangeModelCobx();
	afx_msg void OnTypeIRdo();
	//virtual void OnCancel();
	afx_msg void OnItemchangedMatlList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickMatlList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeNoPmmEdt();
	afx_msg void OnSelchangePmmModelCobx();  
	afx_msg void OnDestroy();
	afx_msg void OnClickYbPointCbx();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL			m_bModify;	
	CMPhiSectionImageDlg* m_pGuideDlg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MPHIHINGECHILDDLG_H__E5B29378_F957_4B5A_B664_60551D103415__INCLUDED_)
