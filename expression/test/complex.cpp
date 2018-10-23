/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include "gtest/gtest.h"
#include <alps/expression.hpp>

typedef std::complex<double> complex;

TEST(ExpressionTest, Complex) {
  EXPECT_EQ(alps::evaluate<complex>("3"), complex(3));
  EXPECT_EQ(alps::evaluate<complex>("2+3*I"), complex(2,3));
  EXPECT_EQ(alps::evaluate<complex>("2/I"), 2.0 / complex(0,1));
  EXPECT_EQ(alps::evaluate<complex>("sqrt(-2)"), sqrt(-complex(2)));
  EXPECT_EQ(alps::evaluate<complex>("sin(2+3*I)"), sin(complex(2,3)));
}
