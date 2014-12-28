#include <iostream>
#include <cassert>

#include "rencpp/ren.hpp"
#include "rencpp/runtime.hpp"

using namespace ren;

int main(int, char **) {
    Context contextOne;
    Context contextTwo;

    // test of making which runtime is used in creates a
    // property of some global factor...
   
    int contextNumber = 1;

    Context::setFinder(
        [&](Engine * enginePtr) -> Context & {
            UNUSED(enginePtr);
            if (contextNumber == 1)
                return contextOne;
            if (contextNumber == 2)
                return contextTwo;
            throw std::runtime_error("Invalid context number");
        }
    );
    
    // make a set-word for x, then "apply" it to 10
    // we are creating this in contextOne
    SetWord {"x:"}(10);

    // now print using runtime apply notation
    assert(runtime("integer? x"));

    // switch the runtime that will be found by the next call now...
    contextNumber = 2;

    // we see x is not in this one; don't use individual pieces
    assert(runtime("unset? get/any 'x"));

    // now using the default let's set x in the second runtime...
    SetWord {"x:"}(20);
    assert(runtime("integer? x"));

    // even though our default is to run in the second runtime
    // at the moment, let's override it using "construct" method to create
    // a SetWord bound into contextOne

    auto y = contextOne.construct<SetWord>("y:");
    y(30);

    // Switch active contexts and see that we set y
    contextNumber = 1;
    assert(runtime("integer? get/any 'y"));
    assert(contextOne("integer? get/any 'y"));
}