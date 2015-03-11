// 单件模式

#pragma once

template<class T>
class TSingleton
{
public:
	static T& getSingleton()
	{
		static T _instance;
		return _instance;
	}

protected:
	TSingleton() {}
	virtual ~TSingleton()	{}
	TSingleton(const TSingleton<T>&);	// 不实现
	TSingleton<T>& operator = (const TSingleton<T>&); // 不实现
};