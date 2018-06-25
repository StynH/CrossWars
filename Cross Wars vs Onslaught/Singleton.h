#ifndef SINGLETON_H
#define	SINGLETON_H
#include <cstddef>
#include "iostream"

//Class to replace the constantly adding of pInstance and such.
template<class T>
class TSingleton {
protected:
	static T *pSingleton;

public:
	virtual ~TSingleton() { }

	//First instance gets created when called for the first time.
	static T* GetInstance() {
		if (!pSingleton) {
			pSingleton = new T;
		}
		return (pSingleton);
	}

	static void DeleteInstance() {
		if (pSingleton) {
			delete (pSingleton);
			pSingleton = NULL;
		}
	}

	template<typename F> void SafeDelete(F*& a) {
		if (a) {
			delete a;
			a = NULL;
		}
	}
};

template<class T>
T* TSingleton<T>::pSingleton = nullptr;

#endif