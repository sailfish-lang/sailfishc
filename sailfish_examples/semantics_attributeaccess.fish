Cat Foo {
    int i
}

Cfn Foo {}

start {
    dec Foo foo = new Foo {}
    dec flt f = 10.0
    dec int i = 1

    i = foo.i // ok
    f = foo.f // error f does not exist on foo
    f = foo.i // error type mismatch
    f = goo.i // udt does not exist
}