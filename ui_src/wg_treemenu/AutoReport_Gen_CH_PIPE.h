
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
class  AutoReport_Gen_CH_PIPE :public AutoReport_Gen_Base
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
	void WriteCharpt5();
	//
	void WriteCharpt5_1(GenSegmentElements &Elems);
	void WriteCharpt5_2(GenSegmentElements &Elems);
	//
	void WriteCharpt6();
	void WriteCharpt7();
	void WriteCharpt7_1(GenSegmentElements &Elems);
	void WriteCharpt7_2(GenSegmentElements &Elems);
	void WriteCharpt8();
	void WriteCharpt8_1(GenSegmentElements &Elems);
	void WriteCharpt8_2(GenSegmentElements &Elems);
	void WriteCharpt9();
	void WriteCharpt9_3(GenSegmentElements &Elems);

	void WriteCharpt9_3_Dgn(GenSegmentElements &Elems);
	void WriteCharpt9_3_Dgn_TableMaxElemInSbdo(CArray<CStringArray, CStringArray&>& recond, const T_SBDO_D& SbdoD, const T_ELEM_K_LIST& aSbdoElem, int nIndex, const CString& strLoad, int nDir, CRCSCodeCheck* pCodeCheck);
	void WriteCharpt9_3_Serv(GenSegmentElements &Elems);
	BOOL WriteCharpt9_3_Serv_TableMaxElemInSbdo(CArray<CStringArray, CStringArray&>& recond, const T_SBDO_D& SbdoD, const T_ELEM_K_LIST& aSbdoElem, int nIndex, const CString& strLoad, int nDir, CRCSCodeCheck* pCodeCheck);
private:
	Report_Para_CH* Para() const;
	void setTableFormatString(int nlocation = 0, int ncolor = 16777215);
private:
	std::vector<GenSegmentElements> m_Segments;
	CString m_strFormat;
};