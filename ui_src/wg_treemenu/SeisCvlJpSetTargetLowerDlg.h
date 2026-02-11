#if !defined(__SEIS_CVL_JP_SET_TARGET_LOWER_DLG_H__)
#define __SEIS_CVL_JP_SET_TARGET_LOWER_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSeisCvlJpSetTargetLowerDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "..\wg_db\wg_db_selectctrl.h"
#include "..\wg_db\SingleSelectEdit.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetTargetLowerDlg dialog

class CDBDoc;
class CSeisCvlJpSetTargetDlg;

class CSeisCvlJpSetTargetLowerDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CListCtrl		 m_bearingList;		//지승 리스트 컨트롤
	CListCtrl		 m_columnList;		//기둥 리스트 컨트롤
	CListCtrl		 m_beamList;		//보 리스트 컨트롤
	CListCtrl		 m_footingList;		//기초 리스트 컨트롤
	CListCtrl		 m_sigrLowerList;
	MComboBox		 m_memberType;		//교각의 부재타입
	MComboBox		 m_materialType;	//교각의 재료타입

	BOOL CheckData_AddCOLM(const T_SIGR_LOWER_COLM_D& data);
	BOOL CheckData_AddBEAM(const T_SIGR_LOWER_BEAM_D& data);
	BOOL CheckData_BEAR(const T_SIGR_LOWER_BEAR_D& data, BOOL bCheckName = TRUE);
	BOOL CheckData_AddFOOT(const T_SIGR_LOWER_FOOT_D& data);
	BOOL IsContainString(const CString& name, const CString& gridName);

	void InsertItem_COLM(const int& nIndex, IN T_SIGR_LOWER_COLM_D& data);
	void InsertItem_BEAM(const int& nIndex, IN T_SIGR_LOWER_BEAM_D& data);
	void InsertItem_BEAR(const int& nIndex, IN T_SIGR_LOWER_BEAR_D& data);
	void InsertItem_FOOT(const int& nIndex, IN T_SIGR_LOWER_FOOT_D& data);

private:
	CDBDoc* m_pDoc;
	int				 m_nOption;			//옵션 라디오 버튼
	int				m_nAutoRdo;		// 자동 할당 버튼
	MEdit			 m_name;			//명칭
	CSingleSelectEdit m_nodeAxis;		//상부구조 관성력 작용 절점위치(교축방향)
	CSingleSelectEdit m_nodePerp;		//상부구조 관성력 작용 절점위치(교축직각방향)
	MEdit			 m_memberName;		//각 부재의 명칭
	CSelectEditColor m_membersElem;			//각 부재의 절점(지승)이나 요소(기둥,보,기초)
	CSingleSelectEdit m_membersNode;	//각 부재의 절점(지승)
	MComboBox		 m_shearAxisDir;	//교축방향 전단력
	MComboBox		 m_bearingLine;		//지승선 콤보박스
	MComboBox		 m_bearingType;		//지승조건 콤보박스
	BOOL			 m_rebarCut;		//철근 단락부 체크박스
	CSingleSelectEdit m_axisElem;		//교축방향 철근 단락부 요소
	CSingleSelectEdit m_perAxisElem;		//교축직각방향 철근 단락부 요소
	MComboBox		 m_axisEndRle;		//교축방향 철근 단락부 단부
	MComboBox		 m_perAxisEndRle;	//교축직각방향 철근 단락부 단부
	CSeisCvlJpSetTargetDlg* m_pParent;
	
	CArray<UINT, UINT> m_aBearingCtrl;
	CArray<UINT, UINT> m_aColumnCtrl;
	CArray<UINT, UINT> m_aBottunCtrl;
	CArray<UINT, UINT> m_aBearingListCtrl;
	CArray<UINT, UINT> m_aColumnListCtrl;
	CArray<UINT, UINT> m_aBeamListCtrl;
	CArray<UINT, UINT> m_aFootingListCtrl;
	CArray<UINT, UINT> m_aMemberNodeCtrl;
	CArray<UINT, UINT> m_aMemberElemCtrl;
	CArray<UINT, UINT> m_aAutoCtrl;

	// Construction
