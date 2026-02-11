#if !defined(AFX_STLDSTBKDLG_H__66BE7327_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_STLDSTBKDLG_H__66BE7327_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldStbkDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxLdgr.h"

#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\MIT_frx\MillustViewer.h"

class CStldStbkDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{

public:
	CStldStbkDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStldStbkDlg();
	virtual void Execute();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_TM_STLD_STBK };

	CCobxLdgr m_cmbLdgr;
	int m_nOption;

	CEditUnit m_edtDx;
	CEditUnit m_edtDy;
	CEditUnit m_edtDz;

	CTextUnit m_untDx;
	CTextUnit m_untDy;
	CTextUnit m_untDz;

	CListCtrl m_listSdlElnk;
	mit::frx::MillustViewer m_wndPicture;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

protected:
	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
protected:
	CDBDoc* m_pDoc;

	CArray<UINT, UINT> m_aCtrlOption;
	CArray<UINT, UINT> m_aCtrlDisable;

	void InitUnit();
	void InitCtrl();
	void SetListCtrlHeader();
	void SetListCtrlItem();
	void ResizeListCtrl();
	BOOL GetSelectedSdlELink(T_ELNK_K &key);
	BOOL GetSelectedSetBack(T_STBK_K &key);
	virtual BOOL OnInitDialog();
	BOOL CheckNodeList();
	afx_msg void OnTmDefineLdgrBtn();
	afx_msg void OnChangeOption();
	afx_msg void OnChangedItemSelection(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

#endif