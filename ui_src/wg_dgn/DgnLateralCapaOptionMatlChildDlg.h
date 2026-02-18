#if !defined(AFX_DGNLATERALCAPAOPTIONMATLCHILDDLG_H__E5B29378_F957_4B5A_B664_60551D103415__INCLUDED_)
#define AFX_DGNLATERALCAPAOPTIONMATLCHILDDLG_H__E5B29378_F957_4B5A_B664_60551D103415__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnLateralCapaOptionMatlChildDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_DlgChild.h"
//#include "..\wg_base\wg_base_ChildDialog.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "DgnLateralCapaOptionDef.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnLateralCapaOptionMatlChildDlg dialog
struct DgnListData
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
class CDgnLateralCapaOptionMatlDlg;
class CDgnLateralCapaOptionMatlChildDlg : public CDlgChild, public CDBUpdateConnector
{
// Construction
public:
	CDgnLateralCapaOptionMatlChildDlg(LateralCapaDataAll* pDataAll, CWnd* pParent = NULL, int nType=0);   // standard constructor
	
public:
	CDBDoc* m_pDoc;  
	CDgnLateralCapaOptionMatlDlg *m_pParent;
	int m_nType;
	int m_nCurSelectedKey;

	LateralCapaDataAll* m_pDataAll;
	T_MPHG_D m_Data;

// Dialog Data
	//{{AFX_DATA(CDgnLateralCapaOptionMatlChildDlg)
	enum { IDD = IDD_DGN_LATERAL_CAPA_OPT_MATERIAL_RCPSC_DLG };
	CListCtrl	m_cPropertyListCtrl;	
	int				m_nLimitState;
	int				m_nConsider;  
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnLateralCapaOptionMatlChildDlg)
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
	void InitData();
	BOOL ExistMphg(T_MPHG_K MphgK);
	void SetListCtrl(int ListLineNo, DgnListData data);	
	//void SetPmmChk() ;
	void GetEsFromPreference(double& dEs);

	BOOL GetSelectedItem(int &nIndex);
	BOOL ModifyItem(T_MATL_K KeyOld, T_MATL_K Key, T_MATD_D &mData);
	CString DataToStr(int i, int Key, DgnListData &Data);
	void EnableCtrl();

	// Generated message map functions
	//{{AFX_MSG(CDgnLateralCapaOptionMatlChildDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddReplaceBtn();
	afx_msg void OnDelBtn();
	afx_msg void OnRcpscBtn();	
	afx_msg void OnItemchangedMatlList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickMatlList(NMHDR* pNMHDR, LRESULT* pResult);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL			m_bModify;
//	CArray<UINT, UINT> m_aPmmCtrl, m_aMyCtrl, m_aMzCtrl, m_aRestCtrl, m_aMomentCtrl, m_aParameterCtrl;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNLATERALCAPAOPTIONMATLCHILDDLG_H__E5B29378_F957_4B5A_B664_60551D103415__INCLUDED_)
