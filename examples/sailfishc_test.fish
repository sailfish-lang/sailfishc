import Foo : "../examples/Foo.fish"

(fun soFun(int i)(void) {
    # some awesomeness
})

start {
    # basic vars used throughtout
    dec int i = 1
    dec flt f = 1.0
    dec bool b = true
    dec str s = "Hello World"

    # incorrect assignment (=)
    i = 1.0

    # incorrect binary arith (+, **, *, +=)
    i += 1 # correct
    i += 1.0
    i = 1 ** 2 + 3.0 * "hello"

    # incorrect binary comps (<, or, !=)
    b = 1 < 2 # correct
    b = true < false
    b = true or false # correct
    b = 1 or 2
    b = 1 != 2 # correct
    b = true != i

    # incorrect unaries (add, minus, negation)
    ++i # correct
    -- "true"
    !true # correct
    b = !1

    # incorrect dec
    dec int in = 10.0

    # incorrect new dec
    dec Foo f = new Foo {i: 10} # bad redeclaration
    dec Foo fo = new Foo {} # missing params
    dec Foo foo = new Foo {i: 10 } # correct
    
    # incorrect method access
    foo...zoo(void) # does not exist
    f = foo...bar(10) # return mismatch
    i = foo...bar(10) # correct

    # incorrect attribute access
    foo.i = 10 # correct
    foo.i = 10.0 # type mismatch
    foo.i = foo...bar(10) # correct
    i = foo.i # correct
    i = foo.i + 10 # correct
    f = foo.i # type mismatch 
    i = foo.b # nonexistent

    # function param mismatch
    soFun(void)
    soFun(i) # correct
    soFun(foo.i) # correct
    soFun(foo...bar(i)) # correct

    # function return mismatch
    f = soFun(i)

    # list checks
    dec [int] is = [1,2,3] # correct
    dec [int] bads = [1.0,2.0]

    # Tree checks
    Tree (
        # following wants a bool, gets an int
        ( | 1 | { } )
        # correct
        ( | true | { } )
    )
}