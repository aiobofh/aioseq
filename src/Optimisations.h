/**
 * Optimisations.h
 *
 * Various macros for optimisation of C++ pure virtuals.
 *
 * These macros enables a syntax efficient data-type declaration to be used
 * for member variables of classes, or typecasts in constructor assignments.
 *
 * @code
 * class AbstractBar {
 * public:
 *   AbstractBar() {};
 *   virtual void bar() = 0;
 * };
 *
 * class Bar : public AbstractBar
 * public:
 *   Bar() {};
 *   void bar() { ... Some implementation ... };
 * };
 *
 * class Foo
 * private:
 *   Abstract(Bar) *bar;
 * public:
 *   Foo(AbstractBar *bar) : bar(Abstract(Bar)) {};
 *   void foo() { bar->TryInline(Bar)bar(); }
 * };
 * @endcode
 *
 * This could have been done using templates to some degree, however this
 * solution is very efficient.
 *
 * What will happen is that when running test-cases (which is the only
 * known use for abstract pure virtual interfaces in this application so
 * far) all method calls are done using the interface API, when in
 * production mode the code can be _highly_ optimised by the compiler,
 * using som C++ syntactical tricks.
 *
 * For example, when calling methods with the TryInline macro in production
 * mode the code will translate into calling a method using the scope of
 * the concrete class instead of calling the method using the interface.
 * This is a neat thing for the compiler, enabling it to even do produce
 * assembler code inlined at the function call. Awesome! :) Note that the
 * reference to the class must be of the same type, so the reference can be
 * easily transformed by the Abstract macro as described in the example above.
 *
 * If you already have the concrete implemetation stored by explicit type cast
 * or similar you can also try this macro to give the compiler a chance to do
 * its best when it comes to optimising the assembler code.
 */

#ifndef _OPTIMISATIONS_H_
#define _OPTIMISATIONS_H_

#ifndef UNIT_TEST
#define Abstract(class_name) Abstract##class_name
#define TryInline(class_name) class_name::
#else
#define Abstract(class_name) class_name
#define TryInline(class_name)
#endif

#endif
