#ifndef __DB_ST_DT_WNAT_H__
#define __DB_ST_DT_WNAT_H__

#define MIDAS_SDK

#pragma pack(push, 1)

#define T_WNAT_K unsigned int

enum enmStructureType
{
	ENM_T_CHIMNEYSTSS = 0,
	ENM_T_SOLSIGNS,
	ENM_T_OPENSIGNSLF,
	ENM_T_TRUSTOWERS,
	ENM_T_CIRCLE_PLAN,
	ENM_T_SOLID_WL_SIGNS,
	ENM_T_LATTICE_TOWERS,
	ENM_T_ROOF_ST,
	ENM_T_FENCES_ON_GRND,
	ENM_T_SOLID_FREE

};

struct T_WNAT_STOR_850
{
	  T_STOR_K KeyStor;
	int			nStructType;// 0: chimneys, Tanks, and similar structures
											// 1: solid signs
											// 2: open signs and lattice framework
											// 3: truss towers
	int			nSubType1;	// nStructType: 0 -> 0: Square(Wind normal to face), 1: Square(Wind along diagonal), 2: Hexagonal or Octagonal, 3: Round
											// nStructType: 1 -> 0: At Ground Level, 1: Above Ground Level
											// nStructType: 2 -> 0: Flat-Sided Members, 1: Rounded Members
											// nStructType: 3 -> 0: Square Plan, 1: Triangle Plan
	int			nSubType2;  // 0: Steel Rounded members, 1: Square cross-section members, 2: Other Type
											// (nStructType이 truss towers일때만 사용.)

	double		dXDir[2];  
	double		dYDir[2];
};

struct T_WNAT_STOR_900
{
	T_STOR_K KeyStor;
	int			nStructType;// 0: chimneys, Tanks, and similar structures
											// 1: solid signs
											// 2: open signs and lattice framework
											// 3: truss towers
											// 4: circle plan
											// 5: solid walls and solid signs <= [PMS2015] 현재 Gen에서 제공하기 어려울 것 같으므로 제외함
											// 6: lattice towers
											// 7: roof structures
											// 8: fences on ground
	int			nSubType1;	// nStructType: 0 -> 0: Square(Wind normal to face), 1: Square(Wind along diagonal), 2: Hexagonal or Octagonal, 3: Round
											// nStructType: 1 -> 0: At Ground Level, 1: Above Ground Level
											// nStructType: 2 -> 0: Flat-Sided Members, 1: Rounded Members
											// nStructType: 3 -> 0: Square Plan, 1: Triangle Plan
											// nStructType: 5 -> 0: Case A, 1: Case B, 2: Case C
											// nStructType: 6 -> 0: Square Plan, 1: Triangle Plan
											// nStructType: 7 -> 0: Square(Wind normal to face), 1: Square(Wind along diagonal), 2: Hexagonal or Octagonal, 3: Round
	int			nSubType2;  // nStructType: 3 -> 0: Steel Rounded members, 1: Square cross-section members, 2: Other Type
											// nStructType: 6 -> 0: Angle members, 1: Steel Round members

	/*
		nStructType이 2,3,4,6,8 : dXDir[0]는 단위가 없다.
		nStructType이 2,3,6,8		: dYDir[0]는 단위가 없다.
		nStructType이 5					: dXDir[3], dYDir[3]는 단위가 없다.
	*/
	double		dXDir[4];
	double		dYDir[4];

