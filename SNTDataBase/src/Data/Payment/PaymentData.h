#pragma once

#include <string>

#include "Data/Money/Money.h"
#include "Data/Date/Date.h"

namespace LM
{
	struct PaymentData
	{
		Money       Amount;
		Date        Date;
		int         FormOfPayment  = 0;
		std::string DocumentNumber = "";

		static inline const std::vector<const char*> s_FormOfPaymentString{ u8"Р/С",  u8"Касса" };
	};

}