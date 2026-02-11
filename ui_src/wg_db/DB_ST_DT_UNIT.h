#ifndef __DB_ST_DT_UNIT_H__
#define __DB_ST_DT_UNIT_H__

#define MIDAS_SDK

#pragma pack(push, 1)

// (중요!) 이곳 변경시 Solver 팀에도 알려야 함
// 단위계 종류
// L:길이, F:힘, A:각도, D:각도[Degree], R:각도[Radian], T:온도, C:온도[Celsius], H:온도[Fahrenheit]
#define D_UNITSYS_LENGTH_INDEX_MM           0
#define D_UNITSYS_LENGTH_INDEX_CM           1
#define D_UNITSYS_LENGTH_INDEX_M            2
#define D_UNITSYS_LENGTH_INDEX_IN           3
#define D_UNITSYS_LENGTH_INDEX_FT           4
#define D_UNITSYS_LENGTH_NAME_MM             _LS(IDS_MIT_UNIT_MM)
#define D_UNITSYS_LENGTH_NAME_CM             _LS(IDS_MIT_UNIT_CM)
#define D_UNITSYS_LENGTH_NAME_M              _LS(IDS_MIT_UNIT_M)
#define D_UNITSYS_LENGTH_NAME_IN             _LS(IDS_MIT_UNIT_IN)
#define D_UNITSYS_LENGTH_NAME_FT             _LS(IDS_MIT_UNIT_FT)
#define D_UNITSYS_LENGTH_FACTOR_MM          0.001
#define D_UNITSYS_LENGTH_FACTOR_CM          0.01
#define D_UNITSYS_LENGTH_FACTOR_M           1.  
#define D_UNITSYS_LENGTH_FACTOR_IN          0.0254
#define D_UNITSYS_LENGTH_FACTOR_FT          0.3048

#define D_UNITSYS_FORCE_INDEX_KG            0
#define D_UNITSYS_FORCE_INDEX_TON           1
#define D_UNITSYS_FORCE_INDEX_N             2
#define D_UNITSYS_FORCE_INDEX_KN            3
#define D_UNITSYS_FORCE_INDEX_LBF           4
#define D_UNITSYS_FORCE_INDEX_KIP           5
#define D_UNITSYS_FORCE_NAME_KG             _LS(IDS_MIT_UNIT_KGF)
#define D_UNITSYS_FORCE_NAME_TON            _LS(IDS_MIT_UNIT_TONF)
#define D_UNITSYS_FORCE_NAME_N              _LS(IDS_MIT_UNIT_N)
#define D_UNITSYS_FORCE_NAME_KN             _LS(IDS_MIT_UNIT_KN)
#define D_UNITSYS_FORCE_NAME_LBF            _LS(IDS_MIT_UNIT_LBF)
#define D_UNITSYS_FORCE_NAME_KIP            _LS(IDS_MIT_UNIT_KIPS)
#define D_UNITSYS_FORCE_FACTOR_KG           9.80665
#define D_UNITSYS_FORCE_FACTOR_TON          9806.65
#define D_UNITSYS_FORCE_FACTOR_N            1.  
#define D_UNITSYS_FORCE_FACTOR_KN           1000.
#define D_UNITSYS_FORCE_FACTOR_LBF          4.448223
#define D_UNITSYS_FORCE_FACTOR_KIP          4448.223

#define D_UNITSYS_HEAT_INDEX_CAL           0
#define D_UNITSYS_HEAT_INDEX_KCAL          1
#define D_UNITSYS_HEAT_INDEX_JOULE         2
#define D_UNITSYS_HEAT_INDEX_BTU           3
#define D_UNITSYS_HEAT_INDEX_KJOULE        4
#define D_UNITSYS_HEAT_NAME_CAL            _LS(IDS_MIT_UNIT_CAL)
#define D_UNITSYS_HEAT_NAME_KCAL           _LS(IDS_MIT_UNIT_KCAL)
#define D_UNITSYS_HEAT_NAME_JOULE          _LS(IDS_MIT_UNIT_J)
#define D_UNITSYS_HEAT_NAME_BTU            _LS(IDS_MIT_UNIT_BTU)
#define D_UNITSYS_HEAT_NAME_KJOULE         _LS(IDS_MIT_UNIT_KJ)
#define D_UNITSYS_HEAT_FACTOR_CAL          4.186
#define D_UNITSYS_HEAT_FACTOR_KCAL         4186.
#define D_UNITSYS_HEAT_FACTOR_JOULE        1.
#define D_UNITSYS_HEAT_FACTOR_BTU          1055.
#define D_UNITSYS_HEAT_FACTOR_KJOULE       1000.

