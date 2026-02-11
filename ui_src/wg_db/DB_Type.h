#ifndef __DB_TYPE_H__
#define __DB_TYPE_H__

#define MIDAS_SDK

///////////////////////////////////////////////////////////////////
// Common Structure Definitions 

typedef UINT T_KEY;
typedef CArray<T_KEY, T_KEY> T_KEY_LIST;

struct T_CMD_KEY
{
	T_CMD_KEY() {}
	T_CMD_KEY(int nCmd, T_KEY nKey) { this->nCmd = nCmd; this->nKey = nKey; }
	int   nCmd;
	T_KEY nKey;
};

typedef CList<T_CMD_KEY, T_CMD_KEY&> T_CMD_KEY_LIST;
typedef CArray<CString, LPCTSTR> T_STRING_LIST;

template <class T>
inline void hash_combine(std::size_t& seed, T const& v)
{
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pairValue) const {
        std::size_t pairhash = 0;
        hash_combine(pairhash, pairValue.first);
        hash_combine(pairhash, pairValue.second);
        return pairhash;
    }
};

struct EPairk_hash
{
    size_t operator()(const std::pair<unsigned int, UINT>& k) const noexcept
    {
        size_t h1 = std::hash<unsigned int>{}(k.first);
        size_t h2 = std::hash<UINT>{}(k.second);

        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
struct hash_tuple_impl
{
    static void apply(size_t& seed, Tuple const& tuple)
    {
        hash_tuple_impl<Tuple, Index - 1>::apply(seed, tuple);
        hash_combine(seed, std::get<Index>(tuple));
    }
};

template <class Tuple>
struct hash_tuple_impl<Tuple, 0>
{
    static void apply(size_t& seed, Tuple const& tuple)
    {
        hash_combine(seed, std::get<0>(tuple));
    }
};

struct tuple_hash
{
    template <typename... Ts>
    std::size_t operator()(const std::tuple<Ts...>& x) const noexcept
    {
        auto result = std::size_t{};
        hash_tuple_impl<std::tuple<Ts...>>::apply(result, x);
        return result;
    }
};
/*
struct tuple_hash
{
    template <class... TupleArgs>
    std::size_t operator() (const std::tuple<TupleArgs...> &tupleValue) const {
        const auto nSize = std::tuple_size<decltype(tupleValue)>::value;
        std::size_t tuplehash = 0;
        for(int i=0; i<nSize; ++i)
        {
            hash_combine(tuplehash, std::get<i>(tupleValue));
        }
        return tuplehash;
    }
};
*/
#endif // end of __DB_TYPE_H__