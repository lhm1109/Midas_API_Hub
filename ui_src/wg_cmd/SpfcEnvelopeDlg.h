#pragma once

#include "..\wg_db\wg_db_DBDoc.h"

class CSpfcEnvelopeDlg : public CDialogMove
{
public:
	CSpfcEnvelopeDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_ETC_SPFC_ENVELOPE_DLG };

private:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

private:
	void SetHeaderTitle(bool bInit = FALSE);
	void SetHeaderTitle(CListCtrl& rListCtrl, bool bInit);

private:
	afx_msg void OnSpfcBtnAdd();
	afx_msg void OnSpfcBtnDel();
	afx_msg void OnSpfcBtnApply();
	afx_msg void OnSpfcBtnOK();
	afx_msg void OnSpfcBtnClose();
	DECLARE_MESSAGE_MAP()

private:
	void InitSpfcList();
	void AddListItem(CListCtrl& rListCtrl, CString csName);
	void DelListItem(CListCtrl& rListCtrl, CString csName);
	int  GetListItem(const CListCtrl& cListCtrl);
	void SetSpfcName();

private:
	bool AddEnvelope();
	bool CheckSpfcData();
	bool GetEnvelope(T_SPFC_D& rData);
	bool GetSpfcData(T_SPFC_D& rData1, T_SPFC_D& rData2);
	bool AddPeriod(std::vector<double>& rvdPeriod, const T_SPFC_D& cData);
	bool IsEqual(const double& dT1, const double& dT2);
	bool AdjustSpectrum(T_SPFC_D& rData, const std::vector<double>& vdPeriod);
	CString GetFuncName();

private:
	CDBDoc* m_pDoc;
	CListCtrl m_List1;
	CListCtrl m_List2;
};
