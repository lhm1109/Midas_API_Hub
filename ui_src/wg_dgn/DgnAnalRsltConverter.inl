#pragma region TPostAnalRslt.inl
	template <typename TData>
	TPostAnalRslt<TData>::TPostAnalRslt()
	{
		InitData();
	}

	template <typename TData>
	TPostAnalRslt<TData>::~TPostAnalRslt()
	{
	}

	template <typename TData>
	void TPostAnalRslt<TData>::InitData(int nCapacity)
	{
		m_mapKey.clear();
		m_vecData.clear();
		if (nCapacity > 0)
		{
			Reserve(nCapacity);
		}
	}

	template <typename TData>
	void TPostAnalRslt<TData>::Reserve(int nCapacity)
	{
		m_mapKey.reserve(nCapacity);
		m_vecData.reserve(nCapacity);
	}

	template <typename TData>
	bool TPostAnalRslt<TData>::Exist(const pairDKey& pairKey) const
	{
		const auto itr = m_mapKey.find(pairKey);
		return itr == m_mapKey.end() ? false : true;
	}

	template <typename TData>
	bool TPostAnalRslt<TData>::Exist(const dgn::def::DKey& iKey1st, const dgn::def::DKey& iKey2nd) const
	{
		const auto pairKey = std::make_pair(iKey1st, iKey2nd);
		return Exist(pairKey);
	}

	template <typename TData>
	int TPostAnalRslt<TData>::GetIndex(const pairDKey& pairKey) const
	{
		const auto itr = m_mapKey.find(pairKey);
		if (itr == m_mapKey.end())
		{
			return notIndex;	
		}
		return itr->second;
	}

	template <typename TData>
	int TPostAnalRslt<TData>::GetIndex(const dgn::def::DKey& iKey1st, const dgn::def::DKey& iKey2nd) const
	{
		const auto pairKey = std::make_pair(iKey1st, iKey2nd);
		return GetIndex(pairKey);
	}

	template <typename TData>
	void TPostAnalRslt<TData>::SetData(const pairDKey& pairKey, const TData& Data1, const TData& Data2)
	{
		m_vecData.emplace_back(std::array<TData, 2>{Data1, Data2});
		m_mapKey[pairKey] = m_vecData.size() - 1;
	}

	template <typename TData>
	void TPostAnalRslt<TData>::SetData(const dgn::def::DKey& iKey1st, const dgn::def::DKey& iKey2nd, const TData& Data1, const TData& Data2)
	{
		const auto pairKey = std::make_pair(iKey1st, iKey2nd);
		return SetData(pairKey, Data1, Data2);
	}

	template <typename TData>
	bool TPostAnalRslt<TData>::GetData(const pairDKey& pairKey, TData& Data1, TData& Data2) const
	{
		const int iIdx = GetIndex(pairKey);
		if (iIdx == notIndex) return false;

		Data1 = m_vecData[iIdx][0];
		Data2 = m_vecData[iIdx][1];
		return true;
	}

	template <typename TData>
	bool TPostAnalRslt<TData>::GetData(const dgn::def::DKey& iKey1st, const dgn::def::DKey& iKey2nd, TData& Data1, TData& Data2) const
	{
		const auto pairKey = std::make_pair(iKey1st, iKey2nd);
		return GetData(pairKey, Data1, Data2);
	}

	template <typename TData>
	int TPostAnalRslt<TData>::GetDataCount()
	{
		return static_cast<int>(m_vecData.size());
	}
#pragma endregion