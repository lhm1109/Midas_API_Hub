#if !defined(__CMSELECTDLG_H__)
#define __CMSELECTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSelectDlg.h : header file
//

#include "CMDlgBase.h"
//#include "..\wg_db\wg_db_DlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\wg_db_EditMatl.h"
#include "..\wg_db\wg_db_EditSect.h"
#include "..\wg_db\wg_db_EditThik.h"
#include "..\wg_db\wg_db_EditEltp.h"


#include "..\wg_common\wg_common_Query.h"

#include "..\wg_base\AutomationBase.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMSelectDlg dialog
#include "HeaderPre.h"


class __MY_EXT_CLASS__ CCMSelectDlg : public CCMDlgBase, CAutomationBase
{
// Construction
public:
	CCMSelectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMSelectDlg)
	enum { IDD = IDD_CMD_SELECT_DLG };
	MButton	m_wndMultiple;
	CEditThik	m_wndPickThikNo;
	CEditSect	m_wndPickSectNo;
	CEditMatl	m_wndPickMatlNo;
	CEditEltp	m_wndPickElemType;
	
	MButton	m_wndElemCheck;
	MButton	m_wndNodeCheck;
	
	CListBox	m_lbItemList;
	MComboBox	m_cboCategory;
	BOOL	m_bNode;
	BOOL	m_bElem;
	BOOL	m_bMultiple;
	int		m_nStoryOpt;
	//}}AFX_DATA
	CSelectEdit m_editSelectNode;
	CSelectEdit m_editSelectElem;
	CQueryCtrl  m_QueryCtrl;

// Operations
public:
	// nCategory : wg_common/Query.h Âü°í : CAT_????
	void SelectKind(int nKind, int nCategory=CAT_ELEM_TYPE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual void UpdateUnit();

// Implementation
protected:
	void AlignControls();
	int GetCurCategory();
	void FillCategory();
	void ChangeCategory();
	void ShowHidePickCtrl(int nCategory);
	void SelectItemByKey(UINT key);
	void SelectListItemByPick(UINT nID);

	void MakeListNone();
	void MakeListElemType();
	void MakeListMatl();
	void MakeListSect();
	void MakeListThik();
	void MakeListNpln();
	void MakeListStory();
	void MakeListGroup();
	void MakeListBndrGroup();
	void MakeListLoadGroup();

	void MakeListExistOrNot();
	void MakeListCons();
	void MakeListFrls();
	void MakeListWall();
	void MakeListWmak();

	void MakeListHngt();
	void MakeListMemb();

	BOOL ValidQueryCondition();
	CQueryCtrl* GetQueryCtrl();
	void GetQueryCondition(CQueryCondition& QCond);
	void DoSelectOp(int nOpType);
	void ReplaceSelectNode(CView* pSender, CArray<T_NODE_K, T_NODE_K>&rNode);
	void ReplaceSelectElem(CView* pSender, CArray<T_ELEM_K, T_ELEM_K>&rElem, BOOL bIncludeNode);

	CArray<UINT, UINT> arStoryOption;

	// Generated message map functions
	//{{AFX_MSG(CCMSelectDlg)
	afx_msg void OnDlgClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectAll();
	afx_msg void OnSelectNone();
	afx_msg void OnSelectInversion();
	afx_msg void OnSelectPrevious();
	afx_msg void OnChangeCategory();
	afx_msg void OnClickBtnAdd();
	afx_msg void OnClickBtnDel();
	afx_msg void OnClickBtnReplace();
	afx_msg void OnClickBtnIntersect();
	afx_msg void OnToggleNodeCheck();
	afx_msg void OnToggleElemCheck();
	afx_msg void OnToggleReplaceSel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseEditEnter(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSELECTDLG_H__)
