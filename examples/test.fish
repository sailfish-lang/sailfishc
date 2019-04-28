import Foo : "../examples/Foo.fish"

start {
    dec int i = 10
    dec Foo f = new Foo {i: 10}

    f.i = f...bar(f...bar(i))
}