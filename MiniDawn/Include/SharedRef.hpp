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
inline void EnableSharedFromThis(SharedPtr<SharedPtrType> const* InSharedPtr, ObjectType const* InObject, SharedFromThis<OtherType> const* InShareable)
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

inline void EnableSharedFromThis(...) {}

// Cast a shared reference to another type
template< class CastToType, class CastFromType>
inline SharedRef<CastToType> StaticCastSharedRef(SharedRef< CastFromType > const& InSharedRef)
{
    return SharedRef<CastToType>(InSharedRef, FStaticCastTag)
}

// SharedRef is non-nullable
template< class ObjectType >
class SharedRef
{
public:
    // No default constructor due to not supporting empty references
    // Must be initalised with a valid object at construction time

    // Constructs a shared reference that owns the specified object
    // Cannot be nullptr
    template <class OtherType>
    inline explicit SharedRef(ObjectType* InObject)
        : Object(InObject)
        , SharedReferenceCount(NewDefaultReferenceController(InObject))
    {
        Init(InObject);
    }

    // Constructs a shared reference that owns the specified object
    // Cannot be nullptr
    template<class OtherType, class DeleterType>
    inline SharedRef(OtherType* InObject, DeleterType&& InDeleter)
        : Object(InObject)
        , SharedReferenceCount(NewCustomReferenceController(InObject, Forward<DeleterType>(InDeleter)))
    {
        Init(InObject);
    }

    // Constructs a shared reference using a proxy reference to a raw pointer
    // Cannot be nullptr
    template<class OtherType>
    inline SharedRef(RawPtrProxy<OtherType> const& InRawPtrProxy)
        : Object(InRawPtrProxy.object)
        , SharedRefenceCount(InRawPtrProxy.ReferenceController)
    {
        EnableSharedFromThis(this, InRawPtrProxy.object, InRawPtrProxy.object);
    }

    // Constructs a shared reference as a reference to an exisiting shared reference's object
    // Need to implicitly upcast to base classes
    template<class OtherType>
    inline SharedRef(SharedRef<OtherType> const& InSharedRef)
        : Object(InSharedRef.Object)
        , SharedReferenceCount(InSharedRef.SharedReferenceCount)
    { }

    // Used internally to statically cast one shared reference to another
    // Don't use this directly, instead use StaticCastSharedRef
    // Creates a shared reference as a shared reference to an exisiting shared reference after 
    // statically casting that reference's object (what?)
    template<class OtherType>
    inline SharedRef(SharedRef<OtherType> const& InSharedRef, StaticCastTag)
        : Object(static_cast<ObjectType*>(InSharedRef.Object))
        , SharedReferenceCount(InSharedRef.SharedReferenceCount)
    { }

    // Used internally to cast a const shared reference to a mutable reference
    // Don't use this directly, instead use ConstCastSharedRef
    // Creates a shared reference as a shared reference to an exisiting shared reference after
    // const casting that references object (try saying that three times fast!)
    template<class OtherType>
    inline SharedRef(SharedRef<OtherType> const& OtherSharedRef, ObjectType* InObject)
        : Object(InObject)
        , SharedReferenceCount(OtherSharedRef.SharedReferenceCount)
    { }

    // Used internally to create a shared reference from an exisiting shared reference
    // while using the specified object reference instead of the incoming shared reference's object pointer
    // This is used with the SharedFromThis feature by UpdateWeakReferenceInternal
    inline SharedRef(SharedRef const& InSharedRef)
        : Object(InSharedRef.Object)
        , SharedReferenceCount(InSharedRef.SharedReferenceCount)
    { }

    inline SharedRef(SharedRef&& InSharedRef)
        : Object(InSharedRef.Object)
        , SharedReferenceCount(InSharedRef.SharedReferenceCount)
    {
        // No move performed, because it would leave a sharedref in a null state
        // Provided to avoid the compiler complaining
    }

