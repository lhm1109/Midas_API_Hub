
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
struct SeisLayerShear
{
	CString strModle;
	CString strStor;
	double dShear;
	void Initialize()
	{
		strModle = _T("");
		strStor = _T("");
		dShear = 0.;
	}
};
class CEngineBase;
class I_GPSModel;
class  AutoReport_Gen_CH_ISGB :public AutoReport_Gen_Base
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
	void WriteCharpt2  ();
	void WriteCharpt2_1(GenSegmentElements &Elems);
	void WriteCharpt2_2(GenSegmentElements &Elems);
	//
	void WriteCharpt3  ();
	void WriteCharpt3_1(GenSegmentElements &Elems);
	void WriteCharpt3_2(GenSegmentElements &Elems);
	//
	void WriteCharpt4();
	void WriteCharpt4_1(GenSegmentElements &Elems);
	void WriteCharpt4_2(GenSegmentElements &Elems);
	//
	void WriteCharpt5();
	void WriteCharpt5_1(GenSegmentElements &Elems);
	void WriteCharpt5_2(GenSegmentElements &Elems);
	void WriteCharpt5_3(GenSegmentElements &Elems);
	//
	void WriteCharpt6();
	void WriteCharpt6_1(GenSegmentElements &Elems);
	void WriteCharpt6_2(GenSegmentElements &Elems);
	void WriteCharpt6_3(GenSegmentElements &Elems);
	//
	void WriteCharpt7();
	void WriteCharpt7_1(GenSegmentElements &Elems);
	void WriteCharpt7_2(GenSegmentElements &Elems);
	void WriteCharpt7_2_1(GenSegmentElements &Elems);
	void WriteCharpt7_2_2(GenSegmentElements &Elems);
	void WriteCharpt7_2_3(GenSegmentElements &Elems);
	void WriteCharpt7_3(GenSegmentElements &Elems);
	void WriteCharpt7_3_1(GenSegmentElements &Elems);
	//void WriteCharpt7_3_2(GenSegmentElements &Elems);
	//
	void WriteCharpt8();
	void WriteCharpt8_1(GenSegmentElements &Elems);
	void WriteCharpt8_2(GenSegmentElements &Elems);

	void FigureRltGM(BOOL bAnimation);
	void WriteForceAndDispRlt(GenSegmentElements &Elems,I_GPSModel* gm,const Report_Key_LIST& aKeys,const CString& strBook);
	void WriteModelRlt(GenSegmentElements &Elems,I_GPSModel* gm,const Report_Key_LIST& aKeys,Report_Key_LIST& aGroupKeys,int& nIndex);
	//void MakeDesignGraph(const T_KEY_LIST& lstElems,T_LCOM_MAXMIN_K LcomK,const CString& strPicName,int nLoadIndex = 5);//0-5 强度 剪切-y 剪切-z 稳定-y 稳定-z 组合
private:
	Report_Para_CH* Para() const;
	CString MakePicture_1_1(); 
	double GetFabsMax(double v1, double v2);
private:
	double CalcDampingRto(T_NLNK_K NlnkK, double dDamp1, double dDamp2);
	BOOL OpenSSFCFile(CString fName);
	BOOL ReadSSFCData(CString strRSLoadName, CArray<SeisLayerShear, SeisLayerShear&> &arShearX, CArray<SeisLayerShear, SeisLayerShear&> &arShearY);
	int lex(FILE *fp, char *yytext, CStringArray& aString);
	BOOL GetDataLine(char *str, CString &dataLine, BOOL bData = FALSE);
	BOOL ReadNonlistValueFromDataLine(CString dataLine, CArray<CString, CString> &nlValue);
	BOOL IsBlankLine(CString dataLine);
	BOOL IsSingleLine(CString dataLine);
	BOOL GetConnectStringBySlush(CString &dataLine);
	BOOL DevideLine(CString dataLine, CArray<CString, CString> &nlValue);
	BOOL ErrMsg(CString sLine, CString cmt);

private:
	std::vector<GenSegmentElements> m_Segments;
	std::map<std::pair<CString,CString>,vector<string>> m_mapstlInfo;
	CString m_strFormat;
	T_ISGB_D m_IsgbD;
	FILE *m_rfp;
	fpos_t	m_fpos;
};