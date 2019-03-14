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
    {
        i = own.i
        return 10.0
    }
}

// errors
Cat bool  { 
    flt flt // illegal name
    void // illegal type
    udt u // unknown type
}

Cfn bool {}

start {
    // udt method and attributes
    dec Foo udt_method_attribute_a = new Foo { i : 10 } // ok
    dec Foo udt_method_attribute_b = new Foo { a : 10 } // error nonexistent attribute
    dec Foo udt_method_attribute_c = new Foo { i : 10.0 } // error attribute type mismatch
}