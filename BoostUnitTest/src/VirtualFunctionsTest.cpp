/*
*
 * VirtualDunctions.cpp
 *
 *  Created on: March 26, 2017
 *      Author: Burley
 *      Brief: Demo for virtual functions. Also demo that virtual functions should not be called
 *      in the constructor or the destructor. Also illustrates the "override" identifier.
 *
 */
 #include <iostream>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <SharedLibrary/inc/MyLogger.hpp>

using namespace std;
using namespace boost;

BOOST_AUTO_TEST_SUITE( virtualFunctions )

// show that calling virtual functions within a destructor or constructor is non-compliant
// also demonstrate the use of the identifier "override"; a C++11 feature to indicate that a method
// in the derived class is overriding a virtual base class method.
BOOST_AUTO_TEST_CASE( VirtFunctInCtorDtor )
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


    BOOST_TEST_MESSAGE( "D non-compliant construction");
    D* d = new (std::nothrow) D();
    // See if the virtual call to B() -> calls seize(), which should result in D::seize() being invoked;
    // However, gnu compiler is smart enough not to call D::seize() from B's constructor.
    // Calling a virtual inside a constructor in non-compliant and additionally produces a non-compliant
    // warning on compilation.

    BOOST_CHECK ( d );
    BOOST_CHECK( !d->m_seize_called_d);

    // See if the virtual call to seize() resulted in D::seize() being invoked; it SHOULD have.
    d->seize1();
    BOOST_CHECK( d->m_seize_called_d);

    BOOST_TEST_MESSAGE( "D non-compliant construction");
    B* b = new (std::nothrow) D();
    BOOST_CHECK ( b );

    // See if the virtual call to seize() resulted in D::seize() being invoked; it should NOT have.
    BOOST_CHECK( !(((D*)b)->m_seize_called_d));

    b->seize1();
    // See if the virtual call to seize() resulted in D::seize() being invoked; it SHOULD have.
    BOOST_CHECK( (((D*)b)->m_seize_called_d));


}

// Start of test "PureVirtFunctImplementation"
// Show that a pure virtual method can have an implementation.  There is nothing wrong with that.
//  The pure just means that class B1 cvannot be directly instantiated.
// Define some classes for test "PureVirtFunctImplementation" outside of the test..
// class definitions can't go inside the actual boost test; get some kind of compiler error.
class B1  // (B)ase class.
{
public:
      B1() = default;
      virtual ~B1() = 0;
      virtual void message() { cout << "B1::message()" << endl; }
      static bool m_destructorCalled_B1;
};
bool B1::m_destructorCalled_B1 = false;
B1::~B1() { B1::m_destructorCalled_B1 = true;  T_LOG("dtor called") }

class D1 : public B1 // (D)erived class
{
public:
    D1() = default;
    virtual ~D1() = default;
    virtual void message() final { cout << "D1::message()" << endl; }
};


BOOST_AUTO_TEST_CASE( PureVirtFunctImplementation )
{
   //  B1 b1;  will not compile.. cannot instatiate a pure virtual ( astract) class.
    BOOST_CHECK(!B1::m_destructorCalled_B1);
    D1* d1 = new (std::nothrow) D1();
    BOOST_CHECK( d1 );

    d1->message();
    delete d1;
    BOOST_CHECK(B1::m_destructorCalled_B1);

}

// This test, on older compilers, causes a runtime error due to calling a pure virtual function.
// This can happen by using non-compliant code that calls virtual functions
// in the constructor or destructor.
// The GNU C++11 compiler with cygwin is smart enough to flag a compiler error if an implementation
// is NOT provided  for a pure virtual function AND the constructor/destructor calls it.  If the compiler
// does not check for this and allows it, then a runtime virtual error occurs.
// If an implementation is provided, then the pure virtual method's implementation will be called in the base class,
// not the derived class.  This is considered non-compliant behavior.
//  The code below provides an implementation for a pure virtual function.  The compiler does generate a warning to indicate
// that a virtual function is being called in the constructor/destructor.
// warning: pure virtual 'virtual void virtualFunctions::B2::doIt()' called from constructor
// warning: pure virtual 'virtual void virtualFunctions::B2::doIt()' called from destructor
//
// Note the override on method D::doIt().  This indicates that this is a virtual method,
// that is overriding a virtual method of the base class."

class B2   // (B)ase class
{
public:
    B2() { T_LOG("B2::ctor"); doIt(); }  // create a compiler warning;
                    // warning: pure virtual
                    // 'virtual void virtualFunctions::RunTimeVirtualError::test_method()::B::doIt()'
                    // called from constructor
   virtual void doIt() = 0;
   virtual ~B2() { T_LOG("B2::dtor"); doIt(); }
};
// provide implementation for pure virtual function.
void B2::doIt() { T_LOG("B2::doIt()"); }

class D2 : public B2 // (D)erived class
{
public:
    D2() { T_LOG("D2 ctor"); }
    void doIt() override { T_LOG("D2::doit()") }
    ~D2()  { T_LOG("D2 dtor"); }
};

BOOST_AUTO_TEST_CASE( InvokingPureVirtualFunction )
{
    D2* d2 = new (std::nothrow) D2();
    BOOST_CHECK( d2 );
    delete d2;
}

BOOST_AUTO_TEST_SUITE_END()


