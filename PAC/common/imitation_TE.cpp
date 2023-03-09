#include "imitation_TE.h"

imitation_TE::imitation_TE(float dispersion = 0.238f, float m_expec = 27.f, float min_TE = 20.f, float max_TE = 30.f)
    : dispersion(dispersion), m_expec(m_expec), min_TE(min_TE), max_TE(max_TE) 
    {
        initial_arrays(min_TE, max_TE);
        st_deviation = get_st_deviation();   
    }


float imitation_TE::get_random() const
  {
    unsigned int index_flptr = get_index();
    unsigned int index_iptr = get_index();
    const float real = 0.2f; // вспомогательная величина для генерации случайных вещественных чисел 
    return iptr[index_flptr] * real + flptr[index_iptr];
  }

void imitation_TE::set_max(float max_in_range)
   {
      this->max_TE = max_in_range;
   }

float imitation_TE::get_max() const
    {
      return max_TE;
    }

void imitation_TE::set_min(float min_in_range)
    {
      this->min_TE = min_in_range;
    }

float imitation_TE::get_min() const
    {
      return min_TE;
    }

bool imitation_TE::is_p() const
        {
            // f(x)=1/(σ√2π)∙e^(-1/2∙((x-μ)/σ)^2) - функция Гаусса
            // σ - стандартное отклонение
            // x - случайная величина
            // μ - математическое ожидание
            constexpr double two_pi = 2 * M_PI;
            return (1/(st_deviation * sqrt(two_pi))) *
                      exp(-(pow(x - m_expec, 2) / (2 * st_deviation*st_deviation))) > 0.01;
        }   // 0.01 вероятность того, что случайная величина находится на [25,30]

float imitation_TE::get_st_deviation() const
    {
      return float(sqrt(dispersion));
    }

unsigned imitation_TE::get_index() const
    {
      auto value = unsigned(std::time(nullptr));
      unsigned int rd = value % 10;
      return rd;
}

void imitation_TE::initial_arrays(float min, float max) 
{
      const auto ptr_size = static_cast<std::size_t>(max - min);
      iptr = std::unique_ptr<float[]>(new float[ptr_size]); // массив для генерации случайных вещественных чисел
      flptr= std::unique_ptr<float[]>(new float[ptr_size]);  // массив с заданным диапазоном значений температуры
      std::iota(iptr.get(), iptr.get() + ptr_size, 1);
      std::iota(flptr.get(), flptr.get() + ptr_size, min_TE);
      st_deviation = get_st_deviation();
}

float imitation_TE::get_TE()
    {
      x = get_random();
      static float old_value=x;
      if (is_p())
        {
          old_value = x;
          return x;
        }
      else
        {
          return old_value;
        }
}


