#if !defined(__DgnSeisSetParamDlg_H__)
#define __DgnSeisSetParamDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisSetParamDlg.h : header file IDD_DGN_SEIS_SET_PARAM_DLG

//#include <map>
#include <vector>
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MCheckListBox.h"
#include "..\wg_base\wg_base_DialogMove.h"

#include "HeaderPre.h"

using namespace mit::frx;

class CDBDoc;

class __MY_EXT_CLASS__ CDgnSeisSetParamDlg : public CDialogMove
{
public:
	CDgnSeisSetParamDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnSeisSetParamDlg();

	// Dialog Data
	//{{AFX_DATA(CDgnSeisSetParamDlg)
	enum { IDD = IDD_DGN_SEIS_SET_PARAM_DLG };

private:
	MComboBox		m_DesignCode;
	MEdit			m_caseName;
	int				m_examTarget;
	int				m_seisPerform2;
	MComboBox		m_EarthquakeType;
	MComboBox		m_LimitType;
	MComboBox		m_seisForceDir;
	MComboBox		m_seisFroceType;
	MCheckListBox	m_loadCaseList;
	CListCtrl		m_seisParamList;
	//}}AFX_DATA

	CDBDoc*	m_pDoc;

	CArray<UINT, UINT> m_aSesmicPerformanceCtrl;
	CArray<UINT, UINT> m_aLimitStateOfBridgeCtrl;
	CArray<UINT, UINT> m_aEarthquakeLoadCtrl;
	CArray<UINT, UINT> m_aSystemButtonCtrl;

	CArray<T_SIPA_K, T_SIPA_K> m_deletedKeys;

protected:
	// ÇÔ¼ö(ÄÁÆ®·²)
	afx_msg void OnSelectedChangedDesigncodeCmb();
	afx_msg void OnSelectedChangedEarthquakeTypeCmb();
	afx_msg void OnNMClickSeisParamList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddBtn();
	afx_msg void OnModBtn();
	afx_msg void OnDelBtn();
	afx_msg void OnCancel();
	afx_msg void OnOk();
	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

private:
	void MoveSesmicPerformGroupCtrl();
	void InitControl();
	void UpdateComboLimitType();
	void InitDesignCodeComboBox();
	void SetDesignCodeComboBox(const int& nDesignCode);
	
	void UpdateControlByDesigncode();
	void UpdateControlByH14_24();
	void UpdateControlByH29();
	void MoveBelowControl(const int& nDistY);

	void InitLoadCaseListCtrl();
	void InitSeiaParamListCtrl();
	void SetHeaderTitleListCtrl(const int& nDesignCode);
	BOOL InsertItemListCtrl(T_SIPA_K Key, T_SIPA_D& Data, const int& nDesignCode);
	CString DataToStr_H14H24(int i, T_SIPA_D& data);
	CString DataToStr_H29(int i, T_SIPA_D& data);
	
	void DlgToData(T_SIPA_D& rData);
	void GetThisKeys(CArray<T_THIS_K, T_THIS_K>& rKeys);
	BOOL CheckAddedData(IN T_SIPA_D& data);
	BOOL CheckModifiedData(const int& nSelItem, IN T_SIPA_D& modifyData);
	BOOL IsDuplicatedCaseName(IN CString& strCaseName);
	BOOL IsSameSeismicPerformanceType(const T_SIPA_D& data);
	BOOL IsDuplicatedSeismicForceType(const T_SIPA_D& data, const int& nSelItem=-1);

	BOOL InsertItem(int nIndex, T_SIPA_D& data);
	BOOL ModifyItem(int nIndex, T_SIPA_D& data);
	void DeleteItem(int nIndex);

	void UpdataDialog(const int& nItem);
	void UpdataDialog_H14H24(const int& nItem);
	void UpdataDialog_H29(const int& nItem);

	void ListToData_H14H24(const int& nItem, T_SIPA_D& rData);
	void ListToData_H29(const int& nItem, T_SIPA_D& rData);

	void GetLoadCaseKeys(const CString& str, T_KEY_LIST& rThisKeys);
	void GetLoadCaseNames(const CString& str, CStringArray& rNames);
};
#include "HeaderPost.h"

#endif // !defined(__DgnSeisSetParamDlg_H__)