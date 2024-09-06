#ifndef CGALPY_KERNEL_TYPE_HPP
#define CGALPY_KERNEL_TYPE_HPP

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
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_KERNEL");
#endif

#include <CGAL/CORE_BigInt.h>
#include <CGAL/Sqrt_extension.h>
#if CGALPY_KERNEL == CGALPY_KERNEL_EPIC
typedef CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;
#elif CGALPY_KERNEL == CGALPY_KERNEL_EPEC
typedef CGAL::Exact_predicates_exact_constructions_kernel       Kernel;
#elif CGALPY_KERNEL == CGALPY_KERNEL_EPEC_WITH_SQRT
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt
                                                                Kernel;
#elif CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_DOUBLE
typedef double                                                  NT;
typedef CGAL::Filtered_kernel<CGAL::Simple_cartesian<NT>>       Kernel;
#elif CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ
typedef CGAL::Lazy_exact_nt<CGAL::Gmpq>                         NT;
typedef CGAL::Filtered_kernel<CGAL::Simple_cartesian<NT>>       Kernel;

#elif CGALPY_KERNEL == CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL
using Nt_traits = CGAL::CORE_algebraic_number_traits;
using NT = Nt_traits::Rational;
using Kernel = CGAL::Cartesian<NT>;

#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_KERNEL");
#endif

#endif

