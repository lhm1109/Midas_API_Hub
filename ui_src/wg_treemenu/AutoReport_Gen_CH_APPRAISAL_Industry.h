
#pragma once
#include "AutoReport_Gen_CH.h"
#include "ReportTreeText.h"

#include "AutoReport_Gen_Mgr.h"
#include "..\wg_db\Db_st_post.h"
#include "..\wg_db\ReinforceFileMgrCH.h"

struct GenSegmentElements;
struct CH_GEN_IFTAG_STR;
class CTreeReportCtrl;
struct T_TABLE_D;
struct Report_Para_CH;

#include <map>
using namespace std;

class CEngineBase;
class I_GPSModel;
class  AutoReport_Gen_CH_APPRAISAL_Industry :public AutoReport_Gen_Base
{
protected:
	virtual void InsertCoverPage();
	virtual void InsertCatalogPage();
	virtual void MakeContent();
	virtual  int TempletID();
	virtual BOOL PreMakeData();
private:
	void WriteCharpt1();
	//
	void WriteCharpt2();
	void WriteCharpt2_1(GenSegmentElements& Elems);
	void WriteCharpt2_2(GenSegmentElements& Elems);
	//
	void WriteCharpt3();
	void WriteCharpt3_1(GenSegmentElements& Elems);
	void WriteCharpt3_2(GenSegmentElements& Elems);
	//
	void WriteCharpt4();
	void WriteCharpt4_1(GenSegmentElements& Elems);
	void WriteCharpt4_2(GenSegmentElements& Elems);
	void WriteCharpt4_3(GenSegmentElements& Elems);

	void WriteCharpt5();
	void WriteCharpt6();

	int SetStlInfo(int key, const T_MATD_D& data, CArray<CStringArray, CStringArray&>& strArray, int& nIndex);
	void MakeStlMatlInfo();
	void setTableFormatString(int nlocation = 0, int ncolor = 16777215);
	int SetRltInfo(int key, CArray<CStringArray, CStringArray&>& strArray, int& nIndex);
	BOOL GetStlList();
	BOOL GetCFSList();
	BOOL GetTypeList();
private:
	Report_Para_CH* Para() const;
	CString MakePicture_1_1();
private:
	CStlReinforceDesignMgr* m_pDesignMgr;

private:
	std::vector<GenSegmentElements> m_Segments;
	std::map<std::pair<CString, CString>, vector<wstring>> m_mapstlInfo;
	CString m_strFormat;
	FILE* m_rfp;
	fpos_t	m_fpos;
	int m_nReinCount;
	T_KEY_LIST m_lstBeam_Dsas, m_lstCol_Dsas, m_lstBrace_Dsas, m_lstTruss_Dsas;
	T_KEY_LIST m_lstBeamCFS_Dsas, m_lstColCFS_Dsas, m_lstBraceCFS_Dsas, m_lstTrussCFS_Dsas;
	T_KEY_LIST m_lstBeam_Dsau, m_lstCol_Dsau, m_lstBrace_Dsau, m_lstTruss_Dsau;
	T_KEY_LIST m_lstBeamCFS_Dsau, m_lstColCFS_Dsau, m_lstBraceCFS_Dsau, m_lstTrussCFS_Dsau;
	T_KEY_LIST m_lstBeam, m_lstCol, m_lstBrace, m_lstTruss;
	T_KEY_LIST m_lstBeamCFS, m_lstColCFS, m_lstBraceCFS, m_lstTrussCFS;
	T_KEY_LIST m_lstTypeC, m_lstTypeD;
};