public:
	CSeisCvlJpSetTargetLowerDlg(CWnd* pParent = NULL);   // standard constructor
	~CSeisCvlJpSetTargetLowerDlg();

	// Dialog Data
		//{{AFX_DATA(CSeisCvlJpSetTargetLowerDlg)
	enum { IDD = IDD_TM_SIGR_LOWER };

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCvlJpSetTargetLowerDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void Execute();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSeisCvlJpSetTargetLowerDlg)
	afx_msg void OnSelectedChangedMemberTypeCmb();
	afx_msg void OnSelectedChangedMaterialTypeCmb();
	afx_msg void OnSetFocusNodeAxisEdt();
	afx_msg void OnSetFocusNodePerpEdt();
	afx_msg void OnSetFocusNodeAndElemEdt();
	afx_msg void OnSetFocusAxisEdt();
	afx_msg void OnSetFocusPerAxisEdt();
	afx_msg void OnBnClickedCheck();
	afx_msg void OnNMClickBearingList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickColumnList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickBeamList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickFootingList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickSigrLowerList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddBtn();
	afx_msg void OnModBtn();
	afx_msg void OnDelBtn();
	afx_msg void OnChangeAutoRdo();
	afx_msg void OnClickAutoDlg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//OnInitDialog() 관련함수
	void MoveBearingCtrl();
	void InitControl();
	void ResetAllControls();
	void ResetViewControls();
	void ResetMemberControls();
	void ResetMemberListControls();
	void ResetAutoMemberControls();
	void InitListControls();
	void SetHeaderTitleBearingListCtrl();
	void SetHeaderTitleColumnListCtrl();
	void SetHeaderTitleBeamListCtrl();
	void SetHeaderTitleFootingListCtrl();
	void SetHeaderTitleListCtrl();
	BOOL InsertItemSigrLowerListCtrl(const T_SIGR_LOWER_K& Key, T_SIGR_LOWER_D& Data);
	CString DataToStr_BEAR(const int& index, const T_SIGR_LOWER_BEAR_D& data);
	CString DataToStr_COLM(const int& index, const T_SIGR_LOWER_COLM_D& data);
	CString DataToStr_BEAM(const int& index, const T_SIGR_LOWER_BEAM_D& data);
	CString DataToStr_FOOT(const int& index, const T_SIGR_LOWER_FOOT_D& data);
	CString DataToStr(const int& index, const T_SIGR_LOWER_D& data);

	void UpdateControlByMemberType();
	void UpdateControlMemberNode();
	void UpdateControlMemberElem();
	void UpdateControlSelectedBearing();
	void UpdateControlSelectedColumn();
	void UpdateControlSelectedBeam();
	void UpdateControlSelectedFooting();
	void MoveY_MemberTypeListCtrl(const int& nDistY);
	void SetMemberCmbConnectType();

	BOOL AddBtnBearing();
	void DlgToData_BEAR(T_SIGR_LOWER_BEAR_D& rData);
	
	BOOL AddBtnColumn();
	void DlgToData_COLM(T_SIGR_LOWER_COLM_D& rData);
	BOOL CheckData_ModCOLM(const T_SIGR_LOWER_COLM_D& data);

	BOOL AddBtnBeam();
	void DlgToData_BEAM(T_SIGR_LOWER_BEAM_D& rData);
	BOOL CheckData_ModBEAM(const T_SIGR_LOWER_BEAM_D& data);

	BOOL AddBtnFooting();
	void DlgToData_FOOT(T_SIGR_LOWER_FOOT_D& rData);
	BOOL CheckData_ModFOOT(const T_SIGR_LOWER_FOOT_D& data);

	BOOL IsDuplicatedMemberName(const CString& memberName);
	BOOL IsDuplicatedAtMemberElem(const T_KEY_LIST& elems, CString myName=_T(""));
	BOOL IsSortedLocalAxis(const T_KEY_LIST& elems);

	void ModBtnBearing();
	void ModBtnColumn();
	void ModBtnBeam();
	void ModBtnFooting();
	void DelBtnBearing();
	void DelBtnColumn();
	void DelBtnBeam();
	void DelBtnFooting();

	void UpdataDialog_BEAR(const int& nItem);
	void UpdataDialog_COLM(const int& nItem);
	void UpdataDialog_BEAM(const int& nItem);
	void UpdataDialog_FOOT(const int& nItem);

	//Execute() 관련함수
	BOOL AddData();
	BOOL ModData(const T_SIGR_LOWER_K& key);
	BOOL DelData(const T_SIGR_LOWER_K& key);
	BOOL DlgToData(OUT T_SIGR_D& rSigrD, OUT T_SIGR_LOWER_D& rLowerD);
	void ListToData_BEAR(CArray<T_SIGR_LOWER_BEAR_D, T_SIGR_LOWER_BEAR_D&>& rData);
	void ListToData_COLM(CArray<T_SIGR_LOWER_COLM_D, T_SIGR_LOWER_COLM_D&>& rData);
	void ListToData_COLM_RebarCut(const CString& strGirdText, OUT T_SIGR_LOWER_COLM_D& rData);
	void ListToData_BEAM(CArray<T_SIGR_LOWER_BEAM_D, T_SIGR_LOWER_BEAM_D&>& rData);
	void ListToData_FOOT(CArray<T_SIGR_LOWER_FOOT_D, T_SIGR_LOWER_FOOT_D&>& rData);
	BOOL CheckData(IN T_SIGR_D& sigrD, const T_SIGR_LOWER_D& lowerD, BOOL bCheckName=TRUE);

	//OnUpdate() 관련함수
	void UpdateBuffer();
	BOOL InsertItem(const T_SIGR_LOWER_K& Key, const T_SIGR_LOWER_D& Data);
	BOOL DeleteItem(const T_SIGR_LOWER_K& Key, const T_SIGR_LOWER_D& Data);
	BOOL ModifyItem(const T_SIGR_LOWER_K& Key, const T_SIGR_LOWER_D& Data);

	void UpdateDialog(IN T_SIGR_LOWER_D& data);
	void ClearSelectList(CListCtrl* pList);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEIS_CVL_JP_SET_TARGET_LOWER_DLG_H__)