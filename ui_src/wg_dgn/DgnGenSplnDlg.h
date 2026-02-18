#if !defined(AFX_DGNGENSPLNDLG_H__147352C9_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNGENSPLNDLG_H__147352C9_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_baseMouseEdit.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnGenSplnDlg dialog

class CDgnGenSplnDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenSplnDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_DGN_GEN_SPLN_DLG };

	CMouseEdit	m_LengCtrl;
	CTextUnit	m_LengUnit;
	int		m_nOption;
	int 	m_nElemType; // Element Type (0: Element, 1: VBeam)
	double	m_SpanLength; // m_BLy

private:
	CArray<UINT, UINT> m_aPositionCtrl, m_aElemTypeCtrl;

public:
	void Initial_SelectItem();
	void Initial_Data();
	BOOL ErrorCheck();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

private:
	void InitCtrl();
	void AlignControl();
	void EnableDisableControls();
	void ShowHideControls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	CDBDoc* m_pDoc;

	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnLengthAr();
	afx_msg void OnDgnLengthDel();
	afx_msg void OnDgnCPGElemTypeRdo();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DGNGENSPLNDLG_H__147352C9_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
