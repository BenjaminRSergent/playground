#include <cmath>
#include <iostream>
#include <string>

class weird_mul {
public:
    weird_mul(double val) {
        _val = val;
    }

    weird_mul operator+(double val) {
        return weird_mul(_val + val);
    }
    weird_mul operator*(const weird_mul& other) {
        return weird_mul(_val * std::sqrt(other._val));
    }
    double get_val() {
        return _val;
    }

private:
    double _val;
};

template <typename __T>
decltype(auto) double_func(__T val) {
    return val * val;
}

template <typename __FUNC, typename... __ARGS>
decltype(auto) call_func(__FUNC&& func, __ARGS... args) {
    return func(std::forward<__ARGS>(args)...);
}

int main(int argc, char** argv) {
    const auto desired_result = 33.5;
    const auto ref_test = -100.0;
    const auto ref_add = 1.5;
    const auto double_add = 5.0;
    const auto final_before_add = desired_result - (ref_add + double_add);
    const auto initial_val = final_before_add / std::pow(final_before_add, 2.0/6.0);

    uint32_t memory[sizeof(weird_mul)];
    new (memory) weird_mul(initial_val);
    auto& inital_obj = *reinterpret_cast<weird_mul*>(memory);

    auto my_lambda = [](
        auto&& func, auto&& arg, auto&& add) { return func(std::forward<decltype(arg)>(arg)) + add; };

    auto val = ref_test;
    auto call_double_add_val = [&val](auto&& arg) { return double_func(std::forward<decltype(arg)>(arg)) + val; };
    val = ref_add;

    auto ret = call_func(my_lambda, call_double_add_val, inital_obj, double_add);

    std::cout << "Operations expected:\n " << initial_val << " * " << std::sqrt(initial_val) << " = " << final_before_add << "\n";
    std::cout << final_before_add << " + " << double_add << " + " << ref_add << " = " << desired_result << "\n";

    std::cout << "Expect " << desired_result << ". Got: " << ret.get_val() << "\n";
    return EXIT_SUCCESS;
}