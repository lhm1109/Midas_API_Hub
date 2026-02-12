
#pragma once
#include "AutoReport_Gen_CH.h"
#include "ReportTreeText.h"

struct GenSegmentElements;
struct CH_GEN_IFTAG_STR;
class CTreeReportCtrl;
struct T_TABLE_D;
struct Report_Para_CH;

#include <map>
using namespace std;

//´¨µç
class  AutoReport_Gen_CD :public AutoReport_Gen_Base
{
protected:
		virtual void MakeContent(); 
		virtual  int TempletID();
		virtual BOOL PreMakeData();
private:
		void WriteCharpt1();
		void WriteCharpt1_1(GenSegmentElements &Elems);
		void WriteCharpt1_2(GenSegmentElements &Elems);
		void WriteCharpt1_3(GenSegmentElements &Elems);
		void WriteCharpt1_4(GenSegmentElements &Elems);
		void WriteCharpt1_5(GenSegmentElements &Elems);
		void WriteCharpt1_6(GenSegmentElements &Elems);
		//
		void WriteCharpt2  ();
		void WriteCharpt2_1(GenSegmentElements &Elems);
		void WriteCharpt2_2(GenSegmentElements &Elems);
		void WriteCharpt2_3(GenSegmentElements &Elems);
		void WriteCharpt2_4(GenSegmentElements &Elems);
		void WriteCharpt2_5(GenSegmentElements &Elems);
		//
		void WriteCharpt3  ();
		void WriteCharpt3_1(GenSegmentElements &Elems);
		void WriteCharpt3_2(GenSegmentElements &Elems);
		void WriteCharpt3_3(GenSegmentElements &Elems);
		void WriteCharpt3_4(GenSegmentElements &Elems);
		void WriteCharpt3_5(GenSegmentElements &Elems);
		void WriteCharpt3_6(GenSegmentElements &Elems);
		void WriteCharpt3_7(GenSegmentElements &Elems);
		//
		void WriteCharpt4  ();
		void WriteCharpt4_1(GenSegmentElements &Elems);
		void WriteCharpt4_2(GenSegmentElements &Elems);
		void WriteCharpt4_3(GenSegmentElements &Elems);
		void WriteCharpt4_4(GenSegmentElements &Elems);
		void WriteCharpt4_5(GenSegmentElements &Elems);
private:
		Report_Para_CH* Para() const;
		void GetCodeInfo(CString &strCodeMatl, CString &strCodeWind, CString &strCodeQuake);
		CString MakePicture_1_1(); 
		void GetStoryDrift(CString aVal[]);
		void ShowLoad         (const Report_Key &CaseK, BOOL bShow);
		void MakeTBLRecord_4_2(int nDir, T_TABLE_D &TableD);
		void CloseCurDlgbar   ();
		BOOL CallRebarRltFig  ();
		BOOL CallBeamForceFig (const Report_Key &CaseK, int nComp);
		void MakePic_4_3_Flor (GenSegmentElements &Elems, int &nIdx, LPCTSTR strCaseName);
		void MakePic_4_3_Grup (GenSegmentElements &Elems, int &nIdx, LPCTSTR strCaseName);
private:
		std::vector<GenSegmentElements> m_Segments;
};