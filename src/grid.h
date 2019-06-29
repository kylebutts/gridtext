#ifndef GRID_H
#define GRID_H

#include <Rcpp.h>
using namespace Rcpp;

#include <string>
using namespace std;

#include "length.h"

// This file contains a number of convenience functions that allow for the rapid construction
// or manipulation of grid units or grobs. Each could be replaced by a simple R call to a
// corresponding grid function (e.g., unit_pt(x) is equivalent to unit(x, "pt")), but in general
// the C++ version here is much faster, in particular because it skips extensive input validation.


// replacement for unit(x, "pt")
// [[Rcpp::export]]
NumericVector unit_pt(NumericVector x) {
  NumericVector out(x);
  out.attr("class") = "unit";
  out.attr("valid.unit") = IntegerVector(1, 8);
  out.attr("unit") = "pt";

  return out;
}

// Overloaded version for Length
NumericVector unit_pt(Length x) {
  NumericVector out(1, x);
  out.attr("class") = "unit";
  out.attr("valid.unit") = IntegerVector(1, 8);
  out.attr("unit") = "pt";

  return out;
}


// replacement for gpar() with no arguments
// [[Rcpp::export]]
List gpar_empty() {
  List out;
  out.attr("class") = "gpar";

  return out;
}


// replacement for textGrop(label, x_pt, y_pt, gp = gpar(), hjust = 0, vjust = 0, default.units = "pt", name = NULL)
// [[Rcpp::export]]
List text_grob(CharacterVector label, NumericVector x_pt = 0, NumericVector y_pt = 0, RObject gp = R_NilValue,
               RObject name = R_NilValue) {
  if (label.size() != 1 || x_pt.size() != 1 || y_pt.size() != 1) {
    stop("Function text_grob() is not vectorized.\n");
  }

  if (gp.isNULL()) {
    gp = gpar_empty();
  }

  // need to produce a unique name for each grob, otherwise grid gets grumpy
  static int tg_count = 0;
  if (name.isNULL()) {
    tg_count += 1;
    string s("gridtext.text.");
    s = s + to_string(tg_count);
    CharacterVector vs;
    vs.push_back(s);
    name = vs;
  }

  List out = List::create(
    _["label"] = label, _["x"] = unit_pt(x_pt), _["y"] = unit_pt(y_pt), _["just"] = "centre",
    _["hjust"] = 0., _["vjust"] = 0., _["rot"] = 0.,
      _["check.overlap"] = false, _["name"] = name, _["gp"] = gp, _["vp"] = R_NilValue
  );

  Rcpp::StringVector cl(3);
  cl(0) = "text";
  cl(1) = "grob";
  cl(2) = "gDesc";

  out.attr("class") = cl;

  return out;
}

// replacement for editGrob(grob, x = x, y = y)
// [[Rcpp::export]]
RObject set_grob_coords(RObject grob, NumericVector x, NumericVector y) {
  static_cast<List>(grob)["x"] = x;
  static_cast<List>(grob)["y"] = y;

  return grob;
}

#endif
