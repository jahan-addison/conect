
#pragma once

#include <array>
#include <cstddef>
#include <iterator>

template<typename T, std::size_t N0, std::size_t N1>
struct matrix
{
    using row_t = std::array<T, N1>;
    inline static constexpr std::array sizes{ N0, N1 };

    static constexpr std::size_t size() noexcept { return N0 * N1; }
    static constexpr bool empty() noexcept { return !size(); }

    T& at(std::size_t i, std::size_t j) { return data_.at(i).at(j); }
    const T& at(std::size_t i, std::size_t j) const
    {
        return data_.at(i).at(j);
    }

    row_t& operator[](std::size_t i) noexcept { return data_[i]; }
    const row_t& operator[](std::size_t i) const noexcept { return data_[i]; }

    T& front() { return data_.front().front(); }
    const T& front() const { return data_.front().front(); }

    T& back() { return data_.back().back(); }
    const T& back() const { return data_.back().back(); }

    T* data() noexcept { return data_.data()->data(); }
    const T* data() const noexcept { return data_.data()->data(); }

    T* begin() noexcept { return data(); }
    const T* begin() const noexcept { return data(); }

    T* end() noexcept { return data() + size(); }
    const T* end() const noexcept { return data() + size(); }

    auto rbegin() noexcept { return std::make_reverse_iterator(end()); }
    auto rbegin() const noexcept { return std::make_reverse_iterator(end()); }

    auto rend() noexcept { return std::make_reverse_iterator(begin()); }
    auto rend() const noexcept { return std::make_reverse_iterator(begin()); }

    void fill(const T& v)
    {
        for (auto& row : data_) {
            row.fill(v);
        }
    }

    friend void swap(matrix& a, matrix& b) { a.data_.swap(b.data_); }

    std::array<row_t, N0> data_;
};