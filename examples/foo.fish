Uat {
    [int] list
    int size
}

Ufn {
    (fun append([int] is)(void) {
        own.list = appendListInt(own.list, is)
    })

    (fun delete(int i)(void) {
        own.list = deleteAtIndexInt(own.list, i)
    })

    (fun get(int i)(int) {
       return getAtIndexInt(own.list, 10)
    })

    (fun set(int i, int v)(void) {
       own.list = setAtIndexInt(own.list, i, v)
    })

    (fun size()(int) {
       return own.size
    })
}