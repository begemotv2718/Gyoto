/**
 *  \file GyotoPhoton.h
 *  \brief A single light ray
 *
 *   Gyoto::Photon is a class for ray-tracing.
 *
 */

/*
    Copyright 2011 Frederic Vincent, Thibaut Paumard

    This file is part of Gyoto.

    Gyoto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Gyoto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Gyoto.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GyotoPhoton_H_ 
#define __GyotoPhoton_H_ 


namespace Gyoto{
  class Photon;
  namespace Astrobj { class Generic; }
}

#include <GyotoDefs.h>
#include <GyotoMetric.h>
#include <GyotoScreen.h>
#include <GyotoWorldline.h>

#include <float.h>

/**
 * \class Gyoto::Photon
 * \brief A null geodesic transporting light
 *
 * This is the central object for ray-tracing. 
 */
class Gyoto::Photon : public Gyoto::Worldline, protected Gyoto::SmartPointee {
  friend class Gyoto::SmartPointer<Gyoto::Photon>;
  // Data : 
  // -----

 protected:
  SmartPointer<Gyoto::Astrobj::Generic> object_; ///< The astronomical target
  double freq_obs_; ///< Photon's frequency in observer's frame
  double transmission_freqobs_; ///< integrated optical transmission

  SmartPointer<Spectrometer> spectro_;
  double * transmission_;

  // Constructors - Destructor
  // -------------------------

 public:
  virtual std::string className() const ; ///< "Photon"
  virtual std::string className_l() const ; ///< "photon"

  /**
   * Allocates the default size (GYOTO_DEFAULT_X_SIZE) for x1, x2 etc.
   */
  Photon() ; ///< Default constructor
  Photon(const Photon& ) ;                ///< Copy constructor
  Photon* clone() const ;
  Photon(SmartPointer<Metric::Generic> gg, SmartPointer<Astrobj::Generic> obj,
	 double* coord) ;
  ///< same as Photon() followed by setInitialCondition()
  Photon(SmartPointer<Metric::Generic> gg, SmartPointer<Astrobj::Generic> obj, 
	 SmartPointer<Screen> screen, double d_alpha, double d_delta);
  ///< same as Photon() followed by setInitialCondition()

  // Constructor from a file (see \c sauve(FILE*) )
  //Photon(FILE* ) ;                    
  
  virtual ~Photon() ;                        ///< Destructor
  
  virtual double getMass() const ; ///< Return 0.

  void setAstrobj(SmartPointer<Astrobj::Generic>); ///< Set Astrobj
  SmartPointer<Astrobj::Generic> getAstrobj() const ; ///< Get Astrobj
  void setSpectrometer(SmartPointer<Spectrometer> spr);
  SmartPointer<Spectrometer> getSpectrometer() const ;
  double getFreqObs() const;


  // Mutators / assignment
  // ---------------------
 public:
  /// Assignment to another Photon
  void operator=(const Photon&) ; 

  /**
   * Set initial condition for this Photon :
   *
   * \param gg : Gyoto::SmartPointer to the Gyoto::Metric in this universe;
   *
   * \param obj :  Gyoto::SmartPointer to the target Gyoto::Astrobj;
   *
   * \param coord : 8 element array containing the initial condition,
   *        i.e. the 4-position and the 4-velocity of the Photon at
   *        the receiving end;
   *
   */
  void setInitialCondition(SmartPointer<Metric::Generic> gg, SmartPointer<Astrobj::Generic> obj, const double coord[8]) ;
  ///<Set or re-set the initial condition prior to integration.

  /**
   * Set initial condition for this Photon :
   *
   * \param gg       Gyoto::SmartPointer to the Gyoto::Metric in this universe;
   *
   * \param obj      Gyoto::SmartPointer to the target Gyoto::Astrobj;
   *
   * \param t0       Arrival date at observer's position
   *
   * \param d_alpha  Direction of arrival (RA offset) in radians
   *
   * \param d_delta  Direction of arrival (Dec offset) in radians
   */
  void setInitialCondition(SmartPointer<Metric::Generic> gg, SmartPointer<Astrobj::Generic> obj, SmartPointer<Screen> screen, double d_alpha, double d_delta);
  ///<Set or re-set the initial condition prior to integration.

  //  int hit(double tlim, Astrobj::Properties *data=NULL); ///< Integrate the geodesic and tell whether this photon hits the object
  int hit(Astrobj::Properties *data=NULL); ///< Integrate the geodesic and tell whether this photon hits the object

  /**
   * \brief Find minimum of photon--object distance
   *
   * Return the minimum of object->Astrobj::operator()(this->getCoord())
   * between t1 and t2. The date of this minimum is returned in tmin.
   *
   * \param object the distance to minimize is given by
   *               object->Astrobj::operator()()
   * \param t1   date
   * \param t2   date
   * \param tmin on output, date correspondig to the minimum
   * \param threshold stop searching for a minimum if a value <
   *              threshold is found (very often, we just want to find
   *              a date below the threshold, not the accurate
   *              minimum).
   */
  double findMin(Astrobj::Generic* object, double t1, double t2, double &tmin,
		 double threshold = DBL_MIN) ;

  /**
   * \brief Find date for which the photon is at a given distance from
   * the object
   *
   * \param object the object
   * \param value the value to find
   * \param a date for which
   *        object->Astrobj::operator()(Photon::getCoord()) is < value
   * \param on input: a date for which
   *        object->Astrobj::operator()(Photon::getCoord()) is >
   *        value.  on output, (*object)(getCoord(toutside)) is <
   *        value, very close to value. toutside is closer to tinside
   *        on output than on input.
   */
  void findValue(Astrobj::Generic* object, double value,
		 double tinside, double &toutside) ;

#ifdef GYOTO_USE_XERCES
 public:
    void fillElement(FactoryMessenger *fmp); /// < called from Factory
#endif

    /* transmission stuff */
 public:
    /**
     * Set transmission to 1 for each channel as well as scalar transmission
     */
    void resetTransmission() ;
    /**
     * Get either the transmission at freqObs (with i=-1) or the
     * transmission at spectro_->getMidpoints()[i].
     *
     * \param i channel number of the requested frequency, -1 for getFreqObs().
     */ 
    double getTransmission(size_t i) const ;
         ///< get transmission for freqobs

    /**
     * For i!=-1: getTansmission()[i] == getTransmission(size_t i)
     */
    double const * getTransmission() const ;
         ///< get pointer to transmission array

    /**
     * Update transmission in a given channel:
     *
     * getTransmission(size_t i) *= t.
     *
     * \param i channel number. -1 for getFreqObs().
     * \param t transmission of this fluid element.
     */
    void transmit(size_t i, double t);

 private:
    void _allocateTransmission();
 
};

#ifdef GYOTO_USE_XERCES
namespace Gyoto {
  SmartPointer<Photon> PhotonSubcontractor(Gyoto::FactoryMessenger*);
}
#endif

#endif