    // Assignment operators
    // Replaces the shared reference with the specified reference, the previous referenced object
    // will no longer be referenced and deleted if there are no other references
    inline SharedRef& operator=(SharedRef const& InSharedRef)
    {
        SharedReferenceCount = InSharedRef.SharedReferenceCount;
        Object InSharedRef.Object;
        return *this;
    }

    inline SharedRef& operator=(SharedRef&& InSharedRef)
    {
        //TODO: MemSwap stuff
    }

    template<class OtherType>
    inline SharedRef& operator=(RawPtrProxy<OtherType> const& InRawPtrProxy)
    {
        *this = SharedRef<ObjectType>(InRawPtrProxy);
        return *this;
    }

    // Return a C++ Reference to the object this shared reference is referencing
    inline ObjectType& Get() const
    {
        return *Object;
    }

    // Dereference operator returns a reference to the object this shared reference points to
    inline ObjectType& operator*() const
    {
        return *Object;
    }

    // Arrow operator returns a pointer to this shared reference's object
    inline ObjectType* operator->() const
    {
        return Object;
    }

    // Returns the number of shared references to this object, including itself
    // Not fast, for debugging
    inline const int32_t GetsharedReferencecount() const
    {
        return SharedReferenceCount.GetSharedReferenceCount();
    }

    // Returns true if this is the only shared reference to this object
    // May be weak references
    // Not fast, for debugging
    inline const bool IsUnique() const
    {
        return SharedReferenceCount.IsUnique();
    }

private:

    template<class OtherType>
    void Init(OtherType* InObject)
    {
        EnableSharedFromThis(this, InObject, InObject);
    }

    // Converts a shared pointer to a shared reference, pointer must be valid
    // Intentionally private, use ToSharedRef instead
    template<class OtherType>
    inline explicit SharedRef(SharedPtr<OtherType> const& InSharedPtr)
        : Object(InSharedPtr.Object)
        , SharedReferenceCount(InSharedPtr.SharedReferenceCount)
    {
        // Assert IsValid()
    }

    template<class OtherType>
    inline explicit SharedRef(SharedPtr<OtherType>&& InSharedPtr)
        : Object(InSharedPtr.Object)
        , SharedReferenceCount(MoveTemp(InSharedPtr.SharedReferenceCount))
    {
        InSharedPtr.Object = nullptr;

        // Assert IsValid()
    }

    // Checks to see if this shared reference is actually pointing to an object
    // Only used internally because shared references must always be valid
    inline const bool IsValid() const
    {
        return Object != nullptr;
    }

    // Hash function omitted, maybe later?

    // Kinda sad when you're friends with yourself
    template<class OtherType> friend class SharedRef;
    
    template<class OtherType> friend class SharedPtr;
    template<class OtherType> friend class WeakPtr;

private:
    ObjectType* Object;
    SharedReferencer SharedReferenceCount;
};

// Wrapper for a type that yields a reference to that type
template<class T>
struct MakeReferenceTo
{
    typedef T& Type;
};

// Specialisation for MakeReferenceToVoid
template<>
struct MakeReferenceTo<void>
{
    typedef void Type;
};

// SharedPtr is a non-intrusive reference counted authoriative object pointer
template<class ObjectType>
class SharedPtr
{
public:

    // Constructs an empty shared pointer
    inline SharedPtr(NullTag* = nullptr)
        : Object(nullptr)
        , SharedReferenceCount()
    {}

    // Constructs a shared pointer that owns the specified object, accepts nullptr
    template<class OtherType>
    inline explicit SharedPtr(OtherType* InObject)
        : Object(InObject)
        , SharedReferenceCount(NewDefaultReferenceController(InObject))
    {
        EnableSharedFromThis(this, InObject, InObject);
    }

    // Constructs a shared pointer that owns the specified object, accepts nullptr
    template<class OtherType, class DeleterType>
    inline SharedPtr(OtherType* InObject, DeleterType&& InDeleter)
        : Object(InObject)
        , SharedReferenceCount(NewCustomReferencecontroller(InObject, Forward<DeleterType>(InDeleter)))
    {
        EnableSharedFromThis(this, InObject, InObject);
    }

