#pragma once

#include "Payment.h"

namespace LM
{

    class Accural
    {
    public:
        Date m_Date;
        Money m_Money;
    public:
        static inline Money MembershipFeeAmount = Money(500);
    };

}
