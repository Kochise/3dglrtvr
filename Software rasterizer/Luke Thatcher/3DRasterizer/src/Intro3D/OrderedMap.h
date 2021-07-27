
#pragma once

#include <map>
#include "vector.h"

using namespace std;

template <class keyType, class valueType> class OrderedMap
{
private:
	map<keyType, int> _lookupMap;
	vector<pair<keyType, valueType>> _keyValuePairs;

public:
	bool ContainsKey(const keyType& key) const
	{
		return _lookupMap.find(key) != _lookupMap.end();
	}

	void Add(const keyType& key, const valueType& value)
	{
		pair<keyType, valueType> p;
		p.first = key;
		p.second = value;
		_keyValuePairs.push_back(p);
		int index = _keyValuePairs.size() - 1;

		_lookupMap.insert(pair<keyType, int>(key, index));
	}

	int Count(void) const
	{
		return _keyValuePairs.size();
	}

	const keyType& GetKey(int index) const
	{
		return _keyValuePairs[index].first;
	}
	valueType& GetValue(const keyType& key)
	{
		int index = _lookupMap[key];
		return _keyValuePairs[index].second;
	}

	valueType& operator[](const keyType& key)
	{
		return GetValue(key);
	}
};