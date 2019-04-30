import Foo : "../examples/Foo.fish"

start {
    dec int i = 8
    dec Foo f = new Foo {i: 1, s: empty}
    dec Foo a = new Foo {i: 2, s: f}
    dec Foo b = new Foo {i: 3, s: a}
    dec Foo c = new Foo {i: 4, s: b}
    dec Foo d = new Foo {i: 5, s: c}

    f = b.s.s
    printInt(b.s...bar(d.s.i))
}
