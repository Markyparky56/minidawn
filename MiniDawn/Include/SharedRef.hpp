#pragma once
#include <cstdint> // Probably should make my own type header
#include <utility>

// RemoveReference
template< typename T > struct RemoveReference       { typedef T Type; };
template< typename T > struct RemoveReference<T&>   { typedef T Type; };
template< typename T > struct RemoveReference<T&&>  { typedef T Type; };
// MoveTemp
template< typename T >
inline typename RemoveReference<T>::Type&& MoveTemp(T&& Obj)
{
    return (typename RemoveReference<T>::Type&&)Obj;
}
// Forward
template< typename T>
inline T&& Forward(typename RemoveReference<T>::Type& Obj)
{
    return (T&&)Obj;
}

template< typename T>
inline T&& Forward(typename RemoveReference<T>::Type&& Obj)
{
    return (T&&)Obj;
}

template< class ObjectType > class SharedRef;
// SharedPtr
// WeakPtr

// Heavily inspired by the Unreal Engine implementation

struct FStaticCastTag {};
struct FConstCastTag {};
struct FNullTag {};

class ReferenceControllerBase
{
public:
    explicit ReferenceControllerBase(void* _Object)
    { }

    int32_t SharedReferenceCount;

    int32_t WeakReferenceCount;

    void* Object;

    virtual void DestroyObject() = 0;

    virtual ~ReferenceControllerBase() {}

private:
    ReferenceControllerBase(ReferenceControllerBase const&);
    ReferenceControllerBase& operator=(ReferenceControllerBase const&);
};

template< typename ObjectType, typename DeleterType >
class ReferenceControllerWithDeleter : private DeleterType, public ReferenceControllerBase
{
public:
    explicit ReferenceControllerWithDeleter(void* _object, DeleterType&& Deleter)
        : DeleterType(MoveTemp(Deleter))
        , ReferenceControllerBase(object)
    {
    }

    virtual void DestroyObject()
    {
        (*static_cast<DeleteType*>(this))((ObjectType*)static_cast<ReferenceControllerBase*>(this)->Object);
    }
};

template< typename Type >
struct DefaultDeleter
{
    inline void operator()(Type* Object) const
    {
        delete Object;
    }
};

template< typename ObjectType >
inline ReferenceControllerBase* NewDefaultReferenceController(ObjectType* Object)
{
    return new ReferenceControllerWithDeleter<ObjectType, DefaultDeleter<ObjectType>>(Object, DefaultDeleter<ObjectType>());
}

template< typename ObjectType, typename DeleterType >
inline ReferencecontrollerBase* NewDefaultReferenceController(OBjectType* Object, DeleterType&& Deleter)
{
    return new ReferenceControllerWithDeleter<ObjectType, typename RemoveReference<DeleterType>::Type>(Object, Forward< DeleteType > (Deleter));
}

template< class ObjectType >
struct RawPtrProxy
{
    ObjectType* object;
    ReferenceControllerBase* ReferenceController;

    RawPtrProxy(ObjectType* _object)
        : Object(_object)
        , ReferenceController(NewDefaultReferenceController(_object))
    {}

    template< class Deleter >
    RawPtrProxy(ObjectType* _object, Deleter&& _deleter)
        : Object(_object)
        , ReferenceController(NewDefaultReferenceController(_object, Forward< Deleter >(_deleter)))
    {}
};

struct ReferenceControllerOps
{
    static inline const int32_t GetSharedReferenceCount(const ReferenceControllerBase* ReferenceController)
    {
        return ReferenceController->SharedReferenceCount;
    }

    static inline void AddSharedReference(ReferenceControllerBase* ReferenceController)
    {
        ++ReferenceController->SharedReferenceCount;
    }

    static bool ConditionallyAddSharedReference(ReferenceControllerBase* ReferenceController)
    {
        if (ReferenceController->SharedReferenceCount == 0)
        {
            return false;
        }

        ++ReferenceController->SharedReferenceCount;
        return true;
    }

    static inline void ReleaseSharedReference(ReferenceControllerBase* ReferenceController)
    {
        
    }
};