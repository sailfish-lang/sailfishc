import foo : "../examples/foo.fish"

(fun displayFoo(foo is, int s)(void) {
    Tree (
         ( | s == 0 | { 
            printStr("The End!")
        })
        ( | s !=0 | { 
            --s
            printInt(is...get(s))
            displayFoo(is, s)
        })
    )
})

start {
    dec foo s = new foo { list: [1,2,3], size: 3 }    
}
