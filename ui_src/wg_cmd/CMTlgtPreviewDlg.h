#if !defined(__CMTLGTPREVIEWDLG_H__)
#define __CMTLGTPREVIEWDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTlgtPreviewDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_MySRGraph.h"

#include "CMTlgtDataStore.h"

/////////////////////////////////////////////////////////////////////////////
// CCMTlgtPreviewDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMTlgtPreviewDlg : public CDialogMove
{
	// Construction
public:
	CCMTlgtPreviewDlg(CDBDoc* pDoc, CCMTlgtDataStore* pDStore, CWnd* pParent = NULL);

	enum { IDD = IDD_ETC_TLGT_PREVIEW_DLG };

protected:
	CDBDoc* m_pDoc;
	CCMTlgtDataStore* m_pDStore;
	TLGT_INFO m_trainItem;
	CListCtrl m_ctrlList;
	
	CMySRGraph m_Graph;
	SREGraphView m_GraphView;

public:
	void SetData(TLGT_INFO& item);
	void ConvertInData(TLGT_INFO& trainInfo);
	void InitChart();
	void SetValue(int nIndex, int nGroup, double x, double y);
	void SetText();
	void SetControl();
	void SetListCtrlItem();
	void SetListCtrlHeader();
	void SetAxisLabelText(int nComponent, LPCTSTR lbl);
	BOOL CalcPreview(TLGT_INFO& trainInfo, T_NODE_K KeyNode);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClickListItem(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif 
