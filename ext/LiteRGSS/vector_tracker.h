#ifndef L_VECTOR_TRACKER_HEADER
#define L_VECTOR_TRACKER_HEADER
#include <algorithm>
#include <vector>

template <class T>
class vector_tracker {
public:
    template <class X>
    vector_tracker(std::vector<X> defaultValue) : m_container(std::move(defaultValue)){}
    vector_tracker() = default;
    vector_tracker(vector_tracker&&) noexcept = default;
    vector_tracker(const vector_tracker&) = default;
    vector_tracker& operator=(vector_tracker&&) noexcept = default;
    vector_tracker& operator=(const vector_tracker&) = default;

    void push_back(T el) {
        auto place = firstAvailableBeforeDeletedIterators();
        if(place == m_container.end() || ++place == m_container.end()) {
            m_container.push_back(std::move(el));
        } else {
            *place = std::move(el);
            m_invalids--;
        }
    }
    
    bool remove(const T& el) {    
        auto lastValidElementIterator = firstAvailableBeforeDeletedIterators();
        if(lastValidElementIterator != m_container.end()) {
            auto elementToDeleteIterator = std::find(m_container.begin(), m_container.end(), el);
            if(elementToDeleteIterator != m_container.end()) {
                /*
                const auto positioningPercents = static_cast<std::size_t>(((elementToDeleteIterator - m_container.begin())*100.f)/size());
                std::cout << (elementToDeleteIterator - m_container.begin()) << " on " << size() << " (" << positioningPercents << "%) with " << m_invalids << " more invalid items" << std::endl;
                */
                std::iter_swap(elementToDeleteIterator, lastValidElementIterator);
                m_invalids++;
                return true;
            }
        }
        return false;    
    }
    
    std::size_t size() const { return m_container.size() - m_invalids; }

    void clear() { m_container.clear(); m_invalids = 0; }

    auto begin() const { return m_container.begin(); }
    auto begin() { return m_container.begin(); }
    auto end() const { return m_container.empty() ? m_container.end() : (m_container.end() - m_invalids); }
    auto end() { return m_container.empty() ? m_container.end() : (m_container.end() - m_invalids); }
    
private:
    auto firstAvailableBeforeDeletedIterators() {
        return m_container.empty() ? m_container.end() : (m_container.end() - m_invalids - 1);
    }

    std::vector<T> m_container;
    std::size_t m_invalids = 0;
};

#endif