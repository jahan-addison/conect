#pragma once

#include <engine.h>

namespace linea
{

namespace engine
{

class Naive_Algorithm : public Algorithm
{
  public:
    virtual Color_Column get_next_coin_color_and_column(Engine_Difficulty d) override;

  private:
    virtual Color_Column next_coin_decision_as_beginner_ai() override;
    virtual Color_Column next_coin_decision_as_hard_ai() override;
};

} // namespace engine

} // namespace linea
