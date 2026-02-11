#ifndef __DB_ST_DT_WOPN_H__
#define __DB_ST_DT_WOPN_H__

#define MIDAS_SDK

#pragma pack(push, 1)

// Wall Opening Info
#define T_WOPN_K unsigned int   // = T_ELEM_K
#define D_MAX_WALL_OPEN_SIZE  10    // only for wall opening
struct T_WOPN_D
{
	// size info
	double dy;  // 바닥 node에서 개구부 위치까지의 거리
	double dH;  // 개구부 높이

	// 개구부에 의한 분할 정보. (개구부 편집시 연관처리할 요소들)
	T_ELEM_K  WBeamK_top[D_MAX_WALL_OPEN_SIZE];   // 개구부 상단의 Beam Elements
	T_ELEM_K  WBeamK_bot[D_MAX_WALL_OPEN_SIZE];   // 개구부 하단의 Beam Elements
	T_ELEM_K  WallK_left;       // 개구부 좌측 벽
	T_ELEM_K  WallK_right;      // 개구부 우측 벽

	void Initialize()
	{
		dy = 0.0;
		dH = 0.0;
		for (int i=0; i<D_MAX_WALL_OPEN_SIZE; ++i)
		{
			WBeamK_top[i] = 0;
			WBeamK_bot[i] = 0;
		}
		WallK_left  = 0;
		WallK_right = 0;
	}
};
struct T_WOPN_UDRD_D
{
	T_WOPN_K key;
	T_WOPN_D data;
};
struct T_WOPN_UNIT
{
	int dy;
	int dH;

	T_WOPN_UNIT()
	{
		dy = D_UNITSYS_BASE_LENGTH;
		dH = D_UNITSYS_BASE_LENGTH;
	}
};
#define HASHSIZEWOPN HASHSIZEELEM


#pragma pack(pop)

#endif  // __DB_ST_DT_WOPN_H__
