import foo : "../examples/foo.fish"

(fun money(int i)(int){
    return i
})

start {

    Tree (
        ( | true | { dec flt f = 10.0 } )
        ( | false | { dec bool b = false } )
    )

    dec foo fluf = new foo { i: 10 }
    
    dec int size = money(money(money(money(20))))
    printInt(size)

    size = fluf.i
    printInt(size)

    printInt( fluf...baz(size))
}