#pragma once

#include <engine.h>

namespace orianna {

namespace engine {

class Naive_Algorithm : public Algorithm
{
  public:
    Color_Column get_next_coin_color_and_column(Engine_Difficulty d) override;

  private:
    Color_Column next_coin_decision_as_beginner_ai() override;
    Color_Column next_coin_decision_as_hard_ai() override;
};

} // namespace engine

} // namespace orianna
