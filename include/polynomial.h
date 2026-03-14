#ifndef POLYMONIAL_H
#define POLYMONIAL_H

#include "list.h"
#include <string>
#include <sstream>
#include <cmath>
struct Monom {
    double coeff;
    int i, j, k;

    Monom() : coeff(0), i(0), j(0), k(0) {}
    Monom(double c, int x_pow, int y_pow, int z_pow)
        : coeff(c), i(x_pow), j(y_pow), k(z_pow) {}
    bool operator<(const Monom& other) const {
        if (i != other.i) return i > other.i;
        if (j != other.j) return j > other.j;
        return k > other.k;
    }

    bool operator==(const Monom& other) const {
        return i == other.i && j == other.j && k == other.k;
    }

    bool operator>(const Monom& other) const {
        return other < *this;
    }
    Monom operator*(const Monom& other) const {
        return Monom(coeff * other.coeff, i + other.i, j + other.j, k + other.k);
    }
    Monom operator*(double c) const {
        return Monom(coeff * c, i, j, k);
    }
    bool is_zero() const {
        return std::abs(coeff) < 1e-10;
    }
    std::string to_string() const {
        if (is_zero()) return "";

        std::stringstream ss;
        if (i == 0 && j == 0 && k == 0) {
            ss << coeff;
            return ss.str();
        }

        if (std::abs(coeff - 1.0) > 1e-10 && std::abs(coeff + 1.0) > 1e-10) {
            ss << coeff << "*";
        } else if (std::abs(coeff + 1.0) < 1e-10) {
            ss << "-";
        }
        bool first = true;
        if (i > 0) {
            ss << "x";
            if (i > 1) ss << "^" << i;
            first = false;
        }
        if (j > 0) {
            if (!first) ss << "*";
            ss << "y";
            if (j > 1) ss << "^" << j;
            first = false;
        }
        if (k > 0) {
            if (!first) ss << "*";
            ss << "z";
            if (k > 1) ss << "^" << k;
        }

        return ss.str();
    }
};

class Polynomial {
private:
    List<Monom> monoms;
    void insert_sorted(const Monom& m) {
        if (m.is_zero()) return;
        if (monoms.empty()) {
            monoms.push_front(m);
            return;
        }
        if (m < monoms.front()) {
            monoms.push_front(m);
            return;
        }
        auto prev = monoms.begin();
        auto curr = monoms.begin();
        ++curr;
        if (m == *prev) {
            prev->coeff += m.coeff;
            if (prev->is_zero()) {
                monoms.pop_front();
            }
            return;
        }
        while (curr != monoms.end()) {
            if (m == *curr) {
                curr->coeff += m.coeff;
                if (curr->is_zero()) {
                    monoms.erase_after(prev);
                }
                return;
            }
            if (m < *curr) {
                monoms.insert_after(prev, m);
                return;
            }
            ++prev;
            ++curr;
        }
        monoms.insert_after(prev, m);
    }

    static void append_to_result(Polynomial& result, List<Monom>::Iterator& tail, bool& has_tail, const Monom& m) {
        if (m.is_zero()) return;
        if (!has_tail) {
            result.monoms.push_front(m);
            tail = result.monoms.begin();
            has_tail = true;
        } else {
            tail = result.monoms.insert_after(tail, m);
        }
    }

public:
    Polynomial() {}
    void add_monom(double coeff, int x_pow, int y_pow, int z_pow) {
        insert_sorted(Monom(coeff, x_pow, y_pow, z_pow));
    }
    Polynomial operator+(const Polynomial& other) const {
        Polynomial result;
        auto it1 = monoms.begin();
        auto it2 = other.monoms.begin();

        List<Monom>::Iterator tail;
        bool has_tail = false;

        // Cлияние отсортированных списков
        while (it1 != monoms.end() && it2 != other.monoms.end()) {
            if (*it1 == *it2) {
                Monom s(it1->coeff + it2->coeff, it1->i, it1->j, it1->k);
                append_to_result(result, tail, has_tail, s);
                ++it1;
                ++it2;
            } else if (*it1 < *it2) {
                append_to_result(result, tail, has_tail, *it1);
                ++it1;
            } else {
                append_to_result(result, tail, has_tail, *it2);
                ++it2;
            }
        }
        while (it1 != monoms.end()) {
            append_to_result(result, tail, has_tail, *it1);
            ++it1;
        }
        while (it2 != other.monoms.end()) {
            append_to_result(result, tail, has_tail, *it2);
            ++it2;
        }

        return result;
    }
    Polynomial operator-(const Polynomial& other) const {
        Polynomial result;
        auto it1 = monoms.begin();
        auto it2 = other.monoms.begin();
        while (it1 != monoms.end() && it2 != other.monoms.end()) {
            if (*it1 == *it2) {
                Monom m(it1->coeff - it2->coeff, it1->i, it1->j, it1->k);
                if (!m.is_zero()) {
                    result.insert_sorted(m);
                }
                ++it1;
                ++it2;
            } else if (*it1 < *it2) {
                result.insert_sorted(*it1);
                ++it1;
            } else {
                Monom m(-it2->coeff, it2->i, it2->j, it2->k);
                result.insert_sorted(m);
                ++it2;
            }
        }
        while (it1 != monoms.end()) {
            result.insert_sorted(*it1);
            ++it1;
        }
        while (it2 != other.monoms.end()) {
            Monom m(-it2->coeff, it2->i, it2->j, it2->k);
            result.insert_sorted(m);
            ++it2;
        }

        return result;
    }
    Polynomial operator*(double c) const {
        Polynomial result;
        for (auto it = monoms.begin(); it != monoms.end(); ++it) {
            result.insert_sorted(*it * c);
        }
        return result;
    }
    Polynomial operator*(const Polynomial& other) const {
        Polynomial result;

        for (auto it1 = monoms.begin(); it1 != monoms.end(); ++it1) {
            for (auto it2 = other.monoms.begin(); it2 != other.monoms.end(); ++it2) {
                Monom m = (*it1) * (*it2);
                result.insert_sorted(m);
            }
        }

        return result;
    }
    std::string to_string() const {
        if (monoms.empty()) return "0";
        std::ostringstream out;

        for (auto it = monoms.begin(); it != monoms.end(); ++it) {
            if (it->is_zero()) continue;

            if (it->coeff > 0) {
                if (it != monoms.begin()) out << " + ";
            } else {
                out << (it == monoms.begin() ? "-" : " - ");
            }

            Monom m = *it;
            m.coeff = std::abs(m.coeff);
            out << m.to_string();
        }

        std::string res = out.str();
        return res.empty() ? "0" : res;
    }

    bool empty() const {
        return monoms.empty();
    }
};

#endif // POLYMONIAL_H
