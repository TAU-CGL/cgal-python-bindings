#ifndef CGALPY_KERNEL_TYPE_HPP
#define CGALPY_KERNEL_TYPE_HPP

#include "CGALPY/kernel_config.hpp"

#if CGALPY_KERNEL == CGALPY_KERNEL_EPIC
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#elif CGALPY_KERNEL == CGALPY_KERNEL_EPEC
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#elif CGALPY_KERNEL == CGALPY_KERNEL_EPEC_WITH_SQRT
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#elif CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_DOUBLE
#include <CGAL/Filtered_kernel.h>
#include <CGAL/Simple_cartesian.h>
#elif CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ
#include <CGAL/Filtered_kernel.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Lazy_exact_nt.h>
#include <CGAL/Gmpq.h>
#elif CGALPY_KERNEL == CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL
#include <CGAL/Cartesian.h>
#include <CGAL/CORE_algebraic_number_traits.h>
#elif CGALPY_KERNEL == CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2
#include <CGAL/Exact_circular_kernel_2.h>
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_KERNEL");
#endif

#include <CGAL/CORE_BigInt.h>
#include <CGAL/Sqrt_extension.h>
#if CGALPY_KERNEL == CGALPY_KERNEL_EPIC
using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
#elif CGALPY_KERNEL == CGALPY_KERNEL_EPEC
using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
#elif CGALPY_KERNEL == CGALPY_KERNEL_EPEC_WITH_SQRT
using Kernel = CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt;
#elif CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_DOUBLE
using NT = double;
using Kernel = CGAL::Filtered_kernel<CGAL::Simple_cartesian<NT>>;
#elif CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ
using NT = CGAL::Lazy_exact_nt<CGAL::Gmpq>;
using Kernel = CGAL::Filtered_kernel<CGAL::Simple_cartesian<NT>>;

#elif CGALPY_KERNEL == CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL
using Nt_traits = CGAL::CORE_algebraic_number_traits;
using NT = Nt_traits::Rational;
using Kernel = CGAL::Cartesian<NT>;

#elif CGALPY_KERNEL == CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2
using Kernel = CGAL::Exact_circular_kernel_2;

#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_KERNEL");
#endif

#endif
