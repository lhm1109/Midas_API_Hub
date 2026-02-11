#pragma once

#include "..\wg_db\wg_db_DBDoc.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CThfcListDlg : public CDialogMove, public CDBUpdateConnector
{
public:
	CThfcListDlg(CWnd* pParent = NULL);   // standard constructor
	enum { IDD = IDD_ETC_THFC_LIST };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual void UnitChanged();

protected:
	void SetHeaderTitle(BOOL bInit = FALSE);
	virtual void MakeItemEx(BOOL bUnitChanged = FALSE);

	BOOL InsertItem(T_THFC_K Key, T_THFC_D &Data);
	BOOL DeleteItem(T_THFC_K Key, T_THFC_D &Data);
	BOOL ModifyItem(T_THFC_K KeyOld, T_THFC_K Key, T_THFC_D &Data);
	CString DataToStr(int i, T_THFC_K Key, T_THFC_D &Data);
	CString GetFilePath();

protected:
	afx_msg void OnThfcBtnAdd();
	afx_msg void OnThfcBtnAddSinusoidal();
	afx_msg void OnThfcBtnModify();
	afx_msg void OnThfcBtnDelete();
	afx_msg void OnThfcBtnClose();
	afx_msg void OnDblclkThfcList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnThfcImportBatchesCH();
	DECLARE_MESSAGE_MAP()

private:
	CDBDoc* m_pDoc;

private:
	CListCtrl m_List;
};
#include "HeaderPost.h"
