import intListHandler : "../examples/intListHandler.fish"

(fun merge(intListHandler a, intListHandler b) (intListHandler) {
    # create another empty list for merging
    dec intListHandler c = new intListHandler { list: [], size: 0}
    
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

(fun sublist(intListHandler a, intListHandler b, int front, int back)(intListHandler) {
    Tree (
        ( | front == back | { 
            dec int j = a...peek(front)
            b...push(j)
            return b
        } )
        ( | true | {
            dec int j = a...peek(front)
            b...push(j)
            sublist(a, b, front+1, back)
        })
    )

    return b
})

(fun mergesort(intListHandler a, int front, int end)(intListHandler) {
    Tree (
        ( | (front == end) or ((end - front) == 1) | {
            dec int i = a...peek(front)
            dec intListHandler b = new intListHandler { list: [i], size: 1}
            return b
        })

    # find middle
    dec int middle = (end - front) / 2

    # generate two empty lists for recursively dividing
    dec intListHandler f = new intListHandler { list: [], size: 0}
    dec intListHandler b = new intListHandler { list: [], size: 0}

    f = sublist(a, f, front, middle-1)
    b = sublist(a, b, middle, end-1)

    return merge(
        mergesort(f, 0, f.size),
        mergesort(b, 0, b.size)
    )
})

start {
    dec intListHandler a = new intListHandler { list: [5, 10, 12, 3, 4, 2, 11, 1], size: 8}
    a = mergesort(a, 0, a.size)
    a...display(void)
}