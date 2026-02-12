
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
class  AutoReport_Gen_CH_REINFORCE :public AutoReport_Gen_Base
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
	void WriteCharpt3_3(GenSegmentElements& Elems);
	void WriteCharpt3_4(GenSegmentElements& Elems);
	//
	void WriteCharpt4();
	void WriteCharpt4_1(GenSegmentElements& Elems);
	void WriteCharpt4_2(GenSegmentElements& Elems);
	void WriteCharpt4_3(GenSegmentElements& Elems);
	void WriteCharpt4_4(GenSegmentElements& Elems);

	int SetStlInfo(int key, const T_MATD_D& data, CArray<CStringArray, CStringArray&>& strArray, int& nIndex);
	void MakeStlMatlInfo();
	void setTableFormatString(int nlocation = 0, int ncolor = 16777215);
	void FigureRltGM(BOOL bAnimation);
	void WriteForceAndDispRlt(GenSegmentElements& Elems, I_GPSModel* gm, const Report_Key_LIST& aKeys, const CString& strBook);
	void WriteRlt(GenSegmentElements& Elems, I_GPSModel* gm, const Report_Key_LIST& aKeys, const CString& strBook);
	void WriteModelRlt(GenSegmentElements& Elems, I_GPSModel* gm, const Report_Key_LIST& aKeys, Report_Key_LIST& aGroupKeys, int& nIndex);
	void WriteTable4_3_2(GenSegmentElements& Elems);
	void WriteTable4_4_1(GenSegmentElements& Elems);
	void WriteTable4_4_2(GenSegmentElements& Elems);
	void WriteTable4_4_3(GenSegmentElements& Elems);
	void WriteDesignRlt(GenSegmentElements& Elems, I_GPSModel* gm, const Report_Key_LIST& aKeys, const CString& strBook);
	void WriteCharpt4_DesignRlt(GenSegmentElements& genElements, BOOL bChap4_3);
	void MakeDesignGraph(const T_KEY_LIST& lstElems, T_LCOM_MAXMIN_K LcomK, const CString& strPicName, int nLoadIndex /* = 5 */);
	void WriteDesignGraph(GenSegmentElements& Elems, const Report_Key_LIST& aKey);
	CGraphFuncView* FindGraphView(CRuntimeClass* pViewClass, CDocument* pDoc, CString szWindowTitle);
	CString GetTitle(int idx);
	void GetReinforceAndStlElemDgnList();
private:
	Report_Para_CH* Para() const;
	CString MakePicture_1_1();
private:
	void SaveBitmapToFile(CBitmap* pBitmap, LPCTSTR lpszFileName);
	BOOL MakeImage_Sect(GenSegmentElements& genElements,T_RFST_K rfstK);

private:
	std::vector<GenSegmentElements> m_Segments;
	std::map<std::pair<CString, CString>, vector<wstring>> m_mapstlInfo;
	CString m_strFormat;
	FILE* m_rfp;
	fpos_t	m_fpos;
	int m_nReinCount;
	T_KEY_LIST m_lstRein;
	T_KEY_LIST m_lstStl;
};