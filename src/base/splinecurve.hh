/* 
 */

/*

  Copyright (C) 2014 Ferrero Andrea

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.


*/

/*

  These files are distributed with PhotoFlow - http://aferrero2707.github.io/PhotoFlow/

*/


#ifndef SPLINE_CURVE_H
#define SPLINE_CURVE_H

#include "property.hh"
#include "curve.hh"

#define SPLINE_USE_STDVEC 1


namespace PF
{

  class SplineCurve: public Curve
  {
#ifdef SPLINE_USE_STDVEC
    std::vector< std::pair<float,float> > points;
#else
    std::pair<float,float>* points;
    size_t npoints;
#endif

    GMutex* points_mutex;
    
    double* ypp;
    unsigned int ypp_size;

  public:
    SplineCurve();
    ~SplineCurve();

    void lock() { g_mutex_lock( points_mutex); }
    void unlock() { g_mutex_unlock( points_mutex); }

    int add_point( float x, float y );

    bool remove_point( unsigned int id );

    void clear_points() { 
#ifdef SPLINE_USE_STDVEC
      points.clear();
#else
      delete[] points;
      points = NULL;
      npoints = 0;
#endif
    }

    bool set_point( unsigned int id, float& x, float& y );

#ifdef SPLINE_USE_STDVEC
    const std::vector< std::pair<float,float> >& get_points() const { return points; }
    size_t get_npoints() const { return points.size(); }
#else
    const std::pair<float,float>* get_points() const { return points; }
    size_t get_npoints() const { return npoints; }
#endif
    std::pair<float,float> get_point(int n) const { return points[n]; }

    void update_spline();

    // Get the output value corresponding to an input value x (normalized to the [0,1] range)
    float get_value( float x );

    // Get the output delta corresponding to an input value x (normalized to the [0,1] range)
    float get_delta( float x );

    // Fill a vector of equally-spaced points with input-output value pairs
    void get_values( std::vector< std::pair<float,float> >& vec );

    // Fill a vector of equally-spaced points with input-output value deltas
    void get_deltas( std::vector< std::pair<float,float> >& vec );
    SplineCurve& operator=(const SplineCurve& b)
    {
      lock();
#ifdef SPLINE_USE_STDVEC
      points = b.get_points();
#else
      if( points ) delete[] points;
      npoints = b.get_npoints();
      points = new std::pair<float,float>[npoints];
      for(size_t i = 0; i < npoints; i++) 
        points[i] = b.get_point(i);
#endif
      update_spline();
      unlock();
      return *this;
    } 
  };


  inline bool operator ==(const SplineCurve& l, const SplineCurve& r)
  {
#ifdef SPLINE_USE_STDVEC
    if( l.get_points() != r.get_points() ) return false;
#else
    if( l.get_npoints() != r.get_npoints() ) return false;
    for( size_t i = 0; i < l.get_npoints(); i++ ) {
      if( l.get_point(i) != r.get_point(i) ) return false;
    }
#endif
    return true;
  }

  inline bool operator !=(const SplineCurve& l, const SplineCurve& r)
  {
    return( !(l==r) );
  }



  //template<>
  //void set_gobject_property<SplineCurve>(gpointer object, const std::string name, const std::string& value);

  template<>
  class Property<SplineCurve>: public PropertyBase
  {
    SplineCurve curve;
  public:
    Property(std::string name, OpParBase* par): PropertyBase(name, par), curve() {}

    void set(const SplineCurve& newval) { 
      if( curve != newval )
        modified();
      curve = newval; 
    }

    SplineCurve& get() { return curve; }

    void from_stream(std::istream& str)
    {
      SplineCurve oldcurve = curve;
      int npoints;
      str>>npoints;
      if( npoints > 0 ) curve.clear_points();
      for( int i = 0; i < npoints; i++ ) {
        float x, y;
        str>>x>>y;
        curve.add_point( x, y );
      }
      if( oldcurve != curve )
        modified();
      //str>>value;
    }

    void to_stream(std::ostream& str)
    {
      //std::vector<std::pair<float, float> > points = curve.get_points();
      str<<curve.get_npoints();
      for( unsigned int i = 0; i < curve.get_npoints(); i++ )
    		str<<" "<<curve.get_point(i).first<<" "<<curve.get_point(i).second;
    	//str<<value;
    }

    bool import(PropertyBase* pin)
    {
      Property<SplineCurve>* pin2 = dynamic_cast< Property<SplineCurve>* >( pin );
      if( pin2 ) {
        set( pin2->get() );
      } else {
        set_str( pin->get_str() );
      }
      return true;
    }

    void set_gobject(gpointer object)
    {
      //g_object_set( object, get_name().c_str(), value, NULL );
      //set_gobject_property( object, get_name(), value );
    }
  };
}


#endif