	int			nSubType3;	// nStructType: 6 -> 0: 90, 1: 45
};
struct T_WNAT_STOR
{
	T_STOR_K KeyStor;
	int			nStructType;// 0: chimneys, Tanks, and similar structures
											// 1: solid signs
											// 2: open signs and lattice framework
											// 3: truss towers
											// 4: circle plan
											// 5: solid walls and solid signs <= [PMS2015] 현재 Gen에서 제공하기 어려울 것 같으므로 제외함
											// 6: lattice towers
											// 7: roof structures
											// 8: fences on ground
	int			nSubType1;	// nStructType: 0 -> 0: Square(Wind normal to face), 1: Square(Wind along diagonal), 2: Hexagonal or Octagonal, 3: Round, 4: Octagonal Pyramid
											// nStructType: 1 -> 0: At Ground Level, 1: Above Ground Level
											// nStructType: 2 -> 0: Flat-Sided Members, 1: Rounded Members
											// nStructType: 3 -> 0: Square Plan, 1: Triangle Plan
											// nStructType: 5 -> 0: Case A, 1: Case B, 2: Case C
											// nStructType: 6 -> 0: Square Plan, 1: Triangle Plan
											// nStructType: 7 -> 0: Square(Wind normal to face), 1: Square(Wind along diagonal), 2: Hexagonal or Octagonal, 3: Round
	int			nSubType2;  // nStructType: 3 -> 0: Steel Rounded members, 1: Square cross-section members, 2: Other Type
											// nStructType: 6 -> 0: Angle members, 1: Steel Round members

	/*
		nStructType이 2,3,4,6,8 : dXDir[0]는 단위가 없다.
		nStructType이 2,3,6,8		: dYDir[0]는 단위가 없다.
		nStructType이 5					: dXDir[3], dYDir[3]는 단위가 없다.
	*/
	double		dXDir[4];  
	double		dYDir[4];

	int			nSubType3;	// nStructType: 6 -> 0: 90, 1: 45
	
	double		dHeight;
	double		dArea;

	void Initialize()
	{
		KeyStor			=0;
		nStructType = 0;
		nSubType1		= 0;
		nSubType2		= 0;
		dXDir[0]		= 0.0;
		dXDir[1]		= 0.0;
		dXDir[2]		= 0.0;
		dXDir[3]		= 0.0;
		dYDir[0]		= 0.0;
		dYDir[1]		= 0.0;
		dYDir[2]		= 0.0;
		dYDir[3]		= 0.0;
		nSubType3		= 0;

		dHeight			= 0.;
		dArea			= 0.;
	}
	
	T_WNAT_STOR()
	{
		Initialize();
	}
	T_WNAT_STOR(T_WNAT_STOR& src)
	{
		*this = src;
	}
	T_WNAT_STOR& operator=(const T_WNAT_STOR& src)
	{
		KeyStor			= src.KeyStor			;
		nStructType = src.nStructType ;
		nSubType1		= src.nSubType1		;
		nSubType2		= src.nSubType2		;
		dXDir[0]		= src.dXDir[0]		;
		dXDir[1]		= src.dXDir[1]		;
		dXDir[2]		= src.dXDir[2]		;
		dXDir[3]		= src.dXDir[3]		;
		dYDir[0]		= src.dYDir[0]		;
		dYDir[1]		= src.dYDir[1]		;
		dYDir[2]		= src.dYDir[2]		;
		dYDir[3]		= src.dYDir[3]		;
		nSubType3		= src.nSubType3		;
		dHeight			= src.dHeight		;
		dArea			= src.dArea			;
		return *this;
	}
	void SetData(const T_WNAT_STOR& src)
	{
		KeyStor			= src.KeyStor			;
		nStructType = src.nStructType ;
		nSubType1		= src.nSubType1		;
		nSubType2		= src.nSubType2		;
		dXDir[0]		= src.dXDir[0]		;
		dXDir[1]		= src.dXDir[1]		;
		dXDir[2]		= src.dXDir[2]		;
		dXDir[3]		= src.dXDir[3]		;
		dYDir[0]		= src.dYDir[0]		;
		dYDir[1]		= src.dYDir[1]		;
		dYDir[2]		= src.dYDir[2]		;
		dYDir[3]		= src.dYDir[3]		;
		nSubType3		= src.nSubType3		;
		dHeight			= src.dHeight		;
		dArea			= src.dArea			;
	}
	void Convert850(T_WNAT_STOR_850& src)
	{
		KeyStor			= src.KeyStor			;
		nStructType = src.nStructType ;
		nSubType1		= src.nSubType1		;
		nSubType2		= src.nSubType2		;
		dXDir[0]		= src.dXDir[0]		;
		dXDir[1]		= src.dXDir[1]		;
		dXDir[2]		= 0.0							;
		dXDir[3]		= 0.0							;
		dYDir[0]		= src.dYDir[0]		;
		dYDir[1]		= src.dYDir[1]		;
		dYDir[2]		= 0.0							;
		dYDir[3]		= 0.0							;
		nSubType3		= 0;
	}
	void Convert900(T_WNAT_STOR_900& src)
	{
		KeyStor = src.KeyStor;
		nStructType = src.nStructType;
		nSubType1 = src.nSubType1;
		nSubType2 = src.nSubType2;
		dXDir[0] = src.dXDir[0];
		dXDir[1] = src.dXDir[1];
		dXDir[2] = src.dXDir[2];
		dXDir[3] = src.dXDir[3];
		dYDir[0] = src.dYDir[0];
		dYDir[1] = src.dYDir[1];
		dYDir[2] = src.dYDir[2];
		dYDir[3] = src.dYDir[3];
		nSubType3 = src.nSubType3;
	}
};

