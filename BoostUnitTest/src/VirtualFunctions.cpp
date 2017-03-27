/*
*
 * VirtualDunctions.cpp
 *
 *  Created on: March 26, 2017
 *      Author: Burley
 *      Brief: Demo that  how virtual functions work. Also demo that virtual functions should not be called
 *      in the constructor or the destructor.
 */
 #include <iostream>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <SharedLibrary/inc/MyLogger.hpp>

using namespace std;
using namespace boost;


BOOST_AUTO_TEST_SUITE( virtualFunctions )
BOOST_AUTO_TEST_CASE( test1 )
{
    {


        class B  // (B)ase class
        {
        public:
            B()
            {
                BOOST_TEST_MESSAGE( "B ctor  start");
                // calling a virtual function in the constructor is non-compliant.
                // If the object being constructed is a D object, the call to seize() below will not call D's seize();
                // essentially the virtualization does not work; it cannot work since D is not constructed yet.
                seize();       // non-compliant
                BOOST_TEST_MESSAGE( "B ctor  end" );
            }
            virtual ~B()
            {
                BOOST_TEST_MESSAGE(  "B dtor  start" );
                // calling a virtual function in the destructor is non-compliant.
                // If the object being destructed is a D object, the call to release() below will not call D's release();
                // essentially the virtualizatoon does not work; it cannot since D has already been destructed.
                release();        // non-compliant
                BOOST_TEST_MESSAGE ( "B dtor  end" );
            }
            virtual void seize1()
            {
                BOOST_TEST_MESSAGE(  "B seize1() start" );
                m_seize1_called_b = true;
                seize();
                BOOST_TEST_MESSAGE(  "B seize1() end" );
            }
            void clearb() { m_seize_called_b = m_seize1_called_b = m_release_called_b = false; }
            bool m_seize_called_b  = false;
            bool m_seize1_called_b = false;
            bool m_release_called_b = false;
        protected:
            virtual void seize() { BOOST_TEST_MESSAGE( "B::seize()"); m_seize_called_b = true; }
            virtual void release()  { BOOST_TEST_MESSAGE ( "B::release()" ); m_release_called_b = true; }
        };

        class D : public B  // (D)erived class
        {
        public:
            virtual ~D() {  BOOST_TEST_MESSAGE("D::dtor"); }
            D()          {  BOOST_TEST_MESSAGE("D::ctor"); }
            void seize1() override
            {
                   BOOST_TEST_MESSAGE(  "D seize1() start" );
                   seize();
                   m_seize1_called_d = true;
                   BOOST_TEST_MESSAGE(  "D seize1() end" );
            }
            void clearD() { m_seize_called_d = m_seize1_called_d = m_release_called_d = false; }
            bool m_seize_called_d  = false;
            bool m_seize1_called_d = false;
            bool m_release_called_d = false;
        protected:
            void seize() override
            {
                BOOST_TEST_MESSAGE(  "D::seize() Start" );
                m_seize_called_d = true;
                B::seize();
                BOOST_TEST_MESSAGE ( "D::seize() End");
                // Get derived resources...
            }

            void release() override
            {
                BOOST_TEST_MESSAGE( "D::release() Start"  );
                m_release_called_d = true;
                // Release derived resources...
                B::release();
                BOOST_TEST_MESSAGE( "D::release() End");
            }
        };
        BOOST_TEST_MESSAGE( "D operations");
        D* d = new D();
        // See if the virtual call to seize() resulted in D::seize() being invoked; it should NOT have.
        BOOST_CHECK( !d->m_seize_called_d);

        // See if the virtual call to seize() resulted in D::seize() being invoked; it SHOULD have.
        d->seize1();
        BOOST_CHECK( d->m_seize_called_d);


        BOOST_TEST_MESSAGE( "B operations");
        B* b = new D();
        // See if the virtual call to seize() resulted in D::seize() being invoked; it should NOT have.
        BOOST_CHECK( !(((D*)b)->m_seize_called_d));

        b->seize1();
        // See if the virtual call to seize() resulted in D::seize() being invoked; it SHOULD have.
        BOOST_CHECK( (((D*)b)->m_seize_called_d));

        T_LOG("text")
    }

}

BOOST_AUTO_TEST_SUITE_END()
