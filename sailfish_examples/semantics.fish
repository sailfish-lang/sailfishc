
start {
    
    // assignments
    dec Foo assignment_a = new Foo { i: 1, f: 1.0, s: "hello world" }
    dec Foo assignment_b = new Foo { i: 2, f: 2.0, s: "more hellos"}
    dec flt assignment_c = 10.0

    assignment_a = assignment_b // ok
    assignment_a = 10 // error right hand does not match left hand
    assignment_c = assignment_b // error right hand does not match left hand
    f4 = 1 // error left hand does not exist
    assignment_b = f4 // error right hand does not exist
    good = 10 // cannot assign to functions
    // function calls
    // dec int i = 10
    // i = good(i, "hello world") // ok
    // i = foo(10, i) // error undefined function
    // i = good(10.0, i) // error arg type mismatch
    // i = good(i) // not enough args
    // i = good(i, i, i) // too many args
} 
