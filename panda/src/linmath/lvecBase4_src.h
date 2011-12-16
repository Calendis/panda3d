// Filename: lvecBase4_src.h
// Created by:  drose (08Mar00)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////

class FLOATNAME(LVecBase2);
class FLOATNAME(LVecBase3);

////////////////////////////////////////////////////////////////////
//       Class : LVecBase4
// Description : This is the base class for all three-component
//               vectors and points.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_LINMATH LINMATH_ALIGN FLOATNAME(LVecBase4) {
PUBLISHED:
  typedef const FLOATTYPE *iterator;
  typedef const FLOATTYPE *const_iterator;

  INLINE_LINMATH FLOATNAME(LVecBase4)();
  INLINE_LINMATH FLOATNAME(LVecBase4)(const FLOATNAME(LVecBase4) &copy);
  INLINE_LINMATH FLOATNAME(LVecBase4) &operator = (const FLOATNAME(LVecBase4) &copy);
  INLINE_LINMATH FLOATNAME(LVecBase4) &operator = (FLOATTYPE fill_value);
  INLINE_LINMATH FLOATNAME(LVecBase4)(FLOATTYPE fill_value);
  INLINE_LINMATH FLOATNAME(LVecBase4)(FLOATTYPE x, FLOATTYPE y, FLOATTYPE z, FLOATTYPE w);
  ALLOC_DELETED_CHAIN(FLOATNAME(LVecBase4));

  INLINE_LINMATH static const FLOATNAME(LVecBase4) &zero();
  INLINE_LINMATH static const FLOATNAME(LVecBase4) &unit_x();
  INLINE_LINMATH static const FLOATNAME(LVecBase4) &unit_y();
  INLINE_LINMATH static const FLOATNAME(LVecBase4) &unit_z();
  INLINE_LINMATH static const FLOATNAME(LVecBase4) &unit_w();

  INLINE_LINMATH ~FLOATNAME(LVecBase4)();

  EXTENSION(PyObject *__reduce__(PyObject *self) const);
  EXTENSION(PyObject *__getattr__(const string &attr_name) const);
  EXTENSION(int __setattr__(PyObject *self, const string &attr_name, PyObject *assign));

  INLINE_LINMATH FLOATTYPE operator [](int i) const;
  INLINE_LINMATH FLOATTYPE &operator [](int i);
  EXTENSION(INLINE_LINMATH void __setitem__(int i, FLOATTYPE v));
  INLINE_LINMATH static int size();

  INLINE_LINMATH bool is_nan() const;

  INLINE_LINMATH FLOATTYPE get_cell(int i) const;
  INLINE_LINMATH FLOATTYPE get_x() const;
  INLINE_LINMATH FLOATTYPE get_y() const;
  INLINE_LINMATH FLOATTYPE get_z() const;
  INLINE_LINMATH FLOATTYPE get_w() const;

  INLINE_LINMATH void set_cell(int i, FLOATTYPE value);
  INLINE_LINMATH void set_x(FLOATTYPE value);
  INLINE_LINMATH void set_y(FLOATTYPE value);
  INLINE_LINMATH void set_z(FLOATTYPE value);
  INLINE_LINMATH void set_w(FLOATTYPE value);

  // These next functions add to an existing value.
  // i.e. foo.set_x(foo.get_x() + value)
  // These are useful to reduce overhead in scripting
  // languages:
  INLINE_LINMATH void add_to_cell(int i, FLOATTYPE value);
  INLINE_LINMATH void add_x(FLOATTYPE value);
  INLINE_LINMATH void add_y(FLOATTYPE value);
  INLINE_LINMATH void add_z(FLOATTYPE value);
  INLINE_LINMATH void add_w(FLOATTYPE value);

  INLINE_LINMATH const FLOATTYPE *get_data() const;
  INLINE_LINMATH int get_num_components() const;

public:
  INLINE_LINMATH iterator begin();
  INLINE_LINMATH iterator end();

  INLINE_LINMATH const_iterator begin() const;
  INLINE_LINMATH const_iterator end() const;