    // Constructs a shared pointer using a proxy reference to a raw pointer
    template<class OtherType>
    inline SharedPtr(RawPtrProxy<OtherType> const& InRawPtrProxy)
        : Object(InRawPtrProxy.Object)
        , sharedReferenceCount(InRawPtrProxy.ReferenceController)
    {
        EnableSharedFromThis(this, InRawPtrProxy.Object, InRawPtrProxy.Object);
    }

    // Constructs a shared pointer as a shared reference to an exisiting shared pointers object
    // Required to implicitly upcast to a base class
    template<class OtherType>
    inline SharedPtr(SharedPtr<OtherType> const& InSharedPtr)
        : Object(InSharedPtr.Object)
        , SharedReferenceCount(InSharedPtr.SharedReferenceCount)
    {}

    inline SharedPtr(SharedPtr const& InSharedPtr)
        : Object(InSharedPtr.Object)
        , SharedReferenceCount(InSharedPtr.SharedReferenceCount)
    { }

    inline SharedPtr(SharedPtr&& InSharedPtr)
        : Object(InSharedPtr.Object)
        , SharedReferenceCount(MoveTemp(InSharedPtr.SharedReferenceCount))
    { 
        InSharedPtr.Object = nullptr;
    }

    // Implicitly converts a shared reference to a shared pointer, adding a reference to the object
    // Allows implict conversion from a SharedRef to a SharedPtr because it's always safe
    template<class OtherType>
    inline SharedPtr(SharedRef<OtherType> const& InSharedRef)
        : Object(InSharedRef.Object)
        , SharedReferenceCount(InSharedRef.SharedReferenceCount)
    {
        // Does not steal the pointer from the SharedRef as that would leave it null, invalidating it
    }

    // Special constructor used internally statically cast one shared pointer to another
    // Do not call this directly, instead use StaticCastSharedPtr
    // Creates a shared pointer as a shared reference to an existing shared pointer after
    // statically casting that pointer's object
    template<class OtherType>
    inline SharedPtr(SharedPtr<OtherType> const& InSharedPtr, StaticCastTag)
        : Object(static_cast<ObjectType*>(InSharedPtr.Object))
        , SharedReferenceCount(InSharedPtr.SharedReferenceCount)
    { }

    // Special constructor used internally to cast a const shared pointer to a mutable pointer
    // Do not call this directly, instead use ConstCastSharedPtr
    // Creates a shared pointer as a shared reference to an existing shared pointer after
    // const casting that pointer's object
    template<class OtherType>
    inline SharedPtr(SharedPtr<OtherType> const& InSharedPtr, ConstCastTag)
        : Object(const_cast<ObjectType*>(InSharedPtr.Object))
        , SharedReferenceCount(InSharedPtr.SharedReferenceCount)
    { }

    // Special constructor used internally to create a shared pointer from an exisiting shared pointer,
    // while using the specified object pointer instead of the incoming shared pointer's object pointer
    template<class OtherType>
    inline SharedPtr(SharedPtr<OtherType> const& OtherSharedPtr, ObjectType* InObject)
        : Object(InObject)
        , SharedReferenceCount(OtherSharedPtr.SharedReferenceCount)
    { }

    // Asigned to a nullptr pointer
    // The object reference by this shared pointer will no longer be reference 
    // and deleted if there are no other references
    inline SharedPtr& operator=(NullTag*)
    {
        Reset();
        return *this;
    }

    // Assignment operator replaces this shared pointer with the specified shared pointer
    // The object currently reference by this shared pointer will no longer be referenced and will
    // be deleted if there are no other referencers.
    inline SharedPtr& operator=(SharedPtr const& InSharedPtr)
    {
        SharedReferenceCount = InSharedPtr.SharedReferenceCount;
        Object = InSharedPtr.Object;
        return *this;
    }

    inline SharedPtr& operator=(SharedPtr&& InSharedPtr)
    {
        if (this != &InSharedPtr)
        {
            Object = InSharedPtr.Object;
            InSharedPtr.Object = nullptr;
            SharedReferenceCount = MoveTemp(InSharedPtr.SharedReferenceCount);
        }
        return *this;
    }

