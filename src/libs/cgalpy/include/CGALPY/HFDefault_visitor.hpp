#ifndef CGALPY_HFDEFAULT_VISITOR_HPP
#define CGALPY_HFDEFAULT_VISITOR_HPP

#include <nanobind/nanobind.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>

namespace py = nanobind;

namespace PMP = CGAL::Polygon_mesh_processing;
namespace HF = PMP::Hole_filling;

namespace pmp {

struct HFDefault_visitor : HF::Default_visitor {

  HFDefault_visitor() {
    spp = nullptr;
    epp = nullptr;
    sqp = nullptr;
    qs = nullptr;
    ecp = nullptr;
    scp = nullptr;
    cs = nullptr;
    ecup = nullptr;
    srp = nullptr;
    erp = nullptr;
    sfp = nullptr;
    efp = nullptr;
  }

  void start_planar_phase() const {if (spp) spp();}
  void end_planar_phase(bool success) const {if (epp) epp(success);}
  void start_quadratic_phase(std::size_t n) const {if (sqp) sqp(n);}
  void quadratic_step() const {if (qs) qs();}
  void end_quadratic_phase(bool success) const {if (ecp) ecp(success);}
  void start_cubic_phase(int n) const {if (scp) scp(n);}
  void cubic_step() const {if (cs) cs();}
  void end_cubic_phase() const {if (ecup) ecup();}
  void start_refine_phase() const {if (srp) srp();}
  void end_refine_phase() const {if (erp) erp();}
  void start_fair_phase() const {if (sfp) sfp();}
  void end_fair_phase() const {if (efp) efp();}


  void set_start_planar_phase(const std::function<void()>& fn) {spp = fn;}
  void set_end_planar_phase(const std::function<void(bool)>& fn) {epp = fn;}
  void set_start_quadratic_phase(const std::function<void(std::size_t)>& fn) {sqp = fn;}
  void set_quadratic_step(const std::function<void()>& fn) {qs = fn;}
  void set_end_quadratic_phase(const std::function<void(bool)>& fn) {ecp = fn;}
  void set_start_cubic_phase(const std::function<void(int)>& fn) {scp = fn;}
  void set_cubic_step(const std::function<void()>& fn) {cs = fn;}
  void set_end_cubic_phase(const std::function<void()>& fn) {ecup = fn;}
  void set_start_refine_phase(const std::function<void()>& fn) {srp = fn;}
  void set_end_refine_phase(const std::function<void()>& fn) {erp = fn;}
  void set_start_fair_phase(const std::function<void()>& fn) {sfp = fn;}
  void set_end_fair_phase(const std::function<void()>& fn) {efp = fn;}

private:
  std::function<void()> spp;
  std::function<void(bool)> epp;
  std::function<void(std::size_t)> sqp;
  std::function<void()> qs;
  std::function<void(bool)> ecp;
  std::function<void(int)> scp;
  std::function<void()> cs;
  std::function<void()> ecup;
  std::function<void()> srp;
  std::function<void()> erp;
  std::function<void()> sfp;
  std::function<void()> efp;
};

} // namespace pmp

#endif // CGALPY_HFDEFAULT_VISITOR_HPP

