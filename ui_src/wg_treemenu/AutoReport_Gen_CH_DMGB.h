
#pragma once
#include "AutoReport_Gen_CH.h"
#include "ReportTreeText.h"

#include "AutoReport_Gen_Mgr.h"
#include "..\wg_db\Db_st_post.h"

struct _Disp_HorAcce
{
	double dDisp[2];
	void Initialize()
	{
		dDisp[0] = dDisp[1] = 0.0;
	}
};

struct GenSegmentElements;
struct CH_GEN_IFTAG_STR;
class CTreeReportCtrl;
struct T_TABLE_D;
struct Report_Para_CH;

#include <map>
using namespace std;

class CEngineBase;
class I_GPSModel;
class  AutoReport_Gen_CH_DMGB :public AutoReport_Gen_Base
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
	//
	void WriteCharpt3  ();
	void WriteCharpt3_1(GenSegmentElements &Elems);
	void WriteCharpt3_2(GenSegmentElements &Elems);
	void WriteCharpt3_3(GenSegmentElements &Elems);
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
	void WriteCharpt6_4(GenSegmentElements &Elems);
	void WriteCharpt6_1_1(GenSegmentElements& Elems);
	void WriteCharpt6_1_2(GenSegmentElements& Elems);
	void WriteAdditionalDamperRto_6_1(GenSegmentElements& Elems, T_KEY_LIST& lstThisCase, BOOL bXDir, BOOL bChapOne);
	void CalcCharpt6_4_DampFloorHorAcce(UINT ThisK, _Disp_HorAcce &Disp, _Disp_HorAcce &CenDisp);
	void WriteAdditionalDamperRto();
	void WriteCharptAdditionalDamperRtoTable(GenSegmentElements& genElements, UINT ThisK, T_KEY_LIST& lstKey, BOOL bXDir, int nIndex);
	//
	void WriteCharpt7();
	void WriteCharpt7_1(GenSegmentElements &Elems);
	void WriteCharpt7_2(GenSegmentElements &Elems);
	
	void FigureRltGM(BOOL bAnimation);
	void WriteForceAndDispRlt(GenSegmentElements &Elems,I_GPSModel* gm,const Report_Key_LIST& aKeys,const CString& strBook);
	void WriteModelRlt(GenSegmentElements &Elems,I_GPSModel* gm,const Report_Key_LIST& aKeys,Report_Key_LIST& aGroupKeys,int& nIndex);
	//void MakeDesignGraph(const T_KEY_LIST& lstElems,T_LCOM_MAXMIN_K LcomK,const CString& strPicName,int nLoadIndex = 5);//0-5 强度 剪切-y 剪切-z 稳定-y 稳定-z 组合
private:
	Report_Para_CH* Para() const;
	CString MakePicture_1_1(); 
	double GetFabsMax(double v1, double v2);
private:
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
	T_DAGB_D m_DagbD;
	FILE *m_rfp;
	fpos_t	m_fpos;
	double m_dKesi;
	double m_dEta;
};