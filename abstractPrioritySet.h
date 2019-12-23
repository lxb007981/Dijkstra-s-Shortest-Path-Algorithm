#ifndef ABSTRACTPRIORITYSET_H
#define ABSTRACTPRIORITYSET_H
#include <climits>
#include <iostream>

class myPair
{
public:
    myPair(int vertex = -1, int distance = INT_MAX) : vertex(vertex), distance(distance) {}
    int vertex{-1};
    int distance{INT_MAX};
    myPair(const myPair &b)
    {
        *this = b;
    }
    myPair &operator=(const myPair &b)
    {
        if (this != &b) // Avoid self-assignment
        {
            vertex = b.vertex;
            distance = b.distance;
        }
        return *this;
    };
    friend bool operator>(const myPair &a, const myPair &b);
    friend bool operator<(const myPair &a, const myPair &b);
    friend bool operator==(const myPair &a, const myPair &b);
};
inline bool operator>(const myPair &a, const myPair &b)
{
    return a.distance > b.distance;
}
inline bool operator<(const myPair &a, const myPair &b)
{
    return a.distance < b.distance;
}
inline bool operator==(const myPair &a, const myPair &b)
{
    return a.distance == b.distance;
}

inline std::ostream &operator<<(std::ostream &os, const myPair &a)
{
    return (os << a.distance);
}
class abstractPrioritySet
{
public:
    virtual ~abstractPrioritySet() = 0;
    virtual void insert(const myPair &) = 0;
    virtual bool is_empty() const = 0;
    virtual void print(int depth = 0) const = 0;
    virtual void remove(const myPair &) = 0;
    virtual const myPair &find_min_value() const = 0;
};
#endif // ABSTRACTPRIORITYSET_H