    // Assignment operator replaces this shared pointer with the specified shared pointer
    // The object currently referenced by this shared pointer will no longer be reference and will 
    // be deleted if there are no other referencers
    template<class OtherType>
    inline SharedPtr& operator=(RawPtrProxy<OtherType> const& InRawPtrProxy)
    {
        *this = SharedPtr<ObjectType>(InRawPtrProxy);
        return *this;
    }

    // Converts a shared pointer to a shared reference, pointer must be valid
    inline SharedRef<ObjectType> ToSharedRef() const
    {
        // Assert IsValid()
        return SharedRef<ObjectType>(*this);
    }

    // Returns the object referenced by this pointer, or nullptr if there is no object
    inline ObjectType* Get() const
    {
        return Object;
    }

    // Checks to see if this shared pointer is actually pointing to an object
    inline const bool IsValid() const
    {
        return Object != nullptr;
    }

    // Dereference operator returns a reference to the object this shared pointer points to
    inline typename MakeReferenceTo<ObjectType>::Type operator*() const
    {
        // Assert IsValid()
        return *Object;
    }

    // Arrow operator returns a pointer to the object this shared pointer references
    inline ObjectType* operator->() const
    {
        // Assert IsValid()
        return Object;
    }

    // Resets this shared pointer, removing a reference to the object
    // If there are no other shared references to the object then it will be destroyed
    inline void Reset()
    {
        *this = SharedPtr<ObjectType>();
    }

    // Returns the number of shared references to this object, including itself
    // No fast, for debugging
    inline const int32_t GetSharedReferenceCount() const
    {
        return SharedReferenceCount.GetSharedReferenceCount();
    }

    // Returns true if this is the only shared reference to this object, no including weak references
    // Not fast, for debugging
    inline const bool IsUnique() const
    {
        return SharedReferenceCount.IsUnique();
    }

private:
    // Constructs a shared pointer from a weak point, allowing you to access the object as long as it
    // hasn't expired.
    // Private to force users to be explicit when converting. Use Weak Pointers Pin() method instead
    template<class OtherType>
    inline explicit SharedPtr(WeakPtr<OtherType> const& InWeakPtr)
        : Object(nullptr)
        , SharedReferenceCount(InWeakPtr.WeakReferenceCount)
    {
        if (SharedReferenceCount.IsValid())
        {
            Object = InWeakPtr.Object;
        }
    }

    template<class OtherType> friend class SharedPtr;
    template<class OtherType> friend class SharedRef;
    template<class OtherType> friend class WeakPtr;
    template<class OtherType> friend class SharedFromThis;

private:
    ObjectType* Object;
    SharedReferencer SharedReferenceCount;
};

template<class ObjectType> struct IsZeroConstructType<SharedPtr<ObjectType> > { enum { Value = true }; };

// WeakPtr is a non-intrusive reference counted weak object pointer
template<class ObjectType>
class WeakPtr
{
public:
    // Constructs an empty weakptr
    inline WeakPtr(NullTag* = nullptr)
        : Object(nullptr)
        , WeakReferenceCount()
    { }

    // Constructs a weak pointer from a shared reference
    template<class OtherType>
    inline WeakPtr(SharedRef<OtherType> const& InSharedRef)
        : Object(InSharedRef.Object)
        , WeakReferenceCount(InSharedRef.SharedReferenceCount)
    { }

    // Constructs a weak pointer from a shared pointer
    template<class OtherType>
    inline WeakPtr(SharedPtr<OtherType> const& InSharedPtr)
        : Object(InSharedPtr.Object)
        , WeakReferenceCount(InSharedPtr.SharedReferenceCount)
    { }

    // Constructs a weak point from a weak pointer fo another type
    // Allows derived-to-base conversions
    template<class OtherType>
    inline WeakPtr(WeakPtr<OtherType> const& InWeakPtr)
        : Object(InWeakPtr.Object)
        , WeakReferenceCount(InWeakPtr.WeakReferenceCount)
    { }

