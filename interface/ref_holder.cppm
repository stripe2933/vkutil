export module vkutil:ref_holder;

import std;
export import vulkan_hpp;

#define FWD(...) static_cast<decltype(__VA_ARGS__) &&>(__VA_ARGS__)

export namespace vkutil{
    template <typename T, typename... Ts>
    struct RefHolder {
        std::tuple<Ts...> references;
        T value;

        template <std::invocable<Ts&...> F, std::convertible_to<Ts> ...Us>
        RefHolder(
            F &&gen,
            Us &&...references_) noexcept((std::is_nothrow_convertible_v<Us, Ts> && ...) && std::is_nothrow_invocable_v<F, Ts&...>)
            : references { FWD(references_)... },
              value { std::apply(gen, references) } { }

        operator T&() const noexcept { return value; }
        operator const T&() noexcept { return value; }

        auto get() noexcept -> T& { return value; }
        auto get() const noexcept -> const T& { return value; }
    };
}