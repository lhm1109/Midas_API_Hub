
#pragma once

#include <map>
#include <vector>

class AutoReport_Gen_Base;
#include "HeaderPre.h"

enum ReportType
{
	//AutoReport_GEN_CH
	ARGC_STL = 0,	//一般钢结构
	ARGC_POOL,		//水池	
	ARGC_PIPE,		//管廊
	ARGC_SUBWAY,	//地铁
	ARGC_DMGB,		//减震结构
	ARGC_ISGB,		//隔震结构
	ARGC_REIN,		//加固结构
	ARGC_INDUSTRY,	//鉴定工业结构
	ARGC_CIVIL,		//鉴定民用结构
	ChuanDian_Ch = 1000,	//川电
	ARGNC_INVALID = 9999,
};

struct Report_Key
{
	Report_Key(int nKey = 0, int nType = 0,int nSubType = 0)
	{
		m_nKey  = nKey;
		m_nType = nType;
		m_nSubType = nSubType;
	}
	BOOL operator== (const Report_Key& Key)const
	{
		if(m_nKey != Key.m_nKey || m_nType != Key.m_nType || m_nSubType != Key.m_nSubType)
			return FALSE;
		return TRUE;
	}
	int m_nKey;
	int m_nType;
	int m_nSubType;//Add by xuezc For EN_DGN_ENV::LoadType(1 LOADTYPE_MAX; 2 LOADTYPE_MIN;3 LOADTYPE_MIN)
};

typedef CArray<Report_Key, const Report_Key&> Report_Key_LIST;

struct __MY_EXT_CLASS__ Report_Para_CH
{
public:
	Report_Para_CH();
	void Initial();
	BOOL SegmentIsCheck(int nChart, int nSeg)const;
	BOOL HaveSegmentCheck() const;
	int  GroupKeys(Report_Key_LIST &aKeys, int nType = 0)   const;
	//D_LOADCASE_STATIC, D_LOADCASE_SPECTRUM .....
	int  AllCaseKeys(Report_Key_LIST &aKeys) const;
	int  CaseKeysOnly(Report_Key_LIST &aKeys) const;
	int  LoadCaseKeys(UINT nType, Report_Key_LIST &aKeys) const;
	//{D_LCOMTYPE_GENERAL, D_LCOMTYPE_CONCRETE, D_LCOMTYPE_STEEL, D_LCOMTYPE_SRC};
	int  LoadCombKeys(Report_Key_LIST &aKeys) const;
	int  LoadCombKeys(UINT LCombType, Report_Key_LIST &aKeys) const;
	//_T("FX"),_T("FY"),_T("FZ"),_T("MX"),_T("MY"),_T("MZ") 0-5
	int  ForceOption(Report_Key_LIST &aKeys) const;
	int  ListReportOption(Report_Key_LIST &aKeys, ReportType nType = ARGC_STL) /*const*/;
	BOOL IsUseGroup() const;
	void GetAdditionalDampRtoPara(double &dKesi, double &dEta, BOOL &bChk);
	void SetAdditionalDampRtoPara(double dKesi, double dEta, BOOL bChk);
	void GetReinforceStructureType(int& nType);
	void SetReinforceStructureType(int nType);
	//
	BOOL IsIncluThisForce() const;
	void SetThisForce(BOOL bSetForce);
	BOOL IsIncludeThisForceResult() const;
	void SetIncludeThisForceResult(BOOL bIncThisRlt);
	int AddListChkThisCase(const T_KEY_LIST& lstKeys);
	int ListChkThisCase(T_KEY_LIST& lstKeys) const;
	int AddListDoubleSeisDir(const Report_Key_LIST& aKeys);
	int ListDoubleSeisDir(Report_Key_LIST &aKeys) const;
	int SetHysteresisCurveKeys(const Report_Key_LIST &aKeys);
	int ListHysteresisCurveOption(Report_Key_LIST &aKeys);
	BOOL IsSpfc(CDBDoc *pDoc, int &nSeisFi, int &nSiteClass, int &nnSeisDgnCategory) const;
	BOOL GetWindPara(CDBDoc *pDoc, int &nRoughCategory, double &dBasicWindPressure) const;
	CString SetFilePath(CString strPath);
	CString GetFilePath();
	//
	void SetUseGroup(BOOL bUseGroup);
	void CheckSegment(int nChart, int nSeg, BOOL bCheck);
	int  SetLoadCaseKeys(const Report_Key_LIST &aKeys);
	int  SetForceOption(const Report_Key_LIST &aKeys);
	int  SetGroupKeys(const Report_Key_LIST &aKeys, ReportType nType);
	int  AddListReportKeys(const Report_Key_LIST& aKeys, ReportType nType);
	void SetBufferData(int nPos, int nValue);
	int  GetBufferData(int nPos);
private:
	void InitCheckSegment();
protected:
	BOOL m_bUseGroup;
	BOOL m_bIncForce;
	BOOL m_bIncThisRlt;
	CString m_strPath;
	double m_dKesi;
	double m_dEta;
	BOOL m_bDetail;
	int m_nStructureType;
	std::map< int, std::map<int, int> > m_mapSegment;
	Report_Key_LIST m_lstGroups;
	Report_Key_LIST m_lstCaseKeys;
	//BFORC_COMPONENT_FX .......
	Report_Key_LIST m_ForceOption;
	Report_Key_LIST m_ListReport;
	Report_Key_LIST m_ListCurve;
	Report_Key_LIST m_ListSeisDir;
	T_KEY_LIST m_ListThisCase;
	//设置一些数据,任何模板优先使用这些数据.使用前清空即可.
	int				m_nData[10];
private:
	CMap<int, int, int, int> m_mapLcom;
};

//The data in this class should be initialized when the doc is changed by xuezc
class __MY_EXT_CLASS__ AutoReport_Gen_Mgr
{
public:
	static AutoReport_Gen_Mgr* Inst();
	Report_Para_CH* Para();
private:
	AutoReport_Gen_Mgr(ReportType nType = ARGC_STL);
	~AutoReport_Gen_Mgr();
public:
	CString GetTemplate();
	BOOL UpdateReport(int nTemplet = 0);
	BOOL PreMakeData(int nTemplet = 0);
	BOOL AutoGeneration(int nTemplet = 0, BOOL bUpDate = FALSE);
	void ClearReporter();
	BOOL IsUseAutoReport(BOOL bCD = FALSE);
	BOOL HaveSegmentCheck();
	void SetType(ReportType nType){m_nType = nType;}
	ReportType GetType()const {return m_nType;}
	void NewDoc()
	{
		SetType(ARGC_STL);
		Para()->Initial();
	}
private:
	AutoReport_Gen_Base* Creat(int nTemplet);
	ReportType m_nType;
	Report_Para_CH m_Para;
	AutoReport_Gen_Base* m_pReport;
};
#include "HeaderPost.h"