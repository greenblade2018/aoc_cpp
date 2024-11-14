#include "aoc.h"
using namespace aoc;

class CircularList {
public:
    CircularList() {}

    size_t size() const { return m_size; }
    void insert(int n);
    int erase();
    void next(size_t step = 1);
    void prev(size_t step = 1);

    friend std::ostream& operator<<(std::ostream&, const CircularList& cl);

private:
    struct Element {
        int n;
        Element* prev;
        Element* next;
    };

    size_t m_size = 0UL;
    Element* m_current = nullptr;
};

void CircularList::insert(int n) {
    Element *e = new Element {n, nullptr, nullptr};
    if (m_size == 0) {
        e->prev = e->next = e;
    } else if (m_size == 1) {
        e->prev = e->next = m_current;
        m_current->prev = m_current->next = e;
    } else {
        e->prev = m_current->prev;
        e->next = m_current;
        e->prev->next = e;
        e->next->prev = e;
    }
    m_current = e;
    ++m_size;
}

int CircularList::erase() {
    if (m_size == 0) {
        throw std::out_of_range("erase: list is empty");
    }

    auto result = m_current->n;
    auto old = m_current;
    if (m_size == 1) {
        assert(m_current->prev == m_current);
        m_current = nullptr;
    } else {
        m_current = m_current->next;
        m_current->prev = old->prev;
        old->prev->next = m_current;
    }

    delete old;
    --m_size;
    return result;
}

void CircularList::prev(size_t step) {
    for (; step > 0; --step) m_current = m_current->prev;
}

void CircularList::next(size_t step) {
    for (; step > 0; --step) m_current = m_current->next;
}

std::ostream& operator<<(std::ostream& os, const CircularList& cl) {
    os << std::format("[{}]: ", cl.m_size);
    CircularList::Element* it = cl.m_current;
    do {
        os << it->n << ", ";
        it = it->next;
    } while (it != cl.m_current);
    return os;
}

int play(CircularList& marbles, int n) {
    int score = 0;
    if (n % 23 == 0) {
        score += n;
        marbles.prev(7);
        score += marbles.erase();
    } else {
        marbles.next(2);
        marbles.insert(n);
    }
    return score;
}

long solve_one(int player, int high) {
    CircularList marbles;
    marbles.insert(0);
    // std::cerr << marbles << '\n';

    std::vector<long> scores(player, 0);
    for (int p = 0, n = 1; n <= high; ++n) {
        scores[p] += play(marbles, n);
        // std::cerr << std::format("({}, {}) => ", p, n) << marbles << '\n';
        p = (p + 1) % player;
    }
    return *std::max_element(scores.cbegin(), scores.cend());
}

int main() {
    // assert(solve_one(10, 1618) == 8317);
    // assert(solve_one(13, 7999) == 146373);
    print_answer("one", solve_one(418, 71339));
    print_answer("two", solve_one(418, 7133900));
}