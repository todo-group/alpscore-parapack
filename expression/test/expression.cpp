/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include "gtest/gtest.h"
#include <alps/expression.hpp>

TEST(ExpressionTest, Double) {
  EXPECT_EQ(alps::evaluate<double>("3"), 3);
  EXPECT_EQ(alps::evaluate<double>("3 + 1"), 3 + 1);
  EXPECT_EQ(alps::evaluate<double>("3.5 * 2.1"), 3.5 * 2.1);
  EXPECT_EQ(alps::evaluate<double>("3.5 / 2.1"), 3.5 / 2.1);
  EXPECT_EQ(alps::evaluate<double>("(3.5 + 0.3) / 2.1"), (3.5 + 0.3) / 2.1);
  EXPECT_EQ(alps::evaluate<double>("2 * Pi"), 2 * M_PI);
  EXPECT_EQ(alps::evaluate<double>("2 * sin(Pi/4)"), 2 * std::sin(M_PI/4));
  EXPECT_EQ(alps::evaluate<double>("1 / sqrt(3)"), 1 / std::sqrt(3));
  EXPECT_EQ(alps::evaluate<double>("3.2^1.5"), std::pow(3.2, 1.5));
  EXPECT_ANY_THROW(alps::evaluate<double>("a * b"));
}