    template<class OtherType>
    inline WeakPtr(WeakPtr<OtherType>&& InWeakPtr)
        : Object(InWeakPtr.Object)
        , WeakReferenceCount(MoveTemp(InWeakPtr.WeakReferenceCount))
    {
        InWeakPtr.Object = nullptr;
    }

    inline WeakPtr(WeakPtr const& InWeakPtr)
        : Object(InWeakPtr.Object)
        , WeakReferenceCount(InWeakPtr.WeakReferenceCount)
    { }

    inline WeakPtr(WeakPtr&& InWeakPtr)
        : Object(InWeakPtr.Object)
        , WeakReferenceCount(MoveTemp(InWeakPtr.WeakReferenceCount))
    {
        InWeakPtr.Object = nullptr;
    }

    // Assigned to a nullptr pointer, clears the weak pointer's reference
    inline WeakPtr& operator=(NullTag*)
    {
        Reset();
        return *this;
    }

    // Assignment operator adds a weak reference to the object referenced by the specified weak pointer
    inline WeakPtr& operator=(WeakPtr const& InWeakPtr)
    {
        Object = InWeakPtr.Pin.Get();
        WeakReferenceCount = InWeakPtr.WeakReferenceCount;
        return *this;
    }

    inline WeakPtr& operator=(WeakPTr&& InWeakPtr)
    {
        if (this != &InWeakPtr)
        {
            Object = InWeakPtr.Object;
            InWeakPtr.Object = nullptr;
            WeakReferenceCount = MoveTemp(InWeakPtr.WeakReferenceCount);
        }
        return *this;
    }
    // Assignment operator adds a weak reference to the object referenced byt he specified weak pointer
    // Intended to allow derived-to-base conversions
    template<typename OtherType>
    inline WeakPtr& operator=(WeakPtr<OtherType> const& InWeakPtr)
    {
        Object = InWeakPtr.Pin().Get();
        WeakReferenceCount = InWeakPtr.WeakReferenceCount;
        return *this;
    }

    template<typename OtherType>
    inline WeakPtr& operator=(WeakPtr<OtherType>&& InWeakPtr)
    {
        Object = InWeakPtr.Object;
        InWeakPtr.Object = nullptr;
        WeakReferenceCount = MoveTemp(InWeakPtr.WeakReferenceCount);
        return *this;
    }

    // Assignment operator sets this weak pointer from a shared reference
    template<class OtherType>
    inline WeakPtr& operator=(SharedRef<OtherType> const& InSharedRef)
    {
        Object = InSharedRef.Object;
        WeakReferenceCount = InSharedRef.SharedReferenceCount;
        return *this;
    }

    // Assignment operator sets this weak pointer from a shared pointer
    template<class OtherType>
    inline WeakPtr& operator=(SharedPtr<OtherType> const& InSharedPtr)
    {
        Object InSharedPTr.Object;
        WeakReferenceCount = InSharedPtr.SharedReferenceCount;
        return *this;
    }

    // Converts this weak pointer to a shared pointer that you can used to access the object
    // Object must have no expired, ensure SharedPtr is valid before using
    inline SharedPtr<ObjectType> Pin() const
    {
        return SharedPtr<ObjectType>(*this);
    }

    // Checks to see if the weak pointer actually has a valid reference to an object
    inline const bool IsValid() const
    {
        return Object != nullptr && WeakReferenceCount.IsValid();
    }

    // Resets this weak pointer, removing a weak reference to the object
    // If there are no other shared or weak references to the object then the object will be destroyed
    inline void Reset()
    {
        *this = WeakPtr<ObjectType>();
    }

    // Returns true if the object this weak pointer points to is the same as the specified object pointer
    inline bool HasSameObject(const void* InOtherPtr) const
    {
        return Pin().Get() == InOtherPtr;
    }

private:
    template<class OtherType> friend class WeakPtr;
    template<class OtherType> friend class SharedPtr;

    ObjectType* Object;
    WeakReferencer WeakReferenceCount;
};
