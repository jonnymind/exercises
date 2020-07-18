#include <type_traits>

template<int...> struct Vector;

template<typename, typename> struct concat;
template<int... Left, int... Right> struct concat<Vector<Left...>, Vector<Right...>> {
    using type = Vector<Left..., Right...>;
};


template<typename> struct uniq;
template<int... Is> struct uniq<Vector<Is...>> {
    using type = Vector<Is...>;
};

template<int i, int j, int... Is> 
struct uniq<Vector<i, j, Is...>> {
    using type = typename std::conditional<
        i == j,
        typename concat<Vector<i>, typename uniq<Vector<Is...>>::type>::type,
        typename concat<Vector<i,j>, typename uniq<Vector<Is...>>::type>::type
    >::type;
};

int main() {
    static_assert(std::is_same<uniq<Vector<1, 1, 2, 2, 3>>::type, Vector<1,2,3>>::value);
}
