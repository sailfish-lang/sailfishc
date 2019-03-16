Cat Foo {
    int i
    Foo f
}

Cfn Foo {
    fun get_i
    <- void
    -> int
    {
        return own.i
    }

    fun bar
    <- void
    -> int
    {
        dec int i = 10
        i = own.f...get_i()
        return i
    }
}

start {
    dec Foo f = new Foo { i: 1, f: void }
    dec Foo b = new Foo { i: 20, f: f }
    
    display_int(b...bar())
    display_str("\n")
}
