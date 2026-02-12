#if !defined(__SEISSIGRLOWERAUTODLG_H__)
#define __SEISSIGRLOWERAUTODLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MComboBox.h"
#include "HeaderPre.h"

using namespace mit::frx;
using namespace std;

enum eMembar {
	kUnknown,
	kColoum,
	kBeam,
	kBearing,
	kFoot
};


class CDBDoc;
class CSeisCvlJpSetTargetLowerDlg;
class  CAutoFilterMember
{
public:
	CAutoFilterMember();  // 이 클래스는 싱글톤으로써 사용된다. 절대 외부에서 생성 할수 없게 Protected로 막아줌..
	virtual ~CAutoFilterMember();


public:
	// Elem Key		-> Column, Beam, Foot (Membar)
	// Node Key[2]	-> Bearing,
	std::map<eMembar, std::vector<T_KEY>> m_mMembar;	// 확정 맴버타입
	std::map<eMembar, std::vector<T_KEY>> m_mPreMembar; // 예비 맴버타입
	void SetExcute(const T_GRUP_K& key, const double& dDgree, const int& nMatType);
private:
	void SetExistMemberType(const T_GRUP_K& key);
	int GetExistMemberType(const T_ELEM_K& ElemK);

	void FindAllMemberType();
	void ExceptElasticElem(const int& nMatType);
	void FindColumnBeam(const eMembar& nMember);
	void FindBearingforElem(const T_ELEM_D& ElemD);
	void FindBearingforNode(const T_NODE_K& NodeK);
	void FindBearing();
	void FindFoot();
	void FindColumn2Foot();
	void MakeOneColumn(const T_ELEM_D& ElemD, const vector<T_ELEM_K>& allColumn, vector<T_ELEM_K>& aColumn);
	void FindFootForBeam(const T_NODE_K& NodeK, vector<T_NODE_K>& aChecked);
	void SetFootElem(const T_NODE_K& NodeK);
	void FindNodeZ2Foot();
	void AddFootForElasticElem();
	T_NODE_K FindLowerNode(const vector<T_ELEM_K>& aColumn);
	T_NODE_K FindHigherNode(const vector<T_ELEM_K>& aColumn);
	void SetConnectMphiElem(const eMembar& nMember, const T_ELEM_D& ElemD, vector<T_KEY>& aVecElemK, vector<T_ELEM_K>& aChecked); // 재귀함수
	BOOL CheckMpaiElem(const T_ELEM_K& ElemK);
	BOOL IsExistMemberType(const eMembar& nMember, const T_KEY& ElemK);
	eMembar CustomCheckFilter(const T_ELEM_D& ElemD, const double& dDgree);
	BOOL IsLowerElem(const T_ELEM_K& ElemK);
	void ExceptElem(const eMembar& nMember, const double& dElastic, const double& dSectAs, const int& nMatType);
	void SortElemForY();

	CDBDoc* m_pDoc;
	double m_dDgree;
};

class __MY_EXT_CLASS__ CSeisSigrLowerAutoDlg : public CChildDialog
{
public:
	CSeisSigrLowerAutoDlg(CSeisCvlJpSetTargetLowerDlg* pParent = NULL);  // 이 클래스는 싱글톤으로써 사용된다. 절대 외부에서 생성 할수 없게 Protected로 막아줌..
	virtual ~CSeisSigrLowerAutoDlg();
	enum {
		IDD = IDD_TM_SIGR_LOWER_AUTO_DLG
	};

private:
	MComboBox		 m_Group;		//교각의 부재타입
	MComboBox		 m_shearAxisDir;	//교축방향 전단력
	MComboBox		 m_bearingLine;		//지승선 콤보박스
	MComboBox		 m_bearingType;		//지승조건 콤보박스

	CEditUnit			m_Degree;			//각도
	CTextUnit		m_DegreeUnit;			//각도 단위

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	// Generated message map functions
//{{AFX_MSG(CSeisCvlJpSetTargetLowerDlg)
	afx_msg void OnOk();
	afx_msg void OnChangeGroupCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void InitUnit();
	void InitComboBox();
	void InitEditBox();
	void ResetViewControls();
	void AddData();
	void ConvertDlg2Data(T_SIGR_LOWER_D& SigrLowerD);
	void SetColumn(const int& nDir, T_SIGR_LOWER_D& rData);
	void SetBeam(const int& nDir, T_SIGR_LOWER_D& rData);
	void SetBearing(const int& nDir, T_SIGR_LOWER_D& rData);
	void SetFoot(const int& nDir, T_SIGR_LOWER_D& rData);
	BOOL CheckDlg2Data(const T_SIGR_LOWER_D& SigrLowerD);
	void InsertListItem(const T_SIGR_LOWER_D& SigrLowerD);
	void ViewInsertItem(const T_SIGR_LOWER_D& SigrLowerD);
	CString SetSubGroup(const eMembar& nMembar);
	int SetSubGroupNum(const CString& strSubGroup, const eMembar& nMembar);
	void SetMapColBeamFoot(const eMembar& nMember, std::map <CString, std::vector<T_ELEM_K>>& mArray);
	void SetMapBearing(const int& nBearLine, std::map <int, std::vector<T_NODE_K>>& mBearing);
	double GetMidValue(const int& nPos);

	CDBDoc* m_pDoc;
	CAutoFilterMember* m_pAutoFilter;
	CSeisCvlJpSetTargetLowerDlg* m_pParent;
};

#include "HeaderPost.h"

#endif // !defined(__LOADWORDLIB_H__)
