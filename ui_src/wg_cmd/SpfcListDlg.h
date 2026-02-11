#pragma once

#include "..\wg_db\wg_db_DBDoc.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CSpfcListDlg : public CDialogMove, public CDBUpdateConnector
{
public:
	CSpfcListDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_ETC_SPFC_LIST };

private:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual void UnitChanged();
	virtual BOOL OnInitDialog();

private:
	virtual void MakeItemEx(BOOL bUnitChanged = FALSE);
	void SetHeaderTitle(BOOL bInit = FALSE);

	BOOL InsertItem(T_SPFC_K Key, T_SPFC_D &Data);
	BOOL DeleteItem(T_SPFC_K Key, T_SPFC_D &Data);
	BOOL ModifyItem(T_SPFC_K KeyOld, T_SPFC_K Key, T_SPFC_D &Data);
	CString DataToStr(int i, T_SPFC_K Key, T_SPFC_D &Data);

public:
	void SetSeismicEvaluation(BOOL bSet) { m_bSeismicEvaluation = bSet; }

private:
	afx_msg void OnSpfcBtnAdd();
	afx_msg void OnSpfcBtnModify();
	afx_msg void OnSpfcBtnEnvelope();
	afx_msg void OnSpfcBtnDelete();
	afx_msg void OnSpfcBtnClose();
	afx_msg void OnDblclkSpfcList(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

private:
	void SetEnbleEnvelope();

private:
	CDBDoc* m_pDoc;
	BOOL m_bSeismicEvaluation;
	CListCtrl m_List;
};
#include "HeaderPost.h"
