#if !defined(AFX_DGNGENLENGDLG_H__147352C9_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNGENLENGDLG_H__147352C9_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_baseMouseEdit.h"

class CDBDoc;
class CDgnGenLengDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CDgnGenLengDlg(CWnd* pParent = NULL);
	virtual void Execute();

	enum { IDD = IDD_DGN_GEN_LENG_DLG };

	CMouseEdit	m_BLzCtrl;
	CMouseEdit	m_BLyCtrl;
	CMouseEdit	m_BLuCtrl;
	CMouseEdit	m_BLtCtrl;
	CTextUnit	m_BLzUnit;
	CTextUnit	m_BLyUnit;
	CTextUnit	m_BLuUnit;
	CTextUnit	m_BLtUnit;

	int	m_nOption;
	int	m_nElemType;
	double m_BLy;
	double m_BLz;
	double m_BLu;
	double m_BLt;
	BOOL m_bNotUseBLu;
	BOOL m_bAutoCalcLe;

public:
	void Initial_SelectItem();
	void Initial_Data();
	BOOL ErrorCheck();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();

protected:
	void InitCtrl();
	void AlignControl();
	void EnableDisableControls();
	void ShowHideControls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnLengthAr();
	afx_msg void OnDgnLengthDel();
	afx_msg void OnDgnLengthNotlength();
	afx_msg void OnDgnLengthAutocalcBle();
	afx_msg void OnDgnCPGElemTypeRdo();
	DECLARE_MESSAGE_MAP()

protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aGenGroup;
	CArray<UINT, UINT> m_aButtonGroup;
	CArray<UINT, UINT> m_aPositionCtrl, m_aElemTypeCtrl;
};

#endif
