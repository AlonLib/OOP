//
// Created by Alon on 15/06/2019.
//

#ifndef PART2_LIST_H
#define PART2_LIST_H

/* List */
template<typename... >
struct List;

template <typename T, typename... Ts>
struct List<T,Ts...> {
    typedef T head;
    typedef List<Ts...> next;
    constexpr static int size = 1 + sizeof...(Ts);
};

template <>
struct List<> {
    constexpr static int size = 0;
    //typedef List<> head;
    //typedef List<> next;
};

/* PrependList */
template <typename ...T>
struct PrependList;

template <typename T, typename ...Ts>
struct PrependList<T, List<Ts...>> {
    typedef List<T,Ts...> list;
};

/* GetAtIndex */
template <int N, typename ...T>
struct GetAtIndex;

template <typename T, typename ...Ts>
struct GetAtIndex<0, List<T, Ts...>> {
    typedef T value;
};

template <int N, typename T, typename ...Ts>
struct GetAtIndex<N, List<T, Ts...>> {
    static_assert(N > 0, "GetAtIndex recived N that is not positive!");
    typedef typename GetAtIndex<N-1,List<Ts...>>::value value;
};

/* SetAtIndex */
template <int N, class Type, typename ...Ts>
struct SetAtIndex;

template <int N, typename Type, typename T, typename ...Ts>
struct SetAtIndex<N, Type, List<T, Ts...>> {
    static_assert(N > 0, "SetAtIndex recived N that is not positive!");
    typedef typename PrependList<T, typename SetAtIndex<N-1, Type, List<Ts...>>::list>::list list;
};

template <typename Type, typename T, typename ...Ts>
struct SetAtIndex<0, Type, List<T, Ts...>> {
    typedef List<Type, Ts...> list;
};


#endif //PART2_LIST_H
