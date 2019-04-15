import foo : "../examples/foo.fish"

(fun bar(int z)(int){
    z = 10
    return z
})

start {
    dec int i = 10
    dec str s = "Hello World"

    Tree (
        (| i == 10 | {
            Tree (
                (| true | { i = 11 } )
                (| "watermelon" == "tasty" | { i = 11 } )
            )
        })

        (| i != 10 | {
            i = 2
        })

        (| (10 < i) and (10 < (i + 1)) | {
            i = 2
        })
    )

    i = bar(i)

    dec foo fuh = new foo { i:10, fl:10.0, s: empty }

    dec [flt] is = [1.0, 2.0, 2020.0]
}