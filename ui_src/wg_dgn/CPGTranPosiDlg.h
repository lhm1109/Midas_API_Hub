// CPGTranPosiDlg.h: interface for the CCPGTranPosiDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPGTRANPOSIDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_CPGTRANPOSIDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "../wg_base/I_GENModelBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"

using namespace mit::frx;

class CDBDoc;

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class __MY_EXT_CLASS__ CCPGTranPosiDlg : public CMenuBarChildDlg
{
public:
	CCPGTranPosiDlg(CWnd* pParent = NULL);
	virtual void Execute();

	enum { IDD = IDD_DGN_CPG_TRAN_POSI_DLG };
		
	int		m_nOption;
	int     m_nElemType;
	int		m_iPosi;
		
public:
	void Initial_SelectItem();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	CDBDoc* m_pDoc;

	void InitCtrl();	
	void AlignControl();
	void EnableDisableControls();
	void ShowHideControls();
	BOOL Dlg2Data();

	T_CGDP_D m_Data;
	CArray<UINT, UINT> m_aElemTypeCtrl;
	CArray<UINT, UINT> m_aPositionCtrl;

	virtual BOOL OnInitDialog();
	afx_msg void OnDgnCPGPosiAddDel();
	afx_msg void OnDgnCPGElemTypeRdo();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////
#endif // !defined(AFX_CPGTRANPOSIDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
