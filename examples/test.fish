import Foo : "../examples/Foo.fish"

start {
    dec int i = 10
    dec Foo f = new Foo {i: 10, s: empty}
    dec Foo b = new Foo {i: 11, s: f}

    f.i = b.s.i
}