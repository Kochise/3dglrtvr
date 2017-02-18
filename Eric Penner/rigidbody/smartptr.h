//
// smart pointer garbage collection
//
// Written by Maxwell Sayles and Eric Penner, copyright (C) 2002, 2004.
// Idea for the Assign method credit to Sandu Turcan


#ifndef SMARTPTR_H
#define SMARTPTR_H

#include "assert.h"

#define NULL 0


//General reference counting interface. Release method must free resources
//Classes can derive from this if they want to do their own reference counting.
class IRefCount  
{
public:
	virtual ~IRefCount(){}
	virtual void AddRef() = 0;
	virtual void Release() = 0;
};


//Reference counter to derive from. If you want objects to have their own reference
//count, it's best to virtually derive from this.
class RefCountImpl : public IRefCount  
{
public:
	virtual ~RefCountImpl(){}
	RefCountImpl() { m_nCount = 0; }
	
	virtual void AddRef()  { m_nCount++; }
	virtual void Release() 
	{ 
		assert(m_nCount>=0);
		m_nCount--; 
		if(m_nCount<=0) 
			delete this; 
	}

protected:

	int m_nCount;
};



template <class T> class SmartPtr
{
public:

	//Reference counter for classes that don't implement one. A copy of the pointer is kept
	//in here so we can delete the object from Release(). If we cast to another smart pointer type, 
	//the type of pointer kept in the reference counter will remain the same. This is likely a good
	//thing, since otherwise delete would be called on the type of the last pointer that goes out of
	//scope, which is sort of random. Basically, if you are going to have smart pointers to base classes, 
	//make sure the destructor is virtual.
	class InternalRefCount : public IRefCount  
	{
	public:
		virtual ~InternalRefCount(){}
		InternalRefCount(T* pT) {m_nCount = 0; m_pT = pT;}

		virtual void AddRef() {m_nCount++;}
		virtual void Release()
		{
			assert(m_nCount>=0);
			m_nCount--;
			if(m_nCount<=0)
			{
				if(m_pT != NULL)
					delete m_pT; 
				delete this;
			}
		}

	protected:

		int m_nCount;
		T*  m_pT;      //This is here so we are able to delete the object
	};

	T* m_pObject;
	IRefCount* m_pRefCount;

    // this method is called if T does not implement IRefCount
    void Assign(void *pT)
    {
        if(pT==NULL)
            Assign((IRefCount*)NULL);
        else
        {
            Assign(new InternalRefCount(static_cast<T*>(pT)));
        }
    }

    // this method is picked over Assign(void *ptr) if T implements IRefCount.
    void Assign(IRefCount *pRefcount)
    {
        if( pRefcount!=NULL ) 
			pRefcount->AddRef();
        IRefCount* pOldref = m_pRefCount;
        m_pRefCount = pRefcount;
        if (pOldref!=NULL) 
			pOldref->Release();
    }

	//
	// default constructor
	//
	SmartPtr()
	{
		m_pObject = NULL;
		m_pRefCount = NULL;
	}


	SmartPtr (T* p)
	{
		m_pObject = p;
		m_pRefCount = NULL;
		Assign(p);
	}

	//
	// copy constructor
	//
	SmartPtr (const SmartPtr<T>& p)
	{
		m_pObject = p.m_pObject;
		m_pRefCount = NULL;
		Assign(p.m_pRefCount);
	}


	//
	// constructor from another SmartPtr (typically downcasting)
	//
	template <class T2> SmartPtr (const SmartPtr<T2>& p)
	{
		m_pObject = p.m_pObject;
		m_pRefCount = NULL;
		Assign(p.m_pRefCount);
	}

	//
	// construct with an object and a reference count
	// to be used only by reinterpretCast(), staticCast(), and dynamicCast()
	//
	SmartPtr (T* pObject, IRefCount* pRefCount)
	{
		m_pObject = pObject;
		m_pRefCount = NULL;
		Assign(pRefCount);
	}


	//
	// destruction
	//
	~SmartPtr()
	{
		if(m_pRefCount)
			m_pRefCount->Release();
	}

	//
	// assignment
	//
	SmartPtr<T>& operator= (T* p)
	{
		m_pObject = p;
		Assign(p);
		return *this;
	}

	//
	// assignment
	//
	SmartPtr<T>& operator= (const SmartPtr<T>& p)
	{
		// self assignment
		if (this->m_pObject == p.m_pObject) return *this; 

		m_pObject = p.m_pObject;
		Assign(p.m_pRefCount);
		return *this;
	}


	//
	// assignment from another SmartPtr (typically downcasting)
	//
	template <class T2> SmartPtr<T>& operator= (const SmartPtr<T2>& p)
	{
		// self assignment
		if (this->m_pObject == p.m_pObject) return *this;

		m_pObject = p.m_pObject;
		Assign(p.m_pRefCount);
		return *this;
	}

	//
	// member dereference
	//
	T* operator->()
	{
		return m_pObject;
	}


	//
	// const member dereference
	//
	const T* operator->() const
	{
		return m_pObject;
	}


	T* GetPtr() {return m_pObject;}


	//
	// pointer cast (should only be used for leaf routines)
	//
	operator T*()
	{
		return m_pObject;
	}


	//
	// const pointer cast (should only be used for leaf routines)
	//
	operator const T*() const
	{
		return m_pObject;
	}


	//
	// reinterpret_cast<>
	//
	template <class T2> SmartPtr<T2> ReinterpretCast() const
	{
		return SmartPtr<T2> (reinterpret_cast<T2*>(m_pObject), m_pRefCount);
	}


	//
	// static_cast<>
	//
	template <class T2> SmartPtr<T2> StaticCast() const
	{
		return SmartPtr<T2> (static_cast<T2*>(m_pObject), m_pRefCount);
	}


	//
	// dynamic_cast<>
	//
	template <class T2> SmartPtr<T2> DynamicCast() const
	{
		T2* p = dynamic_cast<T2*>(m_pObject);
		if (p == 0)
		{	// cast failed
			return SmartPtr<T2> (0, NULL);
		}
		else
		{	// cast succeeded
			return SmartPtr<T2> (p, m_pRefCount);
		}
	}

	template <class T2> bool Is() const
	{
		T2* p = dynamic_cast<T2*>(m_pObject);
		return (p != NULL);
	}


};


