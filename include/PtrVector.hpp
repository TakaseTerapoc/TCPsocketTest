/*! \file PtrVector.hpp
\author Kenta Kabashima
\date 2017/8/1

For vector of pointers
*/
# pragma once

# include <type_traits>
# include <memory>
# include <vector>

//! a vector for classes derived from one base classes
/*
a vector for classes derived from one base classes
Example:
\code{.cpp}
class Base {};

class Derived1 : public Base {};

class Derived2 : public Base {};

void test()
{
    PtrVector<Base> vec;
    vec.push_back(Derived1());
    vec.push_back(Derived2());
}
\endcode
*/
template<typename BaseType, typename Container = std::vector<std::shared_ptr<BaseType> > >
class PtrVector {
public:
    //! type of container
    using container_type = Container;
    //! type of size
    using size_type = typename Container::size_type;
    //! type of difference
    using difference_type = typename Container::difference_type;
    //! type of value
    using value_type = BaseType;
    //! type of value in container
    using inner_value_type = typename Container::value_type;

private:
    //! data
    Container _data;

public:
    //! default constructor (do nothing)
    PtrVector() noexcept : _data() {}

    //! add a data (STL-like function)
    /*!
    push a entry to the end of the vector on the condition that it is a instance of a class derived from BaseType.
    */
    template <typename Type>
    inline auto push_back(Type&& data)
    -> typename std::enable_if_t<std::is_base_of<BaseType, std::remove_const_t<std::remove_reference_t<Type> > >::value >
    {
        _data.push_back(std::make_shared<std::remove_const_t<std::remove_reference_t<Type> > >(std::forward<Type>(data)));
    }

    //! add a data with constructor
    /*!
    construct and push a entry to the end of the vector on the condition that it is a instance of a class derived from BaseType.
    */
    template <typename Type, typename... Args>
    inline auto emplace_back(Args&&... args)
    -> typename std::enable_if_t<std::is_base_of<BaseType, Type>::value >
    {
        _data.push_back(std::make_shared<Type>(std::forward<Args>(args)...));
    }

    //! i-th data
    BaseType& operator[](size_type i) { return *_data[i]; }
    //! i-th data
    const BaseType& operator[](size_type i) const { return *_data[i]; }

    //! reserve memory
    void reserve(size_type size) { _data.reserve(size); }

    //! clear memory
    void clear() { _data.clear(); }

    //! get size
    size_type size() const { return _data.size(); }
};
