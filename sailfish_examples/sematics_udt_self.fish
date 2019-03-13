// --------       udt       --------
// good
Cat Foo {
    int i
    flt f
    str s
}

Cfn Foo {
    fn foo
    <- flt i
    -> flt
    {
        i = own.f
        return 10.0
    }
}

start {
    // udt method and attributes
    dec Foo udt_method_attribute_a = new Foo { i : 10 } // ok
    
}