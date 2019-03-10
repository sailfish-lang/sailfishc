// -------       function      -------- //
// good
fun good
<- int i, str s
-> flt
{}

// input describes nonexistent type
fun inputDoesNotExist
<- nonExistentType foo
-> void
{}

// input name is a reserved word
fun reserevedKeywordAsInputName
<-flt flt
-> void
{}

// output describes a nonexistent type
fun outputDoesNotExist
<- void
-> nonExistentType
{}

// function name is a reserved type
fun flt
<- void
-> void
{}

// --------       primitives         --------
dec flt function_good = 10.0 // good
dec flt dec = 10.0 // error name is a primitive
dec flt function_foo = 10 // error expression type and assignment type are different

// --------       dictionary        -------
dec dictionary dictionary_good <[ int : flt ]> new { 1 : 1.0 } // good
dec dictionary dictionary_error_a <[ Foo : flt ]> new { 1 : 1.0 } // error key type does not exist
dec dictionary dictionary_error_b <[ int : Foo ]> new { 1 : 1.0 } // error value type does not exist
dec dictionary str <[ int : flt ]> new { 1 : 1.0 } // error name is reserved
dec dictionary dictionary_error_c <[ int : flt ]> new [ 1 ] // error type is not a dictionary
dec dictionary dictionary_error_d <[ int : flt ]> new { 1 : 1.0, 1.0 : 1} // error dictionary is not homogenous
dec dictionary dictionary_error_e <[ int : flt ]> new { 1.0 : 1.0 } // error dictionary keys don't match
dec dictionary dictionary_error_f <[ int : flt ]> new { 1 : 1 } // error dictionary values don't match

// ---------        list         ---------
dec list list_good <[ int ]> new [ 1 ] // good
dec list list_error_a <[ Foo ]> new [ 1 ] // error key type does not exist
dec list list_error_b <[ int ]> new { 1 : 1.0 }  // error type is not a list
dec list list_error_c <[ int ]> new [ 1, 1.0 ]  // error list is not homogenous
dec list list_error_d <[ int ]> new [ 1.0, 1.0 ]  // error list is not same as declared

// --------       udt       --------
// good
Cat Foo {
    int i
    flt f
    str s
}

Cfn Foo {
    fn foo
    <- int i
    -> flt
    {}
}

// errors
Cat bool  { 
    flt flt // illegal name
    void // illegal type
    udt u // unknown type
}

Cfn bool {}

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

    // udt method and attributes
    // dec Foo udt_method_attribute_a = new Foo { i : 10 } // ok
    // dec Foo udt_method_attribute_b = new Foo { a : 10 } // error nonexistent attribute
    // dec Foo udt_method_attribute_c = new Foo { i : 10.0 } // error attribute type mismatch
    // method_attribute_a = udt_method_attribute_a.i // ok
    // method_attribute_b = udt_method_attribute_a...foo(10) //ok
   // method_attribute_c = udt_method_attribute_a.t // error no such attribute
   // method_attribute_d = udt_method_attribute_a...bar() //  error no such method
    
    // declare f for all subsequent tests
    dec bool f = true

    // +
    f = 10.0 + 10.0 // ok
    f = 1 + 2 // ok
    f = false + true // error
    f = 10 + 10.0 // error
    f = 10.0 + 10 // error

    // -
    f = 10.0 - 10.0 // ok
    f = 1 - 2 // ok
    f = false - true // error
    f = 10 - 10.0 // error
    f = 10.0 - 10 // error


    // *
    f = 10.0 * 10.0 // ok
    f = 1 * 2 // ok
    f = false * true // error
    f = 10 * 10.0 // error
    f = 10.0 * 10 // error

    // /
    f = 10.0 / 10.0 // ok
    f = 1 / 2 // ok
    f = false / true // error
    f = 10 / 10.0 // error
    f = 10.0 / 10 // error

    // **
    f = 10 ** 10 // ok
    f = 1 ** 2.0 // error
    f = false ** 1 // error

    // %
    f = 10 % 10 // ok
    f = 1 % 2.0 // error
    f = false % 1 // error

    // > comp
    f = 10.0 > 10.0 // ok
    f = 1 > 2 // ok
    f = false > true // error
    f = 10 > 10.0 // error
    f = 10.0 > 10 // error

    // >= comp
    f = 10.0 >= 10.0 // ok
    f = 1 >= 2 // ok
    f = false >= true // error
    f = 10 >= 10.0 // error
    f = 10.0 >= 10 // error

    // < comp
    f = 10.0 < 10.0 // ok
    f = 1 < 2 // ok
    f = false < true // error
    f = 10 < 10.0 // error
    f = 10.0 < 10 // error

    // <= comp
    f = 10.0 <= 10.0 //ok
    f = 1 <= 2 // ok
    f = false <= true // error
    f = 10 <= 10.0 // error
    f = 10.0 <= 10 // error

    // equivalence comp
    f = false == true // ok
    f = true == 10 // error
    f = 10 == false // error

    // nonequivalence comp
    f = false != true // ok
    f = true != 10 // error
    f = 10 != false // error

    // and comp
    f = false and true // ok
    f = true and 10 // error
    f = 10 and false // error
    
    // or comp
    f = false or true // ok
    f = true or 10 // error
    f = 10 or false // error

    // negation
    f = !true // ok
    f = !10 // error
} 
