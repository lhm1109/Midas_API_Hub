#pragma once

/////////////////////////////////////////////////////////////////////////////
// CStldPresQueryDlg dialog
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxLdgr.h"
#include "TreeNumDetTargetDlg.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#define		COLOR_TABLE_NUM		4
#define		CONTOUR_RANK_NUM	30
class AutoAssignColor
{
public:
	AutoAssignColor();
	void InitColorTable();
	void MakeRankMap(CArray<double>& aValule);
	COLORREF GetColor(double val);
private:
	COLORREF GetSubColor(COLORREF col1, COLORREF col2, double div_num, double order);
	double m_MinRange;
	double m_MaxRange;
	int	m_CurrentRankNum;
	//CArray<double> m_RankMap;
	CArray<double> m_Value;
	COLORREF	m_RGBColor[COLOR_TABLE_NUM][CONTOUR_RANK_NUM+1];
	double		m_RankMap[CONTOUR_RANK_NUM];
	int m_CurrentColorTable;
};

class CStldPresQueryDlg : public CMenuBarChildDlg,public CDBUpdateConnector
{
public:
	//void InitCtrls();
	CStldPresQueryDlg(CWnd* pParent = NULL);   // standard constructor
	~CStldPresQueryDlg();
	virtual void Execute() { OnTmExecute(); }
	enum { IDD = IDD_TM_STLD_PRES_QUERY };
	CListCtrl		m_List;
	CCobxLdgr		m_CobxLdgr;
	//CSelectLC		m_CobxLC;
	MComboBox		m_CobxLC;//Need Custom Std
	MComboBox		m_CobxPslt;
	int   m_nInputType;  // 0:Load Type, 1:Load case
	static int CompareStlCaseColorValue(const void* a,const void* b);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
// Implementation
protected:
	virtual BOOL OnInitDialog();
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	afx_msg void OnSelchangeTmLType();
	afx_msg void OnTmDefineLdgrBtn();
	afx_msg void OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTmClose();
	afx_msg void OnTmExecute();
	afx_msg void OnShowNumDetDlg();
	afx_msg void ChangeTypeRadio();
	afx_msg void ChangeLoadCase();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString DataToStr_LP(int i, T_PSLT_K Key, T_PSLT_D &Data);
	//CString DataToStr_LC(int i, T_FLUE_K Key, T_FLUE_D &Data);
	void InitHeaderTitle();
	void UpdataListData();
	void UpdataLCase();
	void UpdataLCaseColor();
	void SetCoboxLType();
	void SetCoboxLCase();
	void RemoveLColor();
	void UpdateListCtrl();
	void SetLabelMgrLPData();
	void SetLabelMgrLCData();
	void UpdateBuffer();
	//void GetElemByLoadCase(CMap<T_ELEM_K,T_ELEM_K,T_PSLT_K,T_PSLT_K>& mapElemLC);
	T_PSLT_K m_CurPsltK;//Judge the PsltK when changing the type of pressure load
	CDBDoc* m_pDoc;
	CMap<int,int,COLORREF,COLORREF> m_mapColor;
	struct StldCaseColorValue
	{
		void init()
		{
			aPsltK.RemoveAll();
			color = RGB(255,255,255);
			value = 0.0;
		}
		//StldCaseColorValue& operator = (const StldCaseColorValue& src)
		//{
		//	aPsltK.Copy(src.aPsltK);
		//	color = src.color;
		//	value = src.value;
		//}
		CArrayEx<T_PSLT_K,T_PSLT_K>  aPsltK;
		COLORREF color;
		double value;
		bool hasPsltK(const T_PSLT_K psltK)const
		{
			for(int i = 0;i < aPsltK.GetSize();i++)
			{
				if(aPsltK[i] == psltK)
					return true;
			}
			return false;
		}
	};
	typedef CArray<StldCaseColorValue,StldCaseColorValue&> D_STLDCOLOR_ARRAY;
	CMap<T_STLD_K,T_STLD_K,D_STLDCOLOR_ARRAY*,D_STLDCOLOR_ARRAY*> m_mapStlColor;


	//
	AutoAssignColor m_CustomColor;
	DlgNumDet		m_NumDetParam;
};

