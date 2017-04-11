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

// "square" a type (multiply operator with itself) and add some other type.
template <typename __T, typename __U>
decltype(auto) double_add_func(__T val, __U to_add) {
    return val * val + to_add;
}

// variadic template function forwarding
template <typename __FUNC, typename... __ARGS>
decltype(auto) call_func(__FUNC&& func, __ARGS... args) {
    return func(std::forward<__ARGS>(args)...);
}

int main(int argc, char** argv) {
    const auto desired_result = 33.5;
    const auto ref_initial = -100.0;
    const auto ref_after = 1.75;
    const auto copy_initial = -0.25;
    const auto copy_after = 100;
    const auto double_add = 5.0;
    const auto final_before_add = desired_result - (ref_after + double_add + copy_initial);
    const auto initial_val = final_before_add / std::pow(final_before_add, 2.0/6.0);

    // placement new
    uint32_t memory[sizeof(weird_mul)];
    new (memory) weird_mul(initial_val);
    auto& inital_obj = *reinterpret_cast<weird_mul*>(memory);

    // variadic lambda forwarding
    auto forwarding_lambda = [](
        auto&& func, auto&&... args) { return func(std::forward<decltype(args)>(args)...); };

    // variadic lambda forwarding while adding a reference and copied value
    auto val = ref_initial;
    auto val_two = copy_initial;
    auto call_double_ref_after_lambda = [&val, val_two](auto&&... arg) { return double_add_func(std::forward<decltype(arg)>(arg)...) + val + val_two; };
    val = ref_after;
    val_two = copy_after;

    // Call the variadic function with the variadic lambda using the other lambda, initial_obj and double_add as arguments.
    auto ret = call_func(forwarding_lambda, call_double_ref_after_lambda, inital_obj, double_add);

    std::cout << "Operations expected:\n " << initial_val << " * " << std::sqrt(initial_val) << " = " << final_before_add << "\n";
    std::cout << final_before_add << " + " << double_add << " + " << ref_after << " + " << copy_initial << " = " << desired_result << "\n";

    std::cout << "Expect " << desired_result << ". Got: " << ret.get_val() << "\n";
    return EXIT_SUCCESS;
}