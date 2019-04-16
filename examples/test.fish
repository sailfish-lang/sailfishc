import foo : "../examples/foo.fish"

start {
    dec [int] list = [1,2,3]
    dec int size = 3
    dec foo bar = new foo { list: list, size: size}

    bar...dispSize(void)
    bar...incrementSize(void)
    bar...incrementSize(void)
    bar...incrementSize(void)
    bar...incrementSize(void)
    bar...incrementSize(void)
    bar...dispSize(void)
}