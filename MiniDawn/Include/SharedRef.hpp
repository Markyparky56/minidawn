#pragma once
#include <cstdint> // Probably should make my own type header

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
template< class ObjectType > class SharedPtr;
template< class ObjectType > class WeakPtr;
template< class ObjectType > class SharedFromThis;

// Heavily inspired by the Unreal Engine implementation

// Dummies for internal template typecasts
struct StaticCastTag {};
struct ConstCastTag {};
struct NullTag {};

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
        if (--ReferenceController->SharedReferenceCount == 0)
        {
            ReferenceController->DestroyObject();
            ReleaseWeakReference(ReferenceController);
        }
    }

    static inline void AddWeakReference(ReferenceControllerBase* ReferenceController)
    {
        ++ReferenceController->WeakReferenceCount;
    }

    static void ReleaseWeakReference(ReferenceControllerBase* ReferenceController)
    {
        if (--ReferenceController->WeakReferenceCount == 0)
        {
            delete ReferenceController;
        }
    }
};

class SharedReferencer
{
    typedef ReferenceControllerOps TOps;

public:
    SharedReferencer()
        : ReferenceController(nullptr)
    {}

    inline explicit SharedReferencer(ReferenceControllerBase* InReferenceController)
        : ReferenceController(InReferenceController)
    {}

    inline SharedReferencer(SharedReferencer const & InSharedReference)
        : ReferenceController(InSharedReference.ReferenceController)
    {
        if (ReferenceController != nullptr)
        {
            TOps::AddSharedReference(ReferenceController);
        }
    }

    inline SharedReferencer(SharedReferencer&& InSharedReference)
        : ReferenceController(InSharedReference.ReferenceController)
    {
        InSharedReference.ReferenceController = nullptr;
    }

    SharedReferencer(WeakReferencer const& InWeakReference)
        : ReferenceController(InWeakReference.ReferenceController)
    {
        if(ReferenceController != nullptr)
        {
            if (!TOps::ConditionallyAddSharedReference(ReferenceController))
            {
                ReferenceController = nullptr;
            }
        }
    }

    inline ~SharedReferencer()
    {
        if (ReferenceController != nullptr)
        {
            TOps::ReleaseSharedReference(ReferenceController);
        }
    }

    inline SharedReferencer& operator=(SharedReferencer const& InSharedReference)
    {
        auto NewReferenceController = InSharedReference.ReferenceController;
        if (NewReferenceController != ReferenceController)
        {
            if (NewReferenceController != nullptr)
            {
                TOps::AddSharedReference(NewReferenceController);
            }

            if (ReferenceController != nullptr)
            {
                TOps::ReleaseSharedReference(ReferenceController);
            }

            ReferenceController = NewReferenceController;
        }

        return *this;
    }

    inline const bool IsValid() const
    {
        return ReferenceController != nullptr;
    }

    inline const int32_t GetSharedReferenceCount() const
    {
        return ReferenceController != nullptr ? TOps::GetSharedReferenceCount(ReferenceController) : 0;
    }

    inline const bool IsUnique() const
    {
        return GetSharedReferenceCount() == 1;
    }

private:
    friend class WeakReferencer;
    ReferenceControllerBase* ReferenceController;
};

class WeakReferencer
{
    typedef ReferenceControllerOps TOps;

public:
    inline WeakReferencer()
        : ReferenceController(nullptr)
    {}

    inline WeakReferencer(WeakReferencer const & InWeakRefCountPointer)
        : ReferenceController(InWeakRefCountPointer.ReferenceController)
    {
        if (ReferenceController != nullptr)
        {
            TOps::AddWeakReference(ReferenceController);
        }
    }

    inline WeakReferencer(WeakReferencer&& InWeakRefCountPointer)
        : ReferenceController(InWeakRefCountPointer.ReferenceController)
    {
        InWeakRefCountPointer.ReferenceController = nullptr;
    }

