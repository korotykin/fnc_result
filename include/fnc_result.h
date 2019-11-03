#pragma once

#include <memory>
#include <type_traits>

template<typename T>
class SuccessT
{
public:
    T Value;
};

template<>
class SuccessT<void>
{
};

template<typename EC>
class FailT
{
public:
    EC Value;
};

template<>
class FailT<void>
{
};

template<typename T, typename std::enable_if<!std::is_void<T>::value, int>::type = 0>
SuccessT<T> Success(const T& val) { return SuccessT<T>{val}; }

template<typename T, typename std::enable_if<!std::is_void<T>::value, int>::type = 0>
SuccessT<T> Success(T&& val) { return SuccessT<T>{std::forward<T>(val)}; }

SuccessT<void> Success() {return SuccessT<void>();}

template<typename EC, typename std::enable_if<!std::is_void<EC>::value, int>::type = 0>
FailT<EC> Fail(const EC& err) { return FailT<EC>{err}; }

template<typename EC, typename std::enable_if<!std::is_void<EC>::value, int>::type = 0>
FailT<EC> Fail(EC&& err) { return FailT<EC>{std::forward<EC>(err)}; }

FailT<void> Fail() {return FailT<void>();}

template<typename T, typename EC>
class FncResult
{
public:
    template<typename U = T, typename std::enable_if<std::is_copy_constructible<U>::value && std::is_convertible<U*, T*>::value, int>::type = 0 >
    FncResult(const SuccessT<U>& res): mIsSuccess(true), mValue{new U{res.Value}} {}
    
    template<typename U = T, typename std::enable_if<std::is_convertible<U*, T*>::value && std::is_move_constructible<U>::value && !std::is_reference<U>::value, int>::type = 0>
    FncResult(SuccessT<U>&& res): mIsSuccess(true), mValue{new U{std::forward<U>(res.Value)}} {}
    
    template<typename U = EC, typename std::enable_if<std::is_copy_constructible<U>::value && std::is_convertible<U*, EC*>::value, int>::type = 0 >
    FncResult(const FailT<U>& err): mIsSuccess(false), mError{new U{err.Value}} {}
    
    template<typename U = EC, typename std::enable_if<std::is_convertible<U*, EC*>::value && std::is_move_constructible<U>::value && !std::is_reference<U>::value, int>::type = 0>
    FncResult(FailT<U>&& err): mIsSuccess(false), mError{new U{std::forward<U>(err.Value)}} {}

    operator bool() const { return mIsSuccess; }
    const T& Value() const
        { if (!mIsSuccess) { throw std::logic_error{"FncResult: Asking for value when error ocured"};} return *mValue; }
    const EC& Error() const
        { if (mIsSuccess) { throw std::logic_error{"FncResult: Asking for error when succeed"};} return *mError; }
private:
    bool mIsSuccess;
    std::shared_ptr<T> mValue;
    std::shared_ptr<EC> mError;
public:
    FncResult() = delete;
    FncResult(const FncResult<T, EC>& rhv) = default;
    FncResult(FncResult<T, EC>&& rhv) = default;
    FncResult& operator=(const FncResult<T, EC>& rhv) = default;
    FncResult& operator=(FncResult<T, EC>&& rhv) = default;
};

template<typename EC>
class FncResult<void, EC>
{
public:
    FncResult(SuccessT<void>): mIsSuccess(true) {}

    template<typename U = EC, typename std::enable_if<std::is_copy_constructible<U>::value && std::is_convertible<U*, EC*>::value, int>::type = 0 >
    FncResult(const FailT<U>& err): mIsSuccess(false), mError{new U{err.Value}} {}
    
    template<typename U = EC, typename std::enable_if<std::is_convertible<U*, EC*>::value && std::is_move_constructible<U>::value && !std::is_reference<U>::value, int>::type = 0>
    FncResult(FailT<U>&& err): mIsSuccess(false), mError{new U{std::forward<U>(err.Value)}} {}

    operator bool() const { return mIsSuccess; }
    const EC& Error() const
        { if (mIsSuccess) { throw std::logic_error{"FncResult: Asking for error when succeed"};} return *mError; }
private:
    bool mIsSuccess;
    std::shared_ptr<EC> mError;
public:
    FncResult() = delete;
    FncResult(const FncResult<void, EC>& rhv) = default;
    FncResult(FncResult<void, EC>&& rhv) = default;
    FncResult& operator=(const FncResult<void, EC>& rhv) = default;
    FncResult& operator=(FncResult<void, EC>&& rhv) = default;
};

template<typename T>
class FncResult<T, void>
{
public:
    template<typename U = T, typename std::enable_if<std::is_copy_constructible<U>::value && std::is_convertible<U*, T*>::value, int>::type = 0 >
    FncResult(const SuccessT<U>& res): mIsSuccess(true), mValue{new U{res.Value}} {}
    
    template<typename U = T, typename std::enable_if<std::is_convertible<U*, T*>::value && std::is_move_constructible<U>::value && !std::is_reference<U>::value, int>::type = 0>
    FncResult(SuccessT<U>&& res): mIsSuccess(true), mValue{new U{std::forward<U>(res.Value)}} {}

    FncResult(FailT<void>): mIsSuccess(false) {}

    operator bool() const { return mIsSuccess; }
    const T& Value() const
        { if (!mIsSuccess) { throw std::logic_error{"FncResult: Asking for value when error ocured"};} return *mValue; }
private:
    bool mIsSuccess;
    std::shared_ptr<T> mValue;
public:
    FncResult() = delete;
    FncResult(const FncResult<T, void>& rhv) = default;
    FncResult(FncResult<T, void>&& rhv) = default;
    FncResult& operator=(const FncResult<T, void>& rhv) = default;
    FncResult& operator=(FncResult<T, void>&& rhv) = default;
};

template<>
class FncResult<void, void>
{
public:
    FncResult(SuccessT<void>): mIsSuccess(true) {}
    FncResult(FailT<void>): mIsSuccess(false) {}

    operator bool() const { return mIsSuccess; }
private:
    bool mIsSuccess;
public:
    FncResult() = delete;
    FncResult(const FncResult<void, void>& rhv) = default;
    FncResult(FncResult<void, void>&& rhv) = default;
    FncResult& operator=(const FncResult<void, void>& rhv) = default;
    FncResult& operator=(FncResult<void, void>&& rhv) = default;
};
