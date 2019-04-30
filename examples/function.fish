(fun foo(int i)(str) {
    Tree (
        ( | i == 5 | { return "I received a five!"})
        ( | i == 6 | { return "I gotta size?!?"})
    )
    return "Just some number that is not a five or six."
})

start {

}