    inline WeakReferencer(SharedReferencer const& InSharedRefCountPointer)
        : ReferenceController(InSharedRefCountPointer.ReferenceController)
    {
        if (ReferenceController != nullptr)
        {
            TOps::AddWeakReference(ReferenceController);
        }
    }

    inline ~WeakReferencer()
    {
        if (ReferenceController != nullptr)
        {
            TOps::ReleaseWeakReference(ReferenceController);
        }
    }

    inline WeakReferencer& operator=(WeakReferencer const & InWeakReference)
    {
        AssignReferenceController(InWeakReference.ReferenceController);
        return *this;
    }

    inline WeakReferencer& operator=(WeakReferencer&& InWeakReference)
    {
        auto OldReferenceController = ReferenceController;
        ReferenceController = InWeakReference.ReferenceController;
        InWeakReference.ReferenceController = nullptr;
        if (OldReferenceController != nullptr)
        {
            TOps::ReleaseWeakReference(OldReferenceController);
        }

        return *this;
    }

    inline WeakReferencer& operator=(SharedReferencer const & InSharedReference)
    {
        AssignReferenceController(InSharedReference.ReferenceController);
        return *this;
    }

    inline const bool IsValid() const
    {
        return ReferenceController != nullptr && TOps::GetSharedReferenceCount(ReferenceController) > 0;
    }

private:
    inline void AssignReferenceController(ReferenceControllerBase* NewReferenceController)
    {
        if (NewReferenceController != ReferenceController)
        {
            if (NewReferenceController != nullptr)
            {
                TOps::AddWeakReference(NewReferenceController);
            }

            if (ReferenceController != nullptr)
            {
                TOps::ReleaseWeakReference(ReferenceController);
            }

            ReferenceController = NewReferenceController;
        }
    }

private:
    friend class SharedReferencer;
    ReferenceControllerBase* ReferenceController;
};

template<class SharedPtrType, class ObjectType, class  OtherType>
inline void EnableSharedForThis(SharedPtr<SharedPtrType> const* InSharedPtr, ObjectType const* InObject, SharedFromThis<OtherType> const* InShareable)
{
    if (InShareable != nullptr)
    {
        InShareable->UpdateWeakReferenceInternal(InSharedPtr, const_cast<ObjectType*>(InObject));
    }
}

template< class SharedPtrType, class ObjectType, class OtherType>
inline void EnableSharedFromThis(SharedPtr<SharedPtrType>* InSharedPtr, ObjectType const* InObject, SharedFromThis<OtherType> const* InShareable)
{
    if (InShareable != nullptr)
    {
        InShareable->UpdateWeakReferenceInternal(InSharedPtr, const_cast<ObjectType*>(InObject));
    }
}

template<class SharedRefType, class ObjectType, class OtherType>
inline void EnableSharedFromThis(SharedRef<SharedRefType> const* InSharedRef, ObjectType const* InObject, SharedFromThis<OtherType> const* InShareable)
{
    if (InShareable != nullptr)
    {
        InShareable->UpdateWeakReferenceInternal(InSharedRef, const_cast<ObjectType*>(InObject));
    }
}

template<class SharedRefType, class ObjectType, class OtherType>
inline void EnableSharedFromThis(SharedRef<SharedRefType>* InSharedRef, ObjectType, const* InObject, SharedFromThis<OtherType> const* InShareable)
{
    if (InShareable != nullptr)
    {
        InShareable->UpdateWeakReferenceInternal(InSharedRef, const_Cast<ObjectType*>(InObject));
    }
}

inline void EnabledSharedFromThis(...) {}

// Cast a shared reference to another type
template< class CastToType, class CastFromType>
inline SharedRef<CastToType> StaticCastSharedRef(SharedRef< CastFromType > const& InSharedRef)
{
    return SharedRef<CastToType>(InSharedRef, FStaticCastTag)
}