struct T_WNAT_D_850
{
	CArray<T_WNAT_STOR_850, T_WNAT_STOR_850&>	arWnatStor;
	void Initialize()
	{
		arWnatStor.RemoveAll();
	}
	T_WNAT_D_850()
	{
	}
	T_WNAT_D_850(T_WNAT_D_850& src)
	{
		*this = src;
	}
	T_WNAT_D_850& operator=(T_WNAT_D_850& src)
	{
		arWnatStor.Copy(src.arWnatStor);
		return *this;
	}
};

struct T_WNAT_D_900
{
	CArray<T_WNAT_STOR_900, T_WNAT_STOR_900&>	arWnatStor;
	void Initialize()
	{
		arWnatStor.RemoveAll();
	}
	T_WNAT_D_900()
	{
	}
	T_WNAT_D_900(T_WNAT_D_900& src)
	{
		*this = src;
	}
	T_WNAT_D_900& operator=(T_WNAT_D_900& src)
	{
		arWnatStor.Copy(src.arWnatStor);
		return *this;
	}
};
struct T_WNAT_D
{
	CArray<T_WNAT_STOR, T_WNAT_STOR&>	arWnatStor;
	void Initialize()
	{
		arWnatStor.RemoveAll();
	}
	T_WNAT_D()
	{
	}
	T_WNAT_D(T_WNAT_D& src)
	{
		*this = src;
	}
	T_WNAT_D& operator=(T_WNAT_D& src)
	{
		arWnatStor.Copy(src.arWnatStor);
		return *this;
	}
};

struct T_WNAT_D_CH
{
	CArray<T_WNAT_STOR, T_WNAT_STOR&>	arWnatStor;
	void ConvertToString(T_WNAT_D& rData)
	{
		rData.arWnatStor.Copy(arWnatStor);
	}
	void ConvertToChar(T_WNAT_D& rData)
	{
		arWnatStor.Copy(rData.arWnatStor);
	}
};

struct T_WNAT_UDRD_D_850
{
	T_WNAT_K key;
	T_WNAT_D_850 data;
};

struct T_WNAT_UDRD_D_900
{
	T_WNAT_K key;
	T_WNAT_D_900 data;
};

struct T_WNAT_UDRD_D
{
	T_WNAT_K key;
	T_WNAT_D data;
};

struct T_WNAT_UDRD_D_CH
{
	T_WNAT_K key;
	T_WNAT_D_CH data;
};

struct T_WNAT_UDRD_D_CH_RW
{
	T_WNAT_K key;
	int nCountWnatStor;
	
	void GetWnat(T_WNAT_K& rKey, T_WNAT_D_CH& rData)
	{
		rKey=key;
	}
	void SetWnat(const T_WNAT_K Key, const T_WNAT_D_CH& rData)
	{
		key=Key;
		nCountWnatStor = (int)rData.arWnatStor.GetSize();
	}
};
struct T_WNAT_UNIT
{
	T_WNAT_UNIT()
	{
		dXDir = D_UNITSYS_BASE_LENGTH;
		dYDir = D_UNITSYS_BASE_LENGTH;
		dArea = D_UNITSYS_BASE_AREA;
	}
	int dXDir;
	int dYDir;
	int dArea;
}; 
#define HASHSIZEWNAT 1

#pragma pack(pop)

#endif  // __DB_ST_DT_WNAT_H__