PUBLISHED:
  INLINE_LINMATH void fill(FLOATTYPE fill_value);
  INLINE_LINMATH void set(FLOATTYPE x, FLOATTYPE y, FLOATTYPE z, FLOATTYPE w);

  INLINE_LINMATH FLOATTYPE length() const;
  INLINE_LINMATH FLOATTYPE length_squared() const;
  INLINE_LINMATH bool normalize();

  INLINE_LINMATH FLOATTYPE dot(const FLOATNAME(LVecBase4) &other) const;
  INLINE_LINMATH FLOATNAME(LVecBase4) project(const FLOATNAME(LVecBase4) &onto) const;

  INLINE_LINMATH bool operator < (const FLOATNAME(LVecBase4) &other) const;
  INLINE_LINMATH bool operator == (const FLOATNAME(LVecBase4) &other) const;
  INLINE_LINMATH bool operator != (const FLOATNAME(LVecBase4) &other) const;

  INLINE_LINMATH int compare_to(const FLOATNAME(LVecBase4) &other) const;
  INLINE_LINMATH int compare_to(const FLOATNAME(LVecBase4) &other,
                                FLOATTYPE threshold) const;
  INLINE_LINMATH size_t get_hash() const;
  INLINE_LINMATH size_t get_hash(FLOATTYPE threshold) const;
  INLINE_LINMATH size_t add_hash(size_t hash) const;
  INLINE_LINMATH size_t add_hash(size_t hash, FLOATTYPE threshold) const;

  INLINE_LINMATH FLOATNAME(LVecBase4) operator - () const;

  INLINE_LINMATH FLOATNAME(LVecBase4)
  operator + (const FLOATNAME(LVecBase4) &other) const;
  INLINE_LINMATH FLOATNAME(LVecBase4)
  operator - (const FLOATNAME(LVecBase4) &other) const;

  INLINE_LINMATH FLOATNAME(LVecBase4) operator * (FLOATTYPE scalar) const;
  INLINE_LINMATH FLOATNAME(LVecBase4) operator / (FLOATTYPE scalar) const;

  INLINE_LINMATH void operator += (const FLOATNAME(LVecBase4) &other);
  INLINE_LINMATH void operator -= (const FLOATNAME(LVecBase4) &other);

  INLINE_LINMATH void operator *= (FLOATTYPE scalar);
  INLINE_LINMATH void operator /= (FLOATTYPE scalar);

  INLINE_LINMATH FLOATNAME(LVecBase4) fmax(const FLOATNAME(LVecBase4) &other);
  INLINE_LINMATH FLOATNAME(LVecBase4) fmin(const FLOATNAME(LVecBase4) &other);

  INLINE_LINMATH bool almost_equal(const FLOATNAME(LVecBase4) &other,
                           FLOATTYPE threshold) const;
  INLINE_LINMATH bool almost_equal(const FLOATNAME(LVecBase4) &other) const;

  INLINE_LINMATH void output(ostream &out) const;
  EXTENSION(INLINE_LINMATH void python_repr(ostream &out, const string &class_name) const);

  INLINE_LINMATH void generate_hash(ChecksumHashGenerator &hashgen) const;
  INLINE_LINMATH void generate_hash(ChecksumHashGenerator &hashgen,
                                    FLOATTYPE threshold) const;

  INLINE_LINMATH void write_datagram_fixed(Datagram &destination) const;
  INLINE_LINMATH void read_datagram_fixed(DatagramIterator &source);
  INLINE_LINMATH void write_datagram(Datagram &destination) const;
  INLINE_LINMATH void read_datagram(DatagramIterator &source);

public:
  // The underlying implementation is via the Eigen library, if available.
  typedef LINMATH_MATRIX(FLOATTYPE, 1, 4) EVector4;
  EVector4 _v;

  INLINE_LINMATH FLOATNAME(LVecBase4)(const EVector4 &v) : _v(v) { }

private:
  static const FLOATNAME(LVecBase4) _zero;
  static const FLOATNAME(LVecBase4) _unit_x;
  static const FLOATNAME(LVecBase4) _unit_y;
  static const FLOATNAME(LVecBase4) _unit_z;
  static const FLOATNAME(LVecBase4) _unit_w;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type();

private:
  static TypeHandle _type_handle;
};

INLINE ostream &operator << (ostream &out, const FLOATNAME(LVecBase4) &vec) {
  vec.output(out);
  return out;
}

#include "lvecBase4_src.I"
