Uat {
    int i
    Foo s
}

Ufn {
    (fun bar(int i)(int) {
        return (i + own.i)
    })

    (fun self(void)(Foo) {
        return own.s
    })
}