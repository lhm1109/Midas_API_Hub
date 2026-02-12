
#pragma once
#include "AutoReport_Gen_CH.h"
#include "ReportTreeText.h"

#include "AutoReport_Gen_Mgr.h"
#include "..\wg_db\Db_st_post.h"
struct GenSegmentElements;
struct CH_GEN_IFTAG_STR;
class CTreeReportCtrl;
struct T_TABLE_D;
struct Report_Para_CH;

#include <map>
using namespace std;
class CEngineBase;
class I_GPSModel;
class CRCSCodeCheck;
class  AutoReport_Gen_CH_SUBWAY :public AutoReport_Gen_Base
{
protected:
	virtual void InsertCoverPage();
	virtual void InsertCatalogPage();
	virtual void MakeContent();
	virtual  int TempletID();
	virtual BOOL PreMakeData();
	virtual void MakeTable_CH(GenSegmentElements &genElements, T_TABLE_D &TableD, LPCTSTR lpCaption, LPCTSTR lpBookMark = nullptr);
private:
	void WriteCharpt1();
	void WriteCharpt2();
	void WriteCharpt3();
	void WriteCharpt4();
	void WriteCharpt4_1(GenSegmentElements &Elems);
	void WriteCharpt4_3(GenSegmentElements &Elems);
	//
	void WriteCharpt5();
	void WriteCharpt5_1(GenSegmentElements &Elems);
	void WriteCharpt6();
	void WriteCharpt6_Force(GenSegmentElements &Elems);
	void WriteCharpt6_Dgn(GenSegmentElements &Elems);
	void WriteCharpt6_Dgn_TableMaxElemInSbdo(CArray<CStringArray, CStringArray&>& recond, const T_SBDO_D& SbdoD, const T_ELEM_K_LIST& aSbdoElem, int nIndex, const CString& strLoad, int nDir, CRCSCodeCheck* pCodeCheck);
	void WriteCharpt6_Serv(GenSegmentElements &Elems);
	BOOL WriteCharpt6_Serv_TableMaxElemInSbdo(CArray<CStringArray, CStringArray&>& recond, const T_SBDO_D& SbdoD, const T_ELEM_K_LIST& aSbdoElem, int nIndex, const CString& strLoad, int nDir, CRCSCodeCheck* pCodeCheck);	
private:
	Report_Para_CH* Para() const;
	void setTableFormatString(int nlocation = 0, int ncolor = 16777215);
private:
	std::vector<GenSegmentElements> m_Segments;
	CString m_strFormat;
};