Cat Foo {}

Cfn Foo {
    fn foo
    <- int i, flt f
    -> flt
    {
        return 10.0
    }
}

start {
    dec Foo foo = new Foo {}
    dec flt f = 10.0
    dec int i = 1

    f = foo...foo(1, 10.0) // ok
    f = foo...foo(1, 1) // mismatched param
    i = foo...foo(10, 10.0) // mismatched return type
    f = foo...bar(10, 10.0) // undefined method
}