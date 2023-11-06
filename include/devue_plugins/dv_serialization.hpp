#pragma once

#include <vector>
#include <stdexcept>

namespace devue::plugins {
    class dv_bin_writer {
    public:
        dv_bin_writer() = delete;
        dv_bin_writer(std::vector<uint8_t>& ref) : m_ref(ref) {
            m_ref.clear();
        }

    public:
        template <typename T>
        dv_bin_writer& operator<< (const T& value) {
            push(value);
            return *this;
        }

    private:
        template <typename T>
        void push(const T& value, 
                         typename std::enable_if<std::is_arithmetic<T>::value, bool>::type = true) 
        {
            char* ptr = (char*)&value;
            m_ref.insert(m_ref.end(), ptr, ptr + sizeof(T));
        }

        void push(const std::string& str) {
            push(str.size());
            m_ref.insert(m_ref.end(), str.data(), str.data() + (sizeof(std::string::value_type) * str.size()));
        }

        template<typename T> 
        void push(const std::vector<T>& container) {
            push(container.size());
            m_ref.insert(m_ref.end(), container.data(), container.data() + (sizeof(T) * container.size()));
        }

    private:
        std::vector<uint8_t>& m_ref;
    };

    class dv_bin_reader {
    public:
        dv_bin_reader() = default;
        dv_bin_reader(const uint8_t* src, size_t size) {
            set(src, size);
        }

        void set(const uint8_t* src, size_t size) {
            m_src = (uint8_t*)src;
            m_size = size;
        }

        template <typename T>
        dv_bin_reader& operator>>(T& obj) {
            pop(obj);
            return *this;
        }

    private:
        uint8_t* m_src   = nullptr;
        size_t m_size = 0U;

    private:
        template <typename T>
        void pop(T& obj, typename std::enable_if<std::is_arithmetic<T>::value, bool>::type = true) {
            if (m_size - sizeof(obj) < 0)
                throw std::out_of_range("out of bounds, buffer doesn't have more data");

            std::memcpy(&obj, m_src, sizeof(obj));

            m_size -= sizeof(obj);
            m_src  += sizeof(obj);
        }

        void pop(std::string& str) {
            size_t size = 0;
            pop(size);

            str.resize(size);
            str.clear();

            size_t len = sizeof(std::string::value_type) * size;
            if (m_size - len < 0)
                throw std::out_of_range("out of bounds, buffer doesn't have more data");

            str.insert(str.begin(), m_src, m_src + len);

            m_size -= len;
            m_src  += len;
        }

        template<typename T> 
        void pop(std::vector<T>& container) {
            size_t size = 0;
            pop(size);

            container.resize(size);
            container.clear();

            size_t len = sizeof(T) * size;
            if (m_size - len < 0)
                throw std::out_of_range("out of bounds, buffer doesn't have more data");

            container.insert(container.begin(), m_src, m_src + len);

            m_size -= len;
            m_src  += len;
        }
    };
}