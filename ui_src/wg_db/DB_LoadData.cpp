// DB_LoadData.cpp: implementation of the CDB_LoadData class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DB_LoadData.h"

#include "DBLib.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

T_LOADDATA_LIST CDB_LoadData::m_aLoadWind;
T_LOADDATA_LIST CDB_LoadData::m_aLoadSnow;

int CDB_LoadData::m_nLoadType;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDB_LoadData::CDB_LoadData()
{
	m_aLoadWind.RemoveAll();  
	m_aLoadSnow.RemoveAll();
	CreateLoadDB();

	m_nLoadType = 0;
}

CDB_LoadData::~CDB_LoadData()
{
}

void CDB_LoadData::CreateLoadDB()
{
	CreateWindLoad();
	CreateSnowLoad();
}

double CDB_LoadData::GetCurrentUnitWindLoad(double dValue)
{
	return CDBLib::ConvertUnitKN_M(dValue, D_UNITSYS_BASE_VELOCITY);
}

double CDB_LoadData::GetCurrentUnitSnowLoad(double dValue)
{
	return CDBLib::ConvertUnitKN_M(dValue, D_UNITSYS_BASE_PRESSURE);
}

void CDB_LoadData::CreateWindLoad()
{
	if (m_aLoadWind.GetSize() > 0) { m_aLoadWind.RemoveAll(); }

	AddLoadData(m_aLoadWind, _T("특별시"), _T("서울"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("광역시"), _T("인천"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("강화"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("웅진"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("김포"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("구리"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("수원"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("군포"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("오산"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("화성"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("안산"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("시흥"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("의왕"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("부천"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("고양"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("평택"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("안성"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("안양"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("과천"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("광명"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("의정부"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("동두천"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("양주"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("파주"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("연천"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("포천"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("남양주"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("가평"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("하남"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("성남"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("광주"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("양평"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("여주"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("이천"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경기도"), _T("용인"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("속초"), GetCurrentUnitWindLoad(40.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("양양"), GetCurrentUnitWindLoad(40.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("강릉"), GetCurrentUnitWindLoad(40.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("고성"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("동해"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("삼척"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("양구"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("철원"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("화천"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("춘천"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("홍천"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("횡성"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("원주"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("평창"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("정선"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("영월"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("인제"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("강원도"), _T("태백"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("서천"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("보령"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("홍성"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("예산"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("서산"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("태안"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("아산"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("천안"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("연기"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("청주"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("청원"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("광역시"), _T("대전"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("계룡"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("진천"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("증평"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("당진"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("청양"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("공주"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("부여"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("논산"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("금산"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("음성"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("충주"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("제천"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("단양"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("괴산"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("보은"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("영동"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("충청도"), _T("옥천"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("포항"), GetCurrentUnitWindLoad(45.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("울릉도"), GetCurrentUnitWindLoad(45.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("독도"), GetCurrentUnitWindLoad(45.00));
	AddLoadData(m_aLoadWind, _T("광역시"), _T("부산"), GetCurrentUnitWindLoad(40.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("기장"), GetCurrentUnitWindLoad(40.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("경주"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("영덕"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("울진"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("양산"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("김해"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("진해"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("창원"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("마산"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("통영"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("거제"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("고성"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("남해"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("사천"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("광역시"), _T("울산"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("울주"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("함안"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("봉화"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("영주"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("예천"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("문경"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("상주"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("추풍령"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("안동"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("영양"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("청송"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("의성"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("군위"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("구미"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("칠곡"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("김천"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("성주"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("고령"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("광역시"), _T("대구"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("달성"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("경산"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("영천"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("청도"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("창녕"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("의령"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("진주"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("거창"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("산청"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("밀양"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("합천"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("함양"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("경상도"), _T("하동"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("군산"), GetCurrentUnitWindLoad(40.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("익산"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("완도"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("해남"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("진도"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("목포"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("여수"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("고흥"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("신안"), GetCurrentUnitWindLoad(35.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("김제"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("순천"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("영광"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("함평"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("광역시"), _T("광주"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("화순"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("나주"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("무안"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("영암"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("강진"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("장흥"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("보성"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("광양"), GetCurrentUnitWindLoad(30.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("완주"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("무주"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("전주"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("진안"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("장수"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("임실"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("정읍"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("고창"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("순창"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("남원"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("장성"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("담양"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("곡성"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("구례"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("전라도"), _T("부안"), GetCurrentUnitWindLoad(25.00));
	AddLoadData(m_aLoadWind, _T("제주도"), _T("서귀포"), GetCurrentUnitWindLoad(40.00));
	AddLoadData(m_aLoadWind, _T("제주도"), _T("제주"), GetCurrentUnitWindLoad(40.00));
	AddLoadData(m_aLoadWind, _T("제주도"), _T("성산포"), GetCurrentUnitWindLoad(40.00));
}

void CDB_LoadData::CreateSnowLoad()
{
	if (m_aLoadSnow.GetSize() > 0) { m_aLoadSnow.RemoveAll(); }

	AddLoadData(m_aLoadSnow, _T("특별시"), _T("서울"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("경기도"), _T("수원"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("강원도"), _T("춘천"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("충청도"), _T("서산"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("충청도"), _T("청주"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("광역시"), _T("대전"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("경상도"), _T("추풍령"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("경상도"), _T("포항"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("전라도"), _T("군산"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("광역시"), _T("대구"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("전라도"), _T("전주"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("광역시"), _T("울산"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("광역시"), _T("광주"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("광역시"), _T("부산"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("경상도"), _T("통영"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("전라도"), _T("목포"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("전라도"), _T("여수"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("제주도"), _T("제주"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("제주도"), _T("서귀포"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("경상도"), _T("진주"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("경기도"), _T("이천"), GetCurrentUnitSnowLoad(0.500));
	AddLoadData(m_aLoadSnow, _T("전라도"), _T("정읍"), GetCurrentUnitSnowLoad(0.650));
	AddLoadData(m_aLoadSnow, _T("경상도"), _T("울진"), GetCurrentUnitSnowLoad(0.650));
	AddLoadData(m_aLoadSnow, _T("광역시"), _T("인천"), GetCurrentUnitSnowLoad(0.800));
	AddLoadData(m_aLoadSnow, _T("강원도"), _T("속초"), GetCurrentUnitSnowLoad(2.000));
	AddLoadData(m_aLoadSnow, _T("강원도"), _T("강릉"), GetCurrentUnitSnowLoad(3.000));
	AddLoadData(m_aLoadSnow, _T("경상도"), _T("울릉도"), GetCurrentUnitSnowLoad(7.000));
	AddLoadData(m_aLoadSnow, _T("강원도"), _T("대관령"), GetCurrentUnitSnowLoad(7.000));
}

void CDB_LoadData::AddLoadData(T_LOADDATA_LIST& aLoadDB, CString csZone, CString csCity, double dValue)
{
	T_LOADDATA_D LoadD;
	LoadD.Initialize();

	LoadD.csZone = csZone;
	LoadD.csCity = csCity;
	LoadD.dValue = dValue;

	aLoadDB.Add(LoadD);
}

void CDB_LoadData::GetZoneListForWindLoad(CArray<CString, CString&>& aString)
{
	m_nLoadType = LOAD_TYPE_WIND_KR;
	CreateLoadDB();
	GetZoneList(aString);
}

void CDB_LoadData::GetZoneListForSnowLoad(CArray<CString, CString&>& aString)
{
	m_nLoadType = LOAD_TYPE_SNOW_KR;
	CreateLoadDB();
	GetZoneList(aString);
}

void CDB_LoadData::GetCityListForWindLoad(CArray<CString, CString&>& aString, CString csZone)
{
	m_nLoadType = LOAD_TYPE_WIND_KR;
	CreateLoadDB();
	GetCityList(aString, csZone);
}

void CDB_LoadData::GetCityListForSnowLoad(CArray<CString, CString&>& aString, CString csZone)
{
	m_nLoadType = LOAD_TYPE_SNOW_KR;
	CreateLoadDB();
	GetCityList(aString, csZone);
}

void CDB_LoadData::GetZoneList(CArray<CString, CString&>& aString)
{
	aString.RemoveAll();

	BOOL bWind = (m_nLoadType==LOAD_TYPE_WIND_KR) ? TRUE : FALSE;
	T_LOADDATA_LIST& aLoadDB = bWind ? m_aLoadWind : m_aLoadSnow;

	int nLoadDB = aLoadDB.GetSize();
	for (int i=0; i<nLoadDB; i++)
	{
		T_LOADDATA_D& Load = aLoadDB[i];
		if (!IsExist(Load.csZone, aString))
			aString.Add(Load.csZone);
	}
}

void CDB_LoadData::GetCityList(CArray<CString, CString&>& aString, CString csZone)
{
	aString.RemoveAll();

	BOOL bWind = (m_nLoadType==LOAD_TYPE_WIND_KR) ? TRUE : FALSE;
	T_LOADDATA_LIST& aLoadDB = bWind ? m_aLoadWind : m_aLoadSnow;

	int nLoadDB = aLoadDB.GetSize();
	for (int i=0; i<nLoadDB; i++)
	{
		T_LOADDATA_D& Load = aLoadDB[i];
		if (Load.csZone.CompareNoCase(csZone)==0)
		{
			if (!IsExist(Load.csCity, aString))
				aString.Add(Load.csCity);
		}
	}
}

void CDB_LoadData::GetImportanceListForWindLoad(CArray<CString, CString&>& aString)
{
	GetImportanceList(aString);
}

void CDB_LoadData::GetImportanceListForSnowLoad(CArray<CString, CString&>& aString)
{
	GetImportanceList(aString);
}

void CDB_LoadData::GetImportanceListForSnowLoadUS(CArray<CString, CString&>& aString)
{
	GetImportanceListUS(aString);
}

void CDB_LoadData::GetImportanceList(CArray<CString, CString&>& aString)
{
	CString csTemp;
	aString.RemoveAll();
	csTemp = BUILDING_IMPORTANT_CATEGORY_S; aString.Add(csTemp);
	csTemp = BUILDING_IMPORTANT_CATEGORY_1; aString.Add(csTemp);
	csTemp = BUILDING_IMPORTANT_CATEGORY_2; aString.Add(csTemp);
	csTemp = BUILDING_IMPORTANT_CATEGORY_3; aString.Add(csTemp);
}

void CDB_LoadData::GetImportanceListUS(CArray<CString, CString&>& aString)
{
	CString csTemp;
	aString.RemoveAll();
	aString.Add((CString)BUILDING_IMPORTANT_CATEGORY_I);
	aString.Add((CString)BUILDING_IMPORTANT_CATEGORY_II);
	aString.Add((CString)BUILDING_IMPORTANT_CATEGORY_III);
	aString.Add((CString)BUILDING_IMPORTANT_CATEGORY_IV);
}

void CDB_LoadData::GetTopographicFactor(CArray<CString, CString&>& aString)
{
	CString csTemp;
	aString.RemoveAll();
	csTemp = GetTopographicFactor(1.0); aString.Add(csTemp);
}

void CDB_LoadData::GetSurfaceRoughnessCategory(CArray<CString, CString&>& aString)
{
	CString csTemp;
	aString.RemoveAll();
	csTemp = _T("A"); aString.Add(csTemp);
	csTemp = _T("B"); aString.Add(csTemp);
	csTemp = _T("C"); aString.Add(csTemp);
	csTemp = _T("D"); aString.Add(csTemp);
}

void CDB_LoadData::GetExposureCategory(CArray<CString, CString&>& aString)
{
	CString csTemp;
	aString.RemoveAll();
	csTemp = _T("A"); aString.Add(csTemp);
	csTemp = _T("B"); aString.Add(csTemp);
	csTemp = _T("C"); aString.Add(csTemp);
	csTemp = _T("D"); aString.Add(csTemp);
	csTemp = _T("E"); aString.Add(csTemp);
}

void CDB_LoadData::GetBuildingTypeForWindLoadKR(CArray<CString, CString&>& aString)
{
	CString csTemp;
	aString.RemoveAll();
	csTemp = WIND_LOAD_KR_BUILD_TYPE_CHAR_CLOSE; aString.Add(csTemp);
	csTemp = WIND_LOAD_KR_BUILD_TYPE_CHAR_POPEN; aString.Add(csTemp);
	csTemp = WIND_LOAD_KR_BUILD_TYPE_CHAR_WOPEN; aString.Add(csTemp);
	csTemp = WIND_LOAD_KR_BUILD_TYPE_CHAR_FOPEN; aString.Add(csTemp);
}

void CDB_LoadData::GetBuildingTypeForWindLoadUS(CArray<CString, CString&>& aString)
{
	aString.RemoveAll();
	aString.Add((CString)WIND_LOAD_US_BUILD_TYPE_CHAR_ENCLOSE);
	aString.Add((CString)WIND_LOAD_US_BUILD_TYPE_CHAR_PARTENC);
}

void CDB_LoadData::GetBuildingTypeForSnowLoad(CArray<CString, CString&>& aString)
{
	CString csTemp;
	aString.RemoveAll();
	csTemp = SNOW_LOAD_BUILD_TYPE_CHAR_HEATED; aString.Add(csTemp);
	csTemp = SNOW_LOAD_BUILD_TYPE_CHAR_UNHEAT; aString.Add(csTemp);
}

void CDB_LoadData::GetRoofTypeForWindLoadKR(CArray<CString, CString&>& aString)
{
	CString csTemp;
	aString.RemoveAll();
	csTemp = WIND_LOAD_KR_ROOF_TYPE_CHAR_GABLET; aString.Add(csTemp);
	csTemp = WIND_LOAD_KR_ROOF_TYPE_CHAR_MSLOPE; aString.Add(csTemp);
	csTemp = WIND_LOAD_KR_ROOF_TYPE_CHAR_MGABLE; aString.Add(csTemp);
	csTemp = WIND_LOAD_KR_ROOF_TYPE_CHAR_STOOTH; aString.Add(csTemp);
}

void CDB_LoadData::GetRoofTypeForWindLoadUS(CArray<CString, CString&>& aString)
{
	aString.RemoveAll();
	aString.Add((CString)WIND_LOAD_US_ROOF_TYPE_CHAR_FLAT);
	aString.Add((CString)WIND_LOAD_US_ROOF_TYPE_CHAR_GABLE);
	aString.Add((CString)WIND_LOAD_US_ROOF_TYPE_CHAR_MULTIGABLE);
	aString.Add((CString)WIND_LOAD_US_ROOF_TYPE_CHAR_MONOSLOPE);
	aString.Add((CString)WIND_LOAD_US_ROOF_TYPE_CHAR_SAWTOOTH);
}

void CDB_LoadData::GetRoofTypeForSnowLoadKR(CArray<CString, CString&>& aString)
{
	CString csTemp;
	aString.RemoveAll();
	csTemp = SNOW_LOAD_KR_ROOF_TYPE_CHAR_GABLET; aString.Add(csTemp);
	csTemp = SNOW_LOAD_KR_ROOF_TYPE_CHAR_CURVED; aString.Add(csTemp);
	csTemp = SNOW_LOAD_KR_ROOF_TYPE_CHAR_MGABLE; aString.Add(csTemp);
	csTemp = SNOW_LOAD_KR_ROOF_TYPE_CHAR_CYLIND; aString.Add(csTemp);
	csTemp = SNOW_LOAD_KR_ROOF_TYPE_CHAR_STOOTH; aString.Add(csTemp);
}

void CDB_LoadData::GetRoofTypeForSnowLoadUS(CArray<CString, CString&>& aString)
{
	aString.RemoveAll();
	aString.Add((CString)SNOW_LOAD_US_ROOF_TYPE_CHAR_SLOPE);
	aString.Add((CString)SNOW_LOAD_US_ROOF_TYPE_CHAR_GABLE);
	aString.Add((CString)SNOW_LOAD_US_ROOF_TYPE_CHAR_CURVED);
	aString.Add((CString)SNOW_LOAD_US_ROOF_TYPE_CHAR_MULTIFOLD);
	aString.Add((CString)SNOW_LOAD_US_ROOF_TYPE_CHAR_BARREL);
	aString.Add((CString)SNOW_LOAD_US_ROOF_TYPE_CHAR_SAWTOOTH);	
}

void CDB_LoadData::GetCheckPoint(CArray<CString, CString&>& aString, UINT nMembType, UINT nRoofType, double dRoofHeight, double dRoofAngle)
{
	CString csTemp;
	aString.RemoveAll();

	if (nMembType == 1) // Girt
	{
		csTemp = WIND_LOAD_CHECK_CHAR_4; aString.Add(csTemp);
		csTemp = WIND_LOAD_CHECK_CHAR_5; aString.Add(csTemp);
	}
	else // Purlin
	{
		csTemp = WIND_LOAD_CHECK_CHAR_1; aString.Add(csTemp);
		csTemp = WIND_LOAD_CHECK_CHAR_2; aString.Add(csTemp);
		csTemp = WIND_LOAD_CHECK_CHAR_3; aString.Add(csTemp);

		BOOL bFullList = TRUE;
		double d20M = CDBLib::ConvertUnitKN_M(20.0, D_UNITSYS_BASE_LENGTH);
		if (nRoofType  != 1   ) bFullList = FALSE; // Mono-Slope이 아닌 경우
		if (dRoofAngle  > 10.0) bFullList = FALSE; // 10.0도 이상인 경우
		if (dRoofHeight > d20M) bFullList = FALSE; // 평균높이가 20M이상인 경우

		if (bFullList)
		{
			csTemp = WIND_LOAD_CHECK_CHAR_4; aString.Add(csTemp);
			csTemp = WIND_LOAD_CHECK_CHAR_5; aString.Add(csTemp);
		}
	}
}

CString CDB_LoadData::GetBuildingTypeForWindLoadKR(int nBuildType)
{
	CString csBuildType;
	csBuildType.Empty();

	switch (nBuildType)
	{
		case WIND_LOAD_KR_BUILD_TYPE_CASE_CLOSE: csBuildType = WIND_LOAD_KR_BUILD_TYPE_CHAR_CLOSE; break;
		case WIND_LOAD_KR_BUILD_TYPE_CASE_POPEN: csBuildType = WIND_LOAD_KR_BUILD_TYPE_CHAR_POPEN; break;
		case WIND_LOAD_KR_BUILD_TYPE_CASE_WOPEN: csBuildType = WIND_LOAD_KR_BUILD_TYPE_CHAR_WOPEN; break;
		case WIND_LOAD_KR_BUILD_TYPE_CASE_FOPEN: csBuildType = WIND_LOAD_KR_BUILD_TYPE_CHAR_FOPEN; break;
		default: ASSERT(0); break;
	}

	return csBuildType;
}

CString CDB_LoadData::GetBuildingTypeForWindLoadUS(int nBuildType)
{
	CString csBuildType;
	csBuildType.Empty();
	
	switch (nBuildType)
	{
	case WIND_LOAD_US_BUILD_TYPE_CASE_ENCLOSE: csBuildType = WIND_LOAD_US_BUILD_TYPE_CHAR_ENCLOSE; break;
	case WIND_LOAD_US_BUILD_TYPE_CASE_PARTENC: csBuildType = WIND_LOAD_US_BUILD_TYPE_CHAR_PARTENC; break;	
	default: ASSERT(0); break;
	}
	
	return csBuildType;
}

CString CDB_LoadData::GetBuildingTypeForSnowLoad(int nBuildType)
{
	CString csBuildType;
	csBuildType.Empty();

	switch (nBuildType)
	{
		case SNOW_LOAD_BUILD_TYPE_CASE_HEATED: csBuildType = SNOW_LOAD_BUILD_TYPE_CHAR_HEATED; break;
		case SNOW_LOAD_BUILD_TYPE_CASE_UNHEAT: csBuildType = SNOW_LOAD_BUILD_TYPE_CHAR_UNHEAT; break;
		default: ASSERT(0); break;
	}

	return csBuildType;
}

CString CDB_LoadData::GetRoofType(int nRoofType, int nLoadType)
{
	CString csRoofType;
	csRoofType.Empty();

	if (nLoadType == LOAD_TYPE_WIND_KR)
	{
		switch (nRoofType)
		{
			case WIND_LOAD_KR_ROOF_TYPE_CASE_GABLET: csRoofType = WIND_LOAD_KR_ROOF_TYPE_CHAR_GABLET; break;
			case WIND_LOAD_KR_ROOF_TYPE_CASE_MSLOPE: csRoofType = WIND_LOAD_KR_ROOF_TYPE_CHAR_MSLOPE; break;
			case WIND_LOAD_KR_ROOF_TYPE_CASE_MGABLE: csRoofType = WIND_LOAD_KR_ROOF_TYPE_CHAR_MGABLE; break;
			case WIND_LOAD_KR_ROOF_TYPE_CASE_STOOTH: csRoofType = WIND_LOAD_KR_ROOF_TYPE_CHAR_STOOTH; break;
			default: ASSERT(0); break;
		}
	}
	else if (nLoadType == LOAD_TYPE_WIND_US)
	{
		switch (nRoofType)
		{
		case WIND_LOAD_US_ROOF_TYPE_CASE_FLAT      : csRoofType = WIND_LOAD_US_ROOF_TYPE_CHAR_FLAT      ; break;
		case WIND_LOAD_US_ROOF_TYPE_CASE_GABLE     : csRoofType = WIND_LOAD_US_ROOF_TYPE_CHAR_GABLE     ; break;
		case WIND_LOAD_US_ROOF_TYPE_CASE_MULTIGABLE: csRoofType = WIND_LOAD_US_ROOF_TYPE_CHAR_MULTIGABLE; break;
		case WIND_LOAD_US_ROOF_TYPE_CASE_MONOSLOPE : csRoofType = WIND_LOAD_US_ROOF_TYPE_CHAR_MONOSLOPE ; break;
		case WIND_LOAD_US_ROOF_TYPE_CASE_SAWTOOTH  : csRoofType = WIND_LOAD_US_ROOF_TYPE_CHAR_SAWTOOTH  ; break;
		default: ASSERT(0); break;
		}
	}
	else if (nLoadType == LOAD_TYPE_SNOW_KR)
	{
		switch (nRoofType)
		{
		case SNOW_LOAD_KR_ROOF_TYPE_CASE_GABLET: csRoofType = SNOW_LOAD_KR_ROOF_TYPE_CHAR_GABLET; break;
		case SNOW_LOAD_KR_ROOF_TYPE_CASE_CURVED: csRoofType = SNOW_LOAD_KR_ROOF_TYPE_CHAR_CURVED; break;
		case SNOW_LOAD_KR_ROOF_TYPE_CASE_MGABLE: csRoofType = SNOW_LOAD_KR_ROOF_TYPE_CHAR_MGABLE; break;
		case SNOW_LOAD_KR_ROOF_TYPE_CASE_CYLIND: csRoofType = SNOW_LOAD_KR_ROOF_TYPE_CHAR_CYLIND; break;
		case SNOW_LOAD_KR_ROOF_TYPE_CASE_STOOTH: csRoofType = SNOW_LOAD_KR_ROOF_TYPE_CHAR_STOOTH; break;
		default: ASSERT(0); break;
		}
	}
	else if (nLoadType == LOAD_TYPE_SNOW_US)
	{
		switch (nRoofType)
		{
		case SNOW_LOAD_US_ROOF_TYPE_CASE_SLOPE    : csRoofType = SNOW_LOAD_US_ROOF_TYPE_CHAR_SLOPE    ; break;
		case SNOW_LOAD_US_ROOF_TYPE_CASE_GABLE    : csRoofType = SNOW_LOAD_US_ROOF_TYPE_CHAR_GABLE    ; break;
		case SNOW_LOAD_US_ROOF_TYPE_CASE_CURVED   : csRoofType = SNOW_LOAD_US_ROOF_TYPE_CHAR_CURVED   ; break;
		case SNOW_LOAD_US_ROOF_TYPE_CASE_MULTIFOLD: csRoofType = SNOW_LOAD_US_ROOF_TYPE_CHAR_MULTIFOLD; break;
		case SNOW_LOAD_US_ROOF_TYPE_CASE_BARREL   : csRoofType = SNOW_LOAD_US_ROOF_TYPE_CHAR_BARREL   ; break;
		case SNOW_LOAD_US_ROOF_TYPE_CASE_SAWTOOTH : csRoofType = SNOW_LOAD_US_ROOF_TYPE_CHAR_SAWTOOTH ; break;
		default: ASSERT(0); break;
		}
	}
	else
	{
		ASSERT(0);
	}

	return csRoofType;
}

CString CDB_LoadData::GetCheckPoint(int nCheckPoint)
{
	CString csCheckPoint;
	csCheckPoint.Empty();
 
	switch (nCheckPoint)
	{
		case WIND_LOAD_CHECK_CASE_1: csCheckPoint = WIND_LOAD_CHECK_CHAR_1; break;
		case WIND_LOAD_CHECK_CASE_2: csCheckPoint = WIND_LOAD_CHECK_CHAR_2; break;
		case WIND_LOAD_CHECK_CASE_3: csCheckPoint = WIND_LOAD_CHECK_CHAR_3; break;
		case WIND_LOAD_CHECK_CASE_4: csCheckPoint = WIND_LOAD_CHECK_CHAR_4; break;
		case WIND_LOAD_CHECK_CASE_5: csCheckPoint = WIND_LOAD_CHECK_CHAR_5; break;
		default: ASSERT(0); break;
	}

	return csCheckPoint;
}

CString CDB_LoadData::GetTopographicFactor(double dFactor)
{
	return GetStringForFactor(dFactor, 2);
}

int CDB_LoadData::GetBuildingTypeForWindLoadKR(CString csBuildType)
{
	if (csBuildType == WIND_LOAD_KR_BUILD_TYPE_CHAR_CLOSE) return WIND_LOAD_KR_BUILD_TYPE_CASE_CLOSE;
	if (csBuildType == WIND_LOAD_KR_BUILD_TYPE_CHAR_POPEN) return WIND_LOAD_KR_BUILD_TYPE_CASE_POPEN;
	if (csBuildType == WIND_LOAD_KR_BUILD_TYPE_CHAR_WOPEN) return WIND_LOAD_KR_BUILD_TYPE_CASE_WOPEN;
	if (csBuildType == WIND_LOAD_KR_BUILD_TYPE_CHAR_FOPEN) return WIND_LOAD_KR_BUILD_TYPE_CASE_FOPEN;
	
	ASSERT(0);
	return WIND_LOAD_KR_BUILD_TYPE_CASE_CLOSE;
}

int CDB_LoadData::GetBuildingTypeForWindLoadUS(CString csBuildType)
{
	if (csBuildType == WIND_LOAD_US_BUILD_TYPE_CHAR_ENCLOSE) return WIND_LOAD_US_BUILD_TYPE_CASE_ENCLOSE;
	if (csBuildType == WIND_LOAD_US_BUILD_TYPE_CHAR_PARTENC) return WIND_LOAD_US_BUILD_TYPE_CASE_PARTENC;

	ASSERT(0);
	return WIND_LOAD_US_BUILD_TYPE_CASE_ENCLOSE;
}

int CDB_LoadData::GetBuildingTypeForSnowLoad(CString csBuildType)
{
	if (csBuildType == SNOW_LOAD_BUILD_TYPE_CHAR_HEATED) return SNOW_LOAD_BUILD_TYPE_CASE_HEATED;
	if (csBuildType == SNOW_LOAD_BUILD_TYPE_CHAR_UNHEAT) return SNOW_LOAD_BUILD_TYPE_CASE_UNHEAT;

	ASSERT(0);
	return WIND_LOAD_KR_BUILD_TYPE_CASE_CLOSE;
}

int CDB_LoadData::GetRoofType(CString csRoofType, int nLoadType)
{
	if (nLoadType == LOAD_TYPE_WIND_KR)
	{
		if (csRoofType == WIND_LOAD_KR_ROOF_TYPE_CHAR_GABLET) return WIND_LOAD_KR_ROOF_TYPE_CASE_GABLET;
		if (csRoofType == WIND_LOAD_KR_ROOF_TYPE_CHAR_MSLOPE) return WIND_LOAD_KR_ROOF_TYPE_CASE_MSLOPE;
		if (csRoofType == WIND_LOAD_KR_ROOF_TYPE_CHAR_MGABLE) return WIND_LOAD_KR_ROOF_TYPE_CASE_MGABLE;
		if (csRoofType == WIND_LOAD_KR_ROOF_TYPE_CHAR_STOOTH) return WIND_LOAD_KR_ROOF_TYPE_CASE_STOOTH;
	}
	else if (nLoadType == LOAD_TYPE_WIND_US)
	{
		if (csRoofType == WIND_LOAD_US_ROOF_TYPE_CHAR_FLAT      ) return WIND_LOAD_US_ROOF_TYPE_CASE_FLAT;
		if (csRoofType == WIND_LOAD_US_ROOF_TYPE_CHAR_GABLE     ) return WIND_LOAD_US_ROOF_TYPE_CASE_GABLE;
		if (csRoofType == WIND_LOAD_US_ROOF_TYPE_CHAR_MULTIGABLE) return WIND_LOAD_US_ROOF_TYPE_CASE_MULTIGABLE;
		if (csRoofType == WIND_LOAD_US_ROOF_TYPE_CHAR_MONOSLOPE ) return WIND_LOAD_US_ROOF_TYPE_CASE_MONOSLOPE ;
		if (csRoofType == WIND_LOAD_US_ROOF_TYPE_CHAR_SAWTOOTH  ) return WIND_LOAD_US_ROOF_TYPE_CASE_SAWTOOTH;
	}
	else if (nLoadType == LOAD_TYPE_SNOW_KR)
	{
		if (csRoofType == SNOW_LOAD_KR_ROOF_TYPE_CHAR_GABLET) return SNOW_LOAD_KR_ROOF_TYPE_CASE_GABLET;
		if (csRoofType == SNOW_LOAD_KR_ROOF_TYPE_CHAR_CURVED) return SNOW_LOAD_KR_ROOF_TYPE_CASE_CURVED;
		if (csRoofType == SNOW_LOAD_KR_ROOF_TYPE_CHAR_MGABLE) return SNOW_LOAD_KR_ROOF_TYPE_CASE_MGABLE;
		if (csRoofType == SNOW_LOAD_KR_ROOF_TYPE_CHAR_CYLIND) return SNOW_LOAD_KR_ROOF_TYPE_CASE_CYLIND;
		if (csRoofType == SNOW_LOAD_KR_ROOF_TYPE_CHAR_STOOTH) return SNOW_LOAD_KR_ROOF_TYPE_CASE_STOOTH;
	}
	else if (nLoadType == LOAD_TYPE_SNOW_US)
	{
		if (csRoofType == SNOW_LOAD_US_ROOF_TYPE_CHAR_SLOPE    ) return SNOW_LOAD_US_ROOF_TYPE_CASE_SLOPE;
		if (csRoofType == SNOW_LOAD_US_ROOF_TYPE_CHAR_GABLE    ) return SNOW_LOAD_US_ROOF_TYPE_CASE_GABLE;
		if (csRoofType == SNOW_LOAD_US_ROOF_TYPE_CHAR_CURVED   ) return SNOW_LOAD_US_ROOF_TYPE_CASE_CURVED;
		if (csRoofType == SNOW_LOAD_US_ROOF_TYPE_CHAR_MULTIFOLD) return SNOW_LOAD_US_ROOF_TYPE_CASE_MULTIFOLD;
		if (csRoofType == SNOW_LOAD_US_ROOF_TYPE_CHAR_BARREL   ) return SNOW_LOAD_US_ROOF_TYPE_CASE_BARREL;
		if (csRoofType == SNOW_LOAD_US_ROOF_TYPE_CHAR_SAWTOOTH ) return SNOW_LOAD_US_ROOF_TYPE_CASE_SAWTOOTH;
	}

	ASSERT(0);
	return WIND_LOAD_KR_ROOF_TYPE_CASE_GABLET;
}

int CDB_LoadData::GetCheckPoint(CString csCheckPoint)
{
	if (csCheckPoint == WIND_LOAD_CHECK_CHAR_1) return WIND_LOAD_CHECK_CASE_1;
	if (csCheckPoint == WIND_LOAD_CHECK_CHAR_2) return WIND_LOAD_CHECK_CASE_2;
	if (csCheckPoint == WIND_LOAD_CHECK_CHAR_3) return WIND_LOAD_CHECK_CASE_3;
	if (csCheckPoint == WIND_LOAD_CHECK_CHAR_4) return WIND_LOAD_CHECK_CASE_4;
	if (csCheckPoint == WIND_LOAD_CHECK_CHAR_5) return WIND_LOAD_CHECK_CASE_5;

	ASSERT(0);  
	return WIND_LOAD_CHECK_CASE_1;
}

void CDB_LoadData::GetBasicRoofSnowLoadFactor(CArray<CString, CString&>& aString)
{
	CString csTemp;
	aString.RemoveAll();
	csTemp = GetBasicRoofSnowLoadFactor(0.7); aString.Add(csTemp);
}

CString CDB_LoadData::GetBasicRoofSnowLoadFactor(double dFactor)
{
	return GetStringForFactor(dFactor, 2);
}

CString CDB_LoadData::GetStringForFactor(double dFactor, int nDecimal)
{
	CString csFormat;
	csFormat.Format(_T("%%.%if"), nDecimal);

	CString csTemp;
	csTemp.Format(csFormat, dFactor);
	return csTemp;
}

void CDB_LoadData::GetSnowLoadExposureFactor(CArray<CString, CString&>& aString)
{
	CString csTemp;
	aString.RemoveAll();
	csTemp = GetSnowLoadExposureFactor(1.2); aString.Add(csTemp);
	csTemp = GetSnowLoadExposureFactor(1.1); aString.Add(csTemp);
	csTemp = GetSnowLoadExposureFactor(1.0); aString.Add(csTemp);
	csTemp = GetSnowLoadExposureFactor(0.9); aString.Add(csTemp);
	csTemp = GetSnowLoadExposureFactor(0.8); aString.Add(csTemp);
}

CString CDB_LoadData::GetSnowLoadExposureFactor(double dFactor)
{
	return GetStringForFactor(dFactor, 2);
}

void CDB_LoadData::GetWindDirectionalityFactor(CArray<CString, CString&>& aString)
{
	aString.RemoveAll();
	aString.Add(GetWindDirectionalityFactor(0.85));
	aString.Add(GetWindDirectionalityFactor(0.90));
	aString.Add(GetWindDirectionalityFactor(0.95));
}

void CDB_LoadData::GetSnowLoadThermalFactor(CArray<CString, CString&>& aString)
{
	aString.RemoveAll();
	aString.Add(GetSnowLoadThermalFactor(0.85));
	aString.Add(GetSnowLoadThermalFactor(1.00));
	aString.Add(GetSnowLoadThermalFactor(1.10));
	aString.Add(GetSnowLoadThermalFactor(1.20));
	aString.Add(GetSnowLoadThermalFactor(1.30));
}

CString CDB_LoadData::GetWindDirectionalityFactor(double dFactor)
{
	return GetStringForFactor(dFactor, 2);
}

CString CDB_LoadData::GetSnowLoadThermalFactor(double dFactor)
{
	return GetStringForFactor(dFactor, 2);
}

void CDB_LoadData::GetWindLoadData(T_LOADDATA_D& rLoad)
{
	m_nLoadType = LOAD_TYPE_WIND_KR;
	GetLoadData(rLoad);
}

void CDB_LoadData::GetSnowLoadData(T_LOADDATA_D& rLoad)
{
	m_nLoadType = LOAD_TYPE_SNOW_KR;
	GetLoadData(rLoad);
}

void CDB_LoadData::GetLoadData(T_LOADDATA_D& rLoad)
{
	rLoad.dValue = 0.0;
	if (rLoad.csZone.IsEmpty()) { ASSERT(0); return; }
	if (rLoad.csCity.IsEmpty()) { ASSERT(0); return; }
	
	CreateLoadDB();
	BOOL bWind = (m_nLoadType==LOAD_TYPE_WIND_KR) ? TRUE : FALSE;
	T_LOADDATA_LIST& aLoadDB = bWind ? m_aLoadWind : m_aLoadSnow;

	int nLoadNo = aLoadDB.GetSize();
	if (nLoadNo < 1) { ASSERT(0); return; }
	
	for (int i=0; i<nLoadNo; i++)
	{
		T_LOADDATA_D& CurLoad = aLoadDB[i];
		
		if (CurLoad.csZone.CompareNoCase(rLoad.csZone)==0)
		{
			if (CurLoad.csCity.CompareNoCase(rLoad.csCity)==0)
			{
				rLoad.dValue = CurLoad.dValue;
				break;
			}
		}
	}
	
	ASSERT(rLoad.dValue > 0.0);
}

BOOL CDB_LoadData::IsExist(CString csString, CArray<CString, CString&>& aString)
{
	for (int i=0; i<aString.GetSize(); i++)
	{
		if (csString.CompareNoCase(aString[i])==0) return TRUE;
	}

	return FALSE;
}
