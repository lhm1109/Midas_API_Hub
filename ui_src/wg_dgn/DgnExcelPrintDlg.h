#if !defined(__DgnExcelPrintDlg_H__)
#define __DgnExcelPrintDlg_H__

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MCheckListBox.h"

#include "HeaderPre.h"

using namespace mit::frx;

struct SDgnExcelPrintOpt
{
public:
	enum eSummarySize
	{
		kA3,
		kA4,
	};

	T_KEY_LIST  aPrintSipaK;
	eSummarySize summarySize;

	SDgnExcelPrintOpt() { Init(); };
	~SDgnExcelPrintOpt() {};
	
	void Init()
	{
		aPrintSipaK.RemoveAll();
		summarySize = kA3;
	}
};

class __MY_EXT_CLASS__ CDgnExcelPrintDlg : public CDialogMove
{
public:
	CDgnExcelPrintDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnExcelPrintDlg();

	// Dialog Data
	//{{AFX_DATA(CDgnSeisSetParamDlg)
	enum {
		IDD = IDD_DLG_SEIS_PRINT_DLG_JP
	};

protected:
	afx_msg void OnCancel();
	afx_msg void OnOk();
	afx_msg void CheckBoxCheck(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	void InitLoadPrintListCtrl();
	void InitButtonCtrl();
	void SetHeaderTitleListCtrl();
	void InsertItemListCtrl(T_SIPA_K Key, T_SIPA_D& Data);
	void SortList(CArray<T_THIS_K, T_THIS_K>& rkeys);
	CString DataToStr(int i, T_SIPA_D& Data);

private:
	int GetCurrentDgnCode();
	void SetPrintSipaKeyList();

public:
	void GetPrintOptions(OUT SDgnExcelPrintOpt& rPrintOpt);

private:
	CListCtrl	m_loadPrintList;
	CDBDoc*		m_pDoc;

	T_KEY_LIST  m_aPrintSipaK;
	int			m_summarySize;

};

#include "HeaderPost.h"

#endif // !defined(__DgnExcelPrintDlg_H__)