#define D_UNITSYS_TEMPER_INDEX_C            0
#define D_UNITSYS_TEMPER_INDEX_F            1
#define D_UNITSYS_TEMPER_NAME_C             _LS(IDS_MIT_UNIT_CELSIUS)
#define D_UNITSYS_TEMPER_NAME_F             _LS(IDS_MIT_UNIT_FAHRENHEIT)
#define D_UNITSYS_TEMPER_FACTOR_C           1.0        
//#define D_UNITSYS_TEMPER_FACTOR_F           0.555556  // 5.0/9.0
#define D_UNITSYS_TEMPER_FACTOR_F           5.0/9.0   // MNET:4741-HSSHIM-20120521: 열관련 계수에서 유효숫자로 인한 오차가 발생하지 않도록 계산식으로 변경.

#define D_UNITSYS_NONE              0
#define D_UNITSYS_BASE_LENGTH       1   // L
#define D_UNITSYS_BASE_DISPLACEMENT 1   // L
#define D_UNITSYS_BASE_FORCE        2   // F
#define D_UNITSYS_BASE_WEIGHT       2   // F
#define D_UNITSYS_BASE_DEGREE       4   // D[Deg]   // Deg 고정
#define D_UNITSYS_BASE_RADIAN       5   // R[Rad]   // Rad 고정
#define D_UNITSYS_BASE_TEMPER       6   // T[T]     // Cel, Fah 변경가능 -> 값 변환 안됨
#define D_UNITSYS_BASE_AREA         9   // L^2
#define D_UNITSYS_BASE_VOLUME       10  // L^3
#define D_UNITSYS_BASE_STIF         11  // L^4
#define D_UNITSYS_BASE_MOMENT       12  // F*L
#define D_UNITSYS_BASE_UNITMOMENT   13  // F*L/L
#define D_UNITSYS_BASE_UNITFORCE    14  // F/L
#define D_UNITSYS_BASE_UNITLOAD     14  // F/L
#define D_UNITSYS_BASE_SPRING       14  // F/L
#define D_UNITSYS_BASE_ROSPRING     15  // F*L/rad
#define D_UNITSYS_BASE_CPSPRING     16  // F*L/rad/L  -> F/rad
#define D_UNITSYS_BASE_ELAST        17  // F/L^2
#define D_UNITSYS_BASE_STRESS       17  // F/L^2
#define D_UNITSYS_BASE_PRESSURE     17  // F/L^2
#define D_UNITSYS_BASE_DENSITY      18  // F/L^3
#define D_UNITSYS_BASE_THERMAL      19  // 1/T
#define D_UNITSYS_BASE_TIME         20  // sec
#define D_UNITSYS_BASE_GRAVITY      21  // L/sec^2
#define D_UNITSYS_BASE_ACCELERATION 21  // L/sec^2
#define D_UNITSYS_BASE_VELOCITY     22  // L/sec
#define D_UNITSYS_BASE_MASS         23  // M
#define D_UNITSYS_BASE_ROMASS       24  // M*L^2
#define D_UNITSYS_BASE_LINEMASS     25  // M
#define D_UNITSYS_BASE_AREAMASS     26  // M
#define D_UNITSYS_BASE_CPS          27  // [cps]  cycle per second
#define D_UNITSYS_BASE_PERTIME      28  // 1/sec
#define D_UNITSYS_BASE_FORCETIME    29  // F/sec
#define D_UNITSYS_BASE_MOMENTTIME   30  // F*L/sec
#define D_UNITSYS_BASE_ACCELTIME    31  // [L/sec^2]/sec
#define D_UNITSYS_BASE_FREQUENCY    32  // rad/sec
#define D_UNITSYS_BASE_L6           33  // L^6
#define D_UNITSYS_BASE_HOUR         34  // hr
#define D_UNITSYS_BASE_DAY          35  // day
#define D_UNITSYS_BASE_HEAT         36  // H           열량
#define D_UNITSYS_BASE_SPHEAT       37  // H/F*T       비열
#define D_UNITSYS_BASE_FLOWRATE     38  // L^3/hr      유량
#define D_UNITSYS_BASE_CONVECTION   39  // H/L^2*hr*T  대류계수
#define D_UNITSYS_BASE_CONDUCTION   40  // H/L*hr*T    전도계수
#define D_UNITSYS_BASE_HEATSOURCE   41  // H/L^3*hr    발열
#define D_UNITSYS_BASE_RSTRESS      42  // L^2/F       단위 응력당 변형률
#define D_UNITSYS_BASE_RSTRESS2     43  // L/L/F/L^2
#define D_UNITSYS_BASE_PERLENGTH    44  // 1/L
#define D_UNITSYS_BASE_FSEC_L       45  // F*sec/L       (Damping : translational)
#define D_UNITSYS_BASE_FLSEC_R      46  // F*L*sec/rad   (Damping : rotational)
#define D_UNITSYS_BASE_SEC_L        47  // sec/L
#define D_UNITSYS_BASE_MASSDENSITY  48  // F/L^3/M
#define D_UNITSYS_BASE_RADIAN_L     49  // rad/L
#define D_UNITSYS_BASE_RADIAN_SEC2  50  // rad/sec^2
#define D_UNITSYS_BASE_CPMASS       51  // M*L^2/L/rad ->M*L/rad
#define D_UNITSYS_BASE_CPDAMPING    52  // F*sec/rad
#define D_UNITSYS_BASE_DISTSTIFF    53  // F*L^2
#define D_UNITSYS_BASE_STIFFGRADIENT  54  // F*L^4
#define D_UNITSYS_BASE_RADIAN_L_SEC2  55  // rad/L*sec^2
#define D_UNITSYS_BASE_PERVOLUME    56  // 1/L^3
#define D_UNITSYS_BASE_PERCENT      57  // %
#define D_UNITSYS_BASE_FSEC_L3      70  // F*sec/L^3
#define D_UNITSYS_BASE_UNIT_AREA    71  // L2/L
#define D_UNITSYS_BASE_PERAREA		72  // 1/L^2
#define D_UNITSYS_BASE_MASS_INERTIA 73  // M*L^2 // Mass moment of inertia
//*^^*단위계 추가시 여기 수정을 모두 찾아 코딩한다.

//-----------------------------------------------------------------
// 단위계 추가되면 업데이트 해 줄 것 !!!!!! (추가된 단위계 + 1)
//-----------------------------------------------------------------
#define D_UNITSYS_BASE_END          73

struct T_UNIT_NAMEFACT
{
	CString strLength[10];
	CString strForce[10];
	CString strHeat[10];
	CString strTemper[10];
	double dblLength[10];
	double dblForce[10];
	double dblHeat[10];
	double dblTemper[10];
	int nCountLength;
	int nCountForce;
	int nCountHeat;
	int nCountTemper;
};
struct T_UNIT_INDEX
{
	int nBase_Length;
	int nBase_Force;
	int nBase_Heat;
	int nBase_Temper;    // 0: Celsius,   1: Fahrenheit 
	BOOL operator==(const T_UNIT_INDEX& rData)
	{
		if (nBase_Length == rData.nBase_Length &&
				nBase_Force  == rData.nBase_Force  && 
				nBase_Heat   == rData.nBase_Heat   &&
				nBase_Temper == rData.nBase_Temper  )
			return TRUE;
		return FALSE;   
	}
	T_UNIT_INDEX& operator=(const T_UNIT_INDEX &src)
	{
		nBase_Length = src.nBase_Length;
		nBase_Force  = src.nBase_Force;
		nBase_Heat   = src.nBase_Heat;
		nBase_Temper = src.nBase_Temper;

		return *this;
	}
};
struct T_UNIT_CONVERT
{
	double dblBase_Length;
	double dblBase_Force;
	double dblBase_Area;
	double dblBase_Volume;
	double dblBase_Stif;
	double dblBase_Moment;
	double dblBase_UnitForce;
	double dblBase_UnitMoment;
	double dblBase_RoSpring;
	double dblBase_CpSpring;
	double dblBase_Stress;
	double dblBase_Density;
	double dblBase_Thermal;
	double dblBase_Mass;
	double dblBase_Length6;
	double dblBase_Heat;
	double dblBase_Temper;
	double dblBase_SpHeat;
	double dblBase_Convection;
	double dblBase_Conduction;
	double dblBase_HeatSource;
	double dblBase_Rstress;
	double dblBase_FSEC_L;
	double dblBase_FSEC_L3;
	double dblBase_FLSEC_R;
	double dblBase_SEC_L;
	double dblBase_MassDensity;
	double dblBase_RADIAN_L;
	double dblBase_RADIAN_SEC2;
	double dblBase_CpMass;
	double dblBase_CpDamping;
	double dblBase_DistStiff;
	double dblBase_StiffGradient;
	double dblBase_Radian_L_SEC2;
	double dblBase_PerVolume;
	double dblBase_UnitArea;
	double dblBase_PerArea;
	//*^^*단위계 추가시 여기 수정
};
struct T_UNIT_SYSTEM
{
	T_UNIT_SYSTEM(){nIndex=30;}
	CArray<int,int>Index;
	CString strName[30];
	int nIndex;
};

#pragma pack(pop)

#endif  // __DB_ST_DT_UNIT_H__