//This is a copy of the ATL CComPtr. Using #include <atlbase.h> was causing some problems
//and CComPtr<>::operator-> wouldn't compile. Since all we need is this class, I just
//copied it over

template <class T>
class CComPtr
{
public:
	typedef T _PtrClass;
	CComPtr() {p=NULL;}
	CComPtr(T* lp)
	{
		if ((p = lp) != NULL)
			p->AddRef();
	}
	CComPtr(const CComPtr<T>& lp)
	{
		if ((p = lp.p) != NULL)
			p->AddRef();
	}
	~CComPtr() {if (p) p->Release();}
	void Release() {if (p) p->Release(); p=NULL;}
	operator T*() {return (T*)p;}
	T& operator*() {_ASSERTE(p!=NULL); return *p; }
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
	T** operator&() { assert(p==NULL); return &p; }
	T* operator->() { assert(p!=NULL); return p; }
	T* operator=(T* lp)
	{
		if (lp != NULL)
			lp->AddRef();
		if (p)
			(p)->Release();
		p = lp;
		return lp;
	}
	T* operator=(const CComPtr<T>& lp)
	{
		if (lp.p != NULL)
			lp.p->AddRef();
		if (p)
			(p)->Release();
		p = lp.p;
		return lp.p;
	}
#if _MSC_VER>1020
	bool operator!(){return (p == NULL);}
#else
	BOOL operator!(){return (p == NULL) ? TRUE : FALSE;}
#endif
	// pointer member
	T* p;
};



#endif // SMARTPTR_H

