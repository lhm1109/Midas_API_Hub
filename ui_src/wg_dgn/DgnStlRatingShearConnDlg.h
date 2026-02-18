// DgnStlRatingShearConnDlg.h: interface for the CDgnStlRatingShearConnDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNSTLRATINGSHEARCONNDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_DGNSTLRATINGSHEARCONNDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "DgnTabCtrl.h"
#include "DgnStlRatingShearConnTabDlg.h"
#include "DgnStlRatingShearConnTabJDlg.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\MIT_frx\MButton.h"

class CDBDoc;

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CDgnStlRatingShearConnTabDlg;
class CDgnStlRatingShearConnTabJDlg;

class __MY_EXT_CLASS__ CDgnStlRatingShearConnDlg : public CMenuBarChildDlg
{
public:
	CDgnStlRatingShearConnDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_DGN_STL_RATING_SHEAR_CONN_DLG };

	int		m_nOption;
	int		m_nElemType;
	BOOL	m_bBothIJ;
	CDlgTabCtrl	m_Tab;
	mit::frx::MButton m_Ctrl_BothIJ;

	T_SRSC_STUD m_I_Data;
	T_SRSC_STUD m_J_Data;
	T_CPSC_TYPE m_I_VData;
	T_CPSC_TYPE m_J_VData;

	BOOL m_bJTabEnable;

private:
	CArray<UINT, UINT> m_aPositionCtrl, m_aElemTypeCtrl;

public:
	void Initial_SelectItem();
	void SetText();
private:
	void InitCtrl();
	void AlignControl();
	void EnableDisableControls();
	void ShowHideControls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	CDBDoc* m_pDoc;
	BOOL Dlg2Data(T_SRSC_STUD* pData);
	CDgnStlRatingShearConnTabDlg*  m_pSubDlg_I;
	CDgnStlRatingShearConnTabJDlg* m_pSubDlg_J;

	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnCPGShearAdd();
	afx_msg void OnDgnCPGShearDel();
	afx_msg void OnBothIJChk();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnCPGElemTypeRdo();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

#endif // !defined(AFX_DgnStlRatingShearConnDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
