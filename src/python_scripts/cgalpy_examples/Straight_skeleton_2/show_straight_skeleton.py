#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]
CGALPY = importlib.import_module(lib)

Ker = CGALPY.Ker
Point = Ker.Point_2
Pol2 = CGALPY.Pol2
Polygon = Pol2.Polygon_2
Sn2 = CGALPY.Sn2

  Polygon_with_holes pwh ;

  if ( argc > 1 )
  {
    std::string name = argv[1] ;

    std::cout << "Input file: " << name << std::endl ;

    std::ifstream is(name.c_str()) ;
    if ( is )
    {
      is >> pwh ;

      assert(pwh.outer_boundary().is_counterclockwise_oriented());
      for(Polygon_with_holes::Hole_const_iterator it = pwh.holes_begin();
          it != pwh.holes_end();
          ++it){
        assert(it->is_clockwise_oriented());
      }

      //check the validity of the pwh and fix orientation
      if (!pwh.outer_boundary().is_simple())
      {
        std::cerr << "ERROR: outer boundary is not simple.";
        return 1;
      }
      int k=0;
      for (Polygon_with_holes::Hole_iterator it = pwh.holes_begin();
                                             it!=pwh.holes_end(); ++it, ++k)
      {
        if (!it->is_simple())
        {
          std::cerr << "ERROR: hole "<< k << " is not simple.\n";
          return 1;
        }
      }

ss = Sn2.create_interior_straight_skeleton_2(pwh)
      if ( ss )
      {
        std::string eps_name ;
        if ( argc > 2  )
             eps_name = argv[2];
        else eps_name = name + ".skeleton.eps" ;

        std::ofstream eps(eps_name.c_str()) ;
        if ( eps )
        {
          std::cerr << "Result: " << eps_name << std::endl ;
          dump_to_eps(pwh,*ss,eps);
        }
        else
        {
          std::cerr << "Could not open result file: " << eps_name << std::endl ;
        }
      }
      else
      {
        std::cerr << "ERROR creating interior straight skeleton" << std::endl ;
      }
    }
    else
    {
      std::cerr << "Could not open input file: " << name << std::endl ;
    }
  }
  else
  {
    std::cerr << "Computes the straight skeleton in the interior of a polygon with holes and draws it in an EPS file." << std::endl
              << std::endl
              << "Usage: show_straight_skeleton <input_file> [output_eps_file]" << std::endl
              << std::endl
              << "       input_file  Text file describing the input polygon with holes." << std::endl
              << "         (See input_file_format.txt for details" << std::endl
              << "         or use input_file_example.txt)" << std::endl
              << "       output_file     [default='input_file.skeleton.eps']" << std::endl ;
  }

  return 0;
}
