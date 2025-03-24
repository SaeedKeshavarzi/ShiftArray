/*
 * Author: Saeed Keshavarzi <keshavarzi1991@gmail.com>
 *
 * shift_array_t - a template class implements FPGA-Like Shift-Register.
 * Its implementation guarantees continuous memory and constant push and pop complexity O(1)
 */

#ifndef _SHIFT_ARRAY_H_
#define _SHIFT_ARRAY_H_

#include <cstddef>
#include <cassert>
#include <cstring>

template<typename _Type, std::size_t _Capacity>
class shift_array_t
{
    static_assert(_Capacity > 0, "Error: in 'shift_array_t', the capacity should be positive.");
public:
    using type = shift_array_t<_Type, _Capacity>;
    using value_type = _Type;
    static constexpr std::size_t capacity() { return _Capacity; }

    shift_array_t() { _data = new value_type[(2 * _Capacity) - 1]{}; _begin = _end = _data + (_Capacity - 1); }
    ~shift_array_t() { delete[] _data; _data = _begin = _end = nullptr; }

    shift_array_t(const shift_array_t& other) { _data = new value_type[(2 * _Capacity) - 1]{}; (void)this->operator=(other); }
    shift_array_t(shift_array_t&& other) : _data(other._data), _begin(other._begin), _end(other._end) { other._data = other._begin = other._end = nullptr; }

    shift_array_t& operator=(const shift_array_t& other)
    {
        (void)std::memcpy(_data, other._data, ((2 * _Capacity) - 1) * sizeof(value_type));
        _begin = _data + (other._begin - other._data);
        _end = _data + (other._end - other._data);
        return *this;
    }

    shift_array_t& operator=(shift_array_t&& other)
    {
        delete[] _data;
        _data = other._data; _begin = other._begin; _end = other._end;
        other._data = other._begin = other._end = nullptr;
        return *this;
    }

    const value_type* begin() const { return _begin; }
    value_type* begin() { return _begin; }

    const value_type* end() const { return _end; }
    value_type* end() { return _end; }

    const value_type* data() const { return _data; }
    value_type* data() { return _data; }

    std::size_t size() const { return (_end - _begin); }
    bool empty() const { return (_begin == _end); }
    void clear() { _begin = _end = _data + (_Capacity - 1); }

    const value_type& front() const { assert(_begin < _end); return *_begin; }
    value_type& front() { assert(_begin < _end); return *_begin; }

    const value_type& back() const { assert(_begin < _end); return *(_end - 1); }
    value_type& back() { assert(_begin < _end); return *(_end - 1); }

    const value_type& operator[](const std::size_t index) const { assert(_begin + index < _end); return _begin[index]; }
    value_type& operator[](const std::size_t index) { assert(_begin + index < _end); return _begin[index]; }

    void sync_unsafe(value_type* ptr) { if (ptr >= _data + _Capacity) *(ptr - _Capacity) = *ptr; else if (ptr < _data + _Capacity - 1) *(ptr + _Capacity) = *ptr; }
    void sync(value_type* ptr) { assert(ptr >= _begin); assert(ptr < _end); sync_unsafe(ptr); }
    void sync(const std::size_t index) { assert(_begin + index < _end); sync_unsafe(_begin + index); }
    void sync() { for (value_type* ptr = _begin; ptr != _end; ++ptr) sync_unsafe(ptr); }

    void set_unsafe(value_type* ptr, const value_type& value) { *ptr = value; sync_unsafe(ptr); }
    void set(value_type* ptr, const value_type& value) { assert(ptr >= _begin); assert(ptr < _end); set_unsafe(ptr, value); }
    void set(const std::size_t index, const value_type& value) { assert(_begin + index < _end); set_unsafe(_begin + index, value); }

    void pop_front() { assert(_begin < _end); ++_begin; }
    void pop_front(const std::size_t n) { assert(_begin + n <= _end); _begin += n; }

    void pop_back() { assert(_begin < _end); --_end; }
    void pop_back(const std::size_t n) { assert(_begin <= _end - n); _end -= n; }

    void push_front(const value_type& value)
    {
        if (--_begin < _data) { _begin += _Capacity; _end += _Capacity; }
        if (_end - _begin > _Capacity) _end = _begin + _Capacity;
        set_unsafe(_begin, value);
    }

    void push_back(const value_type& value)
    {
        if (++_end > _data + (2 * _Capacity) - 1) { _begin -= _Capacity; _end -= _Capacity; }
        if (_end - _begin > _Capacity) _begin = _end - _Capacity;
        set_unsafe(_end - 1, value);
    }

private:
    value_type* _data;
    value_type* _begin;
    value_type* _end;
};

#endif // !_SHIFT_ARRAY_H_
