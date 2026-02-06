/*
 *	Name: Owned
 *	Author: Pawel Mrochen
 */

#pragma once

#include <stdexcept>
//#include <variant>

namespace core {

//template<typename T, typename U>
//class OwnedVector<T, U>;

/*template<typename T>
class IOwned
{
    //friend class OwnedVector<IOwned<T>, T>;

public:
	virtual bool hasOwner() const = 0;
    virtual T* getOwner() const = 0;

private:
    virtual void setOwner(T* owner) = 0;
};*/

template<typename T>
class Owned //: public IOwned<T>
{
    friend class OwnedPtr<T>;

public:
    virtual ~Owned() = default;

	bool hasOwner() const { return owner_; }
    T* getOwner() const { return owner_; }

protected:
    //virtual void onOwnerChanging(T* newOwner) {}
    //virtual void onOwnerChanged(T* prevOwner) {}

    virtual void setOwner(T* owner) 
    { 
        if (owner_)
            throw std::runtime_error();
        owner_ = owner;
    }

    /*virtual*/ void release()
    {
        if (owner_)
            setOwner(nullptr);
        delete this;
    }

private:
    T* owner_ = {};
};

/*template<typename... Types>
class VariantOwned
{
public:
    virtual ~VariantOwned() = default;

	bool hasOwner() const;
    template<typename T> T* getOwner() const { return std::get<T*>(owner_); }

protected:
    //virtual void onOwnerChanging(T* newOwner) {}
    //virtual void onOwnerChanged(T* prevOwner) {}

private:
    virtual void setOwner(std::variant<Types*...> owner) { owner_ = owner; }

private:
    std::variant<Types*...> owner_ = {};
};*/

//inline void ownedPtrSetOwner(IOwned<T>* owned, T* owner)
//{
//    owned->setOwner(owner);
//}

//inline void ownedPtrRelease(IOwned<T>* owned)
//{
//    owned->release();
//}

} // namespace core
