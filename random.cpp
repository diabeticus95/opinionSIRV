#include "random.h"

bool operator==(pcg const &lhs, pcg const &rhs)
{
    return lhs.m_state == rhs.m_state
        && lhs.m_inc == rhs.m_inc;
}
bool operator!=(pcg const &lhs, pcg const &rhs)
{
    return lhs.m_state != rhs.m_state
        || lhs.m_inc != rhs.m_inc;
}
