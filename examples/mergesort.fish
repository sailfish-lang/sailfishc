import intListHandler : "../examples/intListHandler.fish"

(fun merge(intListHandler a, intListHandler b, intListHandler c) (intListHandler) {
    Tree (
        ( | a.size == 0 | {
            c.list = appendListInt(c.list, b.list, c.size, b.size)
            c.size = b.size + c.size
            return c
        })

        ( | b.size == 0 | {
            c.list = appendListInt(c.list, a.list, c.size, a.size)
            c.size = a.size + c.size
            return c
        })

        ( | a...peek_front(void) < b...peek_front(void) | {
            dec int i = a...peek_front(void)
            c...push(i)
            a...removeByIndex(0)
        })

        ( | a...peek_front(void) >= b...peek_front(void) | {
            dec int i = b...peek_front(void)
            c...push(i)
            b...removeByIndex(0)
        })
    )

    return merge(a,b,c)
})

start {
    dec intListHandler a = new intListHandler { list: [1, 3, 5], size: 3}
    dec intListHandler b = new intListHandler { list: [2, 4, 7], size: 3}
    dec intListHandler c = new intListHandler { list: [], size: 0}

    dec intListHandler d = new intListHandler { list: [7, 11, 12], size: 3}
    dec intListHandler e = new intListHandler { list: [2, 4, 13, 14, 15], size: 5}
    dec intListHandler f = new intListHandler { list: [], size: 0}

    dec intListHandler g = new intListHandler { list: [], size: 0}

    c = merge(a,b,c)
    f = merge(d,e,f)
    g = merge(c,f,g)
    

    g...display(void)
}