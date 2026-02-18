// CPGLateraTorsionDlg.h: interface for the CCPGLateraTorsionDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPGLATERALTORSIONDLG_H__)
#define AFX_CPGLATERALTORSIONDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "DgnTabCtrl.h"
#include "CPGLateralTorsionTabDlg.h"
#include "CPGLateralTorsionTabJDlg.h"

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\MIT_frx\MButton.h"

class CDBDoc;

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGLateralTorsionTabDlg;
class CCPGLateralTorsionTabJDlg;

class __MY_EXT_CLASS__ CCPGLateraTorsionDlg : public CMenuBarChildDlg
{
public:
	CCPGLateraTorsionDlg(CWnd* pParent = NULL, int nType =0);  //nType =0 :PSC Rating.  1: Steel Rating 

	enum { IDD = IDD_DGN_CPG_RATER_TORSION_DLG };

	int		m_nType; // =0 :PSC Rating.  1: Steel Rating 
	int		m_nOption;
	int     m_nElemType;
	BOOL	m_bBothIJ;
	CDlgTabCtrl	m_Tab;
	mit::frx::MButton m_Ctrl_BothIJ;

	T_CGLT_DATA m_I_Data;
	T_CGLT_DATA m_J_Data;

	BOOL m_bJTabEnable;

private:
	CArray<UINT, UINT> m_aPositionCtrl, m_aElemTypeCtrl;

private:
	void InitCtrl();
	void Initial_SelectItem();
	void AlignControl();
	void EnableDisableControls();
	void ShowHideControls();
	BOOL Dlg2Data(T_CGLT_DATA* pLTdata);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	

protected:
	CDBDoc* m_pDoc;
	CCPGLateralTorsionTabDlg* m_pSubDlg_I;
	CCPGLateralTorsionTabJDlg* m_pSubDlg_J;

	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnCPGTorsionAdd();
	afx_msg void OnDgnCPGTorsionDel();
	afx_msg void OnBothIJChk();
	afx_msg void OnDgnCPGElemTypeRdo();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

#endif // !defined(AFX_CPGLATERALTORSIONDLG_H__)
