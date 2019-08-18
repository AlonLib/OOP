//
// Created by Naama on 17/06/2019.
//

#ifndef HW5_UTILITIES_H
#define HW5_UTILITIES_H
/* Conditional */

template<bool, typename, typename>
struct Conditional;

template<typename T, typename F>
struct Conditional<true, T, F> {
    typedef T value;
};
template<typename T, typename F>
struct Conditional<false, T, F> {
    typedef F value;
};

/* ConditionalInteger */

template<bool, int, int>
struct ConditionalInteger;

template<int A, int B>
struct ConditionalInteger<true, A, B> {
    constexpr static int value = A;
};

template<int A, int B>
struct ConditionalInteger<false, A, B> {
    constexpr static int value = B;
};

/* ConditionalAndValue */
/**
 * the following methods will make a LAZY evaluation (-;
 */

template <bool B>
struct ValueWrapperBool {
    //typedef true value;
    typedef ValueWrapperBool<B> value;
    constexpr static bool result = B;
};

template<bool, typename, typename>
struct ConditionalAndValue;

template<typename T, typename F>
struct ConditionalAndValue<true, T, F> {
    typedef typename T::value value;
};
template<typename T, typename F>
struct ConditionalAndValue<false, T, F> {
    typedef typename F::value value;
};


#endif //HW5_UTILITIES_H
