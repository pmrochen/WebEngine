/*
 *	Name: RefCounted
 *	Author: Pawel Mrochen
 */

#pragma once

#include <atomic>

namespace common {

//class IRefCounted
//{
//public:
//    virtual bool hasOwner() const = 0;
//	virtual int getReferenceCount() const = 0;
//	virtual void addReference/*retain*/() = 0;
//	virtual void release() = 0;
//};

class RefCounted //: public IRefCounted
{
public:
    virtual ~RefCounted() = default;

	/*virtual*/ bool hasOwner() const /*override*/ { return (refCount_ > 0); }
	/*virtual*/ int getReferenceCount() const /*override*/ { return refCount_; }
	/*virtual*/ void addReference/*retain*/() /*override*/ { refCount_++; }
	/*virtual*/ void release() /*override*/ { if (--refCount_ <= 0) delete this; }

private:
    std::atomic_int refCount_ = 0;
};

//inline void intrusive_ptr_add_ref(IRefCounted* refCounted)
//{
//    refCounted->addReference();
//}

//inline void intrusive_ptr_release(IRefCounted* refCounter) 
//{ 
//    refCounter->release();
//}

} // namespace